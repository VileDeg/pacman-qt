#ifndef REPLAY_H
#define REPLAY_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDataStream>

class GameScene;

struct ReplayFlags {
    bool StepByStep = false;
    bool OneStep = false;
    bool Forward = true;
    bool Paused = false;
};

struct GameState {
    bool gameOver = false;

    bool win = false;
    int score = 0;
    int steps = 0;

    friend QDataStream& operator<<(QDataStream& stream, const GameState& data)
    {
        stream << data.gameOver << data.win << data.score << data.steps;
        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, GameState& data)
    {
        stream >> data.gameOver >> data.win >> data.score >> data.steps;
        return stream;
    }

    static int sizeOf() {
        return (2 * sizeof(bool) + 2 * sizeof(int));
    }
};

class Serializer : public QObject {

    Q_OBJECT
signals:
    void replayFlagsChanged(ReplayFlags);
    void gameStateChanged(GameState);
public slots:
    void onGameStateChanged(GameState);

    void toggleReplayPaused();
    void toggleReplayDir();
    void toggleReplayMode();

    void replayStepNext();
    void replayStepBack();

    void replayJumpToStart();
    void replayJumpToEnd();
private slots:
    void onSerialize();
    void onDeserialize();

    void onStepTimeout();
public:
    Serializer(QObject* parent = nullptr);
    ~Serializer() {}

    void Init(GameScene* scene, QString filePath, bool recorded, bool replayFromStart);
private:
    void replayJumpTo(bool toStart);
private:
    GameScene* _scene = nullptr;

    QFile _file;
    QDataStream _stream;
    QTimer* _serializationTimer = nullptr;
    QTimer* _stepTimer = nullptr;

    ReplayFlags _rf;
    GameState _state;

    int _serializationFPS = 24;
    int _serializationInterval = 100;
    int _stepInterval = 250;
    int _framesPerStep = 10;
    int _msSinceLastStep = 0;

    int _frameDataSize = 0;

    int _filePosFrameDataStart = 0;
    int _filePosFrameDataEnd = 0;
};

#endif // REPLAY_H