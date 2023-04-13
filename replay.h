#pragma once

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDataStream>

#include "interfaces.h"

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
        stream << data.win << data.score << data.steps;
        return stream;
    }

    friend QDataStream& operator>>(QDataStream& stream, GameState& data)
    {
        stream >> data.win >> data.score >> data.steps;
        return stream;
    }
};

class Serializer : public QObject {

    Q_OBJECT
signals:
    void replayFlagsChanged(const ReplayFlags& rf);
public:
    Serializer(QString filePath, bool replay, QObject* parent = nullptr);
    ~Serializer();

private:
    ISerializable* _target = nullptr;

    QFile _file;
    QDataStream _stream;
    QTimer* _serializationTimer;
    QTimer* _stepTimer;

    ReplayFlags _rf;

    int _serializationFPS = 24;
    int _serializationInterval = 100;
    int _stepInterval = 250;
    int _framesPerStep = 10;
    int _msSinceLastStep = 0;

    int _filePosFrameDataStart = 0;
    int _filePosFrameDataEnd = 0;
};