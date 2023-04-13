#pragma once

#include <QDataStream>

class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual void Serialize(QDataStream& stream) = 0;
    virtual void Deserialize(QDataStream& stream) = 0;
};