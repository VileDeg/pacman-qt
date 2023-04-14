/** @file replay.h
 *  @author Vadim Goncearenco <xgonce00@stud.fit.vutbr.cz>
 *  @brief File with declration of structures and classes that are related to replay functionality.
 *  @details This file contains replay flags, game state and serializer declaration.
 *  The program allows to record and replay the game. 
 *  User can replay game in both directions (forward and backward),
 *  starting from the game start or from the game end. 
 *  User can also choose between Normal Mode and Step by Step Mode.
 *  In Normal Mode the game is replayed continuously frame after frame.
 *  In Step by Step Mode the game is replayed by several steps at a time with a set interval between steps.
 *  User can also make just one step forward or backward without switching to Step by Step Mode.
 *  Replay can also be paused anytime.
 */

#ifndef REPLAY_H
#define REPLAY_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDataStream>

class GameScene;

/**
 * @brief Replay flags structure.
 * @details Contains all flags that are used for controlling replay.
 */
struct ReplayFlags {
    bool StepByStep = false;
    bool OneStep = false;
    bool Forward = true;
    bool Paused = false;
};

/**
 * @brief Game state structure.
 * @details Contains all game state data that is serialized 
 * or used to inform UI and scene about game state changes.
 */
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

/**
 * @brief Serializer class.
 * @details This class is responsible for serializing and deserializing the scene.
 * It also controls replay functionality by setting replay flags and emitting signals.
 */
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
    void onSerialize(); /**< Slot that is called when serialization timer times out. */
    void onDeserialize(); /**< Slot that is called when deserialization timer times out. */ 

    void onStepTimeout(); /**< Slot that is called when step timer times out. */
    
public:
    Serializer(QObject* parent = nullptr);
    ~Serializer() {}

    void Init(GameScene* scene, QString filePath, bool recorded, bool replayFromStart);
private:
    void replayJumpTo(bool toStart);

private:
    GameScene* _scene = nullptr;

    QFile _file; /**< File that is used for serialization/deserialization. */
    QDataStream _stream; /**< Stream that is used for serialization/deserialization. */
    QTimer* _serializationTimer = nullptr; /**< Timer that is used for serialization. */
    QTimer* _stepTimer = nullptr; /**< Timer that is used for replay in step by step mode. */

    ReplayFlags _rf = {}; /**< Replay flags. */
    GameState _state = {}; /**< Current game state. */

    int _serializationFPS = 24; 
    int _serializationInterval = 100;

    int _stepInterval = 250;
    int _framesPerStep = 10;
    int _msSinceLastStep = 0;

    int _frameDataSize = 0; /**< Size of one frame data in bytes (game state size + scene size). */

    int _filePosFrameDataStart = 0; /**< Byte offset in file where the data of game frames starts. */
    int _filePosFrameDataEnd = 0; /**< Byte offset in file where the data of game frames ends. */
};

#endif // REPLAY_H