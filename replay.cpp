#include "replay.h"
#include "gamescene.h"

//(sizeof(GameState))
//#define SER_GAME_END_DATA (2 * sizeof(bool) + 2 * sizeof(int))

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

        _stream << _scene->_mapString;
    } else { // Replay game
        _rf = ReplayFlags(); // Reset replay flags
        connect(_serializationTimer, SIGNAL(timeout()), this, SLOT(onDeserialize()));

        qint64 pos = _file.pos();
        _stream >> _scene->_mapString;
        auto diff = _file.pos() - pos;
        _filePosFrameDataStart = pos + diff;

        auto endData = GameState::sizeOf();
        pr("DES game end data: " << endData); //SER_GAME_END_DATA
        _filePosFrameDataEnd = _file.size() - endData;
        _scene->parseMap(&_scene->_mapString);

        pr("DES File size: " << _file.size());
        { // Read first frame to determine it's size
            pos = _file.pos();
            //bool isLastFrame;
            GameState gs;
            _stream >> gs;
            _scene->Deserialize(_stream);
            //_sceneDataSize = _file.pos() - pos;
            _frameDataSize = _file.pos() - pos;
            pr("DES Frame Data Size: " << _frameDataSize);
            _file.seek(_file.pos() - _frameDataSize); // Go back to start of frame data
        }

        replayJumpTo(replayFromStart);
        pr("DES Starting at: " << _file.pos());

        /*pr("Frame data start at: " << _file.pos());
        pr("MapString size: " << _scene->_mapString);*/
    }
    _serializationTimer->start(0);
    _serializationTimer->stop();
    _serializationTimer->start(_serializationInterval);
}

void Serializer::onSerialize()
{
    ASSERT(_scene != nullptr);
    static int frame = 0;

    //bool isLastFrame = false;
    /*if (_gameEnded) {

        return;
    }*/

    //_stream << false;
    pr("SER Frame: " << frame << " " << _file.pos());
    GameState gs = _scene->getGameState();
    _stream << gs;
    if (gs.gameOver) {
        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }

    _scene->Serialize(_stream);
    ++frame;
}

void Serializer::onDeserialize()
{
    ASSERT(_scene != nullptr);
    

    if (_rf.Paused) {
        return;
    }

    static int frame = 0;

    if (_rf.OneStep || _rf.StepByStep) {
        //qint64 pos = _file.pos();
        //pr("DES file pos 0: " << pos);
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
        //pr("DES file pos 1: " << pos);
        pr("DES paused");
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

    pr("DES Frame: " << frame << " " << _file.pos());
    GameState gs;
    _stream >> gs;
    emit gameStateChanged(gs);
    /*bool isLastFrame = false;
    _stream >> isLastFrame;*/

    if (gs.gameOver) { //TODO:
        

        //emit gameStateChanged(gs);

        _serializationTimer->stop();
        _file.flush();
        _file.close();
        return;
    }
    ASSERT(!_file.atEnd());
    ASSERT(_file.pos() + _frameDataSize <= _file.size());

    _scene->Deserialize(_stream);

    //pr("DES file pos 1: " << _file.pos());

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

    ++frame;
}

void Serializer::onGameStateChanged(GameState gs)
{
    if (gs.gameOver && _state.gameOver != gs.gameOver) {
        onSerialize();
        //serializationEnd(gs);
    }
    _state = gs;
}

//void Serializer::serializationEnd(GameState gs)
//{
//    //bool isLastFrame = true;
//    //pr("File pos end 0: " << _file.pos());
//    //_stream << isLastFrame;
//    _stream << gs;
//    //Scene is already deleted by this moment
//    /*_stream << _playerScore;
//    _stream << _playerSteps;*/
//    //pr("File pos end 1: " << _file.pos());
//
//    _serializationTimer->stop();
//    _file.flush();
//    _file.close();
//}

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
