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
        _rf = ReplayFlags(); // Reset replay flags
        connect(_serializationTimer, SIGNAL(timeout()), this, SLOT(onDeserialize()));

        qint64 pos = _file.pos();
        QString mapString;

        _stream >> mapString;
        _scene->setMapString(mapString);

        auto diff = _file.pos() - pos;
        _filePosFrameDataStart = pos + diff;

        _filePosFrameDataEnd = _file.size() - GameState::sizeOf();

        _scene->parseMap(&mapString);

        { // Read first frame to determine it's size
            pos = _file.pos();
            GameState gs;
            _stream >> gs;
            _scene->Deserialize(_stream);
            _frameDataSize = _file.pos() - pos;
            _file.seek(_file.pos() - _frameDataSize); // Go back to start of frame data
        }

        replayJumpTo(replayFromStart);
    }
    _serializationTimer->start(0);
    _serializationTimer->stop();
    _serializationTimer->start(_serializationInterval);
}

void Serializer::onSerialize()
{
    ASSERT(_scene != nullptr);

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
    
    if (_rf.Paused) {
        return;
    }

    if (_rf.OneStep || _rf.StepByStep) {
        
        qint64 seekPos = _file.pos();
        if (_rf.Forward) {
            seekPos += _framesPerStep * _frameDataSize;
            while (seekPos >= _file.size()) {
                seekPos -= _frameDataSize;
            }

        } else {
            seekPos -= _framesPerStep * _frameDataSize;
            while (seekPos < _filePosFrameDataStart) {
                seekPos += _frameDataSize;
            }
        }
        ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
        _file.seek(seekPos);
        _rf.Paused = true;
        if (_rf.StepByStep) {
            _stepTimer->start(_stepInterval);
        }
    } else if (!_rf.Forward) {
        qint64 seekPos = _file.pos() - _frameDataSize;
        if (seekPos >= _filePosFrameDataStart) {
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        }
    }

    GameState gs;
    _stream >> gs;
    emit gameStateChanged(gs);
    
    if (gs.gameOver) {
        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }
    ASSERT(!_file.atEnd());
    ASSERT(_file.pos() + _frameDataSize <= _file.size());

    _scene->Deserialize(_stream);
    
    if (!_rf.Forward) {
        if (_rf.OneStep) {
            qint64 seekPos = _file.pos() - _framesPerStep * _frameDataSize;
            while (seekPos < _filePosFrameDataStart) {
                seekPos += _frameDataSize;
            }
            ASSERTMSG(seekPos >= 0 && seekPos < _file.size(), "Invalid position in file");
            _file.seek(seekPos);
        } else {
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
