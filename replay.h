#pragma once

#include "sprite.h"

#include "interfaces.h"

struct SceneReplayData : public ISerializable
{
    SceneReplayData();

    void Serialize(QDataStream& stream) override;
    void Deserialize(QDataStream& stream) override;

    QString _mapString;
    QSize _mapSize;

    Sprite*** _map;
};

