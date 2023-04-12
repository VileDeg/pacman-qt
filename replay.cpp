#include "replay.h"

SceneReplayData::SceneReplayData()
{
}


void SceneReplayData::Serialize(QDataStream& stream)
{
    
}

void SceneReplayData::Deserialize(QDataStream& stream)
{
    //for (int i = 0; i < _mapSize.width(); ++i) {
    //    for (int j = 0; j < _mapSize.height(); ++j) {
    //        if (_map[i][j]) {
    //            _map[i][j]->Deserialize(stream);
    //            _map[i][j]->setImage(&_pixmapCache[_map[i][j]->getType()]);
    //            _map[i][j]->update(); // Repaint
    //        }
    //    }
    //}

    //int enemiesAnimFrame;
    //stream >> enemiesAnimFrame;
    //for (auto& enemy : _enemies) {
    //    enemy->Deserialize(stream);
    //    enemy->setSpriteByFrame(enemiesAnimFrame);
    //}

    //int playerAnimFrame;
    //stream >> playerAnimFrame;
    //_player->Deserialize(stream);
    //_player->setSpriteByFrame(playerAnimFrame);
}
