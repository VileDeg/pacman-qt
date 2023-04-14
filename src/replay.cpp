/** @file replay.cpp
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with serializer class definition.
 *  @details This file contains serializer class definition.
 */

#include "replay.h"
#include "gamescene.h"

Serializer::Serializer(QObject* parent) 
    : QObject(parent)
{}

void Serializer::Init(GameScene* scene, QString mapPath, bool recorded, bool replayFromStart)
{
    _scene = scene;
    _serializationTimer = new QTimer(this);
    _stepTimer = new QTimer(this);
    connect(_stepTimer, SIGNAL(timeout()), this, SLOT(onStepTimeout()));

    _serializationInterval = 1000 / _serializationFPS;
    _framesPerStep = _stepInterval / _serializationInterval;

    auto name = mapPath.split('/').last().split('.').first();
    _file.setFileName("saves/" + name + ".bin");
    if (!_file.open(recorded ? QIODevice::ReadOnly : QIODevice::WriteOnly)) {
        throw std::runtime_error("Error opening file: " + _file.fileName().toStdString() + ": " + _file.errorString().toStdString());
    }
    _stream.setDevice(&_file);

    if (!recorded) { // Game wasn't recorded so we need to record it
        connect(_serializationTimer, SIGNAL(timeout()), this, SLOT(onSerialize()));

        _stream << _scene->getMapString();
    } else { // Replay game
        connect(_serializationTimer, SIGNAL(timeout()), this, SLOT(onDeserialize()));

        _rf = ReplayFlags(); // Reset replay flags

        qint64 pos = _file.pos(); // Save current position in file
        QString mapString;

        _stream >> mapString; // Read map contens from file
        _scene->setMapString(mapString);

        auto diff = _file.pos() - pos; // Calculate difference between current position and start

        _filePosFrameDataStart = pos + diff; // Calculate start of frame (game state and scene) data
        _filePosFrameDataEnd = _file.size() - GameState::sizeOf(); // Calculate end of frame (game state and scene) data

        _scene->parseMap(&mapString); // Load map from string

        { // Read first frame to determine it's size
            pos = _file.pos();
            GameState gs;
            _stream >> gs;
            _scene->Deserialize(_stream);
            _frameDataSize = _file.pos() - pos;
            _file.seek(_file.pos() - _frameDataSize); // Go back to start of frame data
        }

        // Jump to start of replay based on replay start point (start or end)
        replayJumpTo(replayFromStart); 
    }
    _serializationTimer->start(0);
    _serializationTimer->stop();
    _serializationTimer->start(_serializationInterval);
}

void Serializer::onSerialize()
{
    ASSERT(_scene != nullptr);

    // Write game state and scene to file
    GameState gs = _scene->getGameState();
    _stream << gs;
    if (gs.gameOver) {
        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }

    _scene->Serialize(_stream);
}

void Serializer::onDeserialize()
{
    ASSERT(_scene != nullptr);
    
    // If replay is paused, wait for unpause
    if (_rf.Paused) {
        return;
    }

    // If replay by step is enabled, wait for step
    if (_rf.OneStep || _rf.StepByStep) {
        // A step is several frames long so we need to move file pointer to the end of step        
        qint64 seekPos = _file.pos();
        if (_rf.Forward) { // If replay is forward, move file pointer further
            seekPos += _framesPerStep * _frameDataSize;
            while (seekPos >= _file.size()) {
                seekPos -= _frameDataSize;
            }
        } else { // If replay is backward, move file pointer back
            seekPos -= _framesPerStep * _frameDataSize;
            while (seekPos < _filePosFrameDataStart) {
                seekPos += _frameDataSize;
            }
        }
        ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");

        _file.seek(seekPos); // Move file pointer
        // Pause replay until step button is clicked or step timer times out
        _rf.Paused = true; 
        if (_rf.StepByStep) { // If step by step is enabled, start step timer
            _stepTimer->start(_stepInterval);
        }
    // If not replay by step and replay backward is enabled, move file pointer back by one frame
    } else if (!_rf.Forward) {
        qint64 seekPos = _file.pos() - _frameDataSize;
        if (seekPos >= _filePosFrameDataStart) {
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        }
    }

    // Read frame data (game state and scene) from file

    GameState gs;
    _stream >> gs;
    emit gameStateChanged(gs);
    // If it is the last frame, end replay
    if (gs.gameOver) {
        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }
    ASSERT(!_file.atEnd());
    ASSERT(_file.pos() + _frameDataSize <= _file.size());

    _scene->Deserialize(_stream);
    
    // If replay backward is enabled, move file pointer back
    if (!_rf.Forward) {
        if (_rf.OneStep) { // If replay by step is enabled, move file pointer back by one step
            qint64 seekPos = _file.pos() - _framesPerStep * _frameDataSize;
            while (seekPos < _filePosFrameDataStart) {
                seekPos += _frameDataSize;
            }
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        } else { // Else move file pointer back by one frame
            qint64 seekPos = _file.pos() - _frameDataSize;
            if (seekPos >= _filePosFrameDataStart) {
                ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
                _file.seek(seekPos);
            }
        }
    }
}

void Serializer::onGameStateChanged(GameState gs)
{
    if (gs.gameOver && _state.gameOver != gs.gameOver) {
        onSerialize();
    }
    _state = gs;
}

void Serializer::onStepTimeout()
{
    _stepTimer->stop();
    _rf.Paused = false;
}

void Serializer::toggleReplayPaused()
{
    _rf.OneStep = false;

    _rf.Paused = !_rf.Paused;
    emit replayFlagsChanged(_rf);
}

void Serializer::toggleReplayDir()
{
    _rf.Paused = false;
    _rf.OneStep = false;

    _rf.Forward = !_rf.Forward;
    emit replayFlagsChanged(_rf);
}

void Serializer::toggleReplayMode()
{
    _rf.Paused = false;
    _rf.OneStep = false;

    _rf.StepByStep = !_rf.StepByStep;
    if (!_rf.StepByStep) {
        _stepTimer->stop();
    }
    emit replayFlagsChanged(_rf);
}

void Serializer::replayJumpToStart()
{
    replayJumpTo(true);
}

void Serializer::replayJumpToEnd()
{
    replayJumpTo(false);
}

void Serializer::replayStepNext()
{
    _rf.Paused = false;
    _rf.OneStep = true;

    _rf.Forward = true;
    emit replayFlagsChanged(_rf);
}

void Serializer::replayStepBack()
{
    _rf.Paused = false;
    _rf.OneStep = true;

    _rf.Forward = false;
    emit replayFlagsChanged(_rf);
}

void Serializer::replayJumpTo(bool toStart)
{
    _rf.Paused = false;
    _rf.OneStep = false;

    ASSERT(_file.isOpen());
    ASSERT(_file.size() >= _frameDataSize);

    if (toStart) {
        _file.seek(_filePosFrameDataStart);
        _rf.Forward = true;
    } else {
        _file.seek(_filePosFrameDataEnd - _frameDataSize);
        _rf.Forward = false;
    }

    emit replayFlagsChanged(_rf);
}
