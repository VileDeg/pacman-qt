Nothing was generated using QT Creator. All UI elements were created manually.

Implemented functionality:
- Everything stated in the task description was implemented:
- - Loding of different maps from text files.
- - Interactive game elements are: ghosts, collectable balls, keys, door, player score, player steps.
- - Moving of the player with arrow keys and by mouse click (A* algorithm used for pathfinding).
- - Recording of the game and replaying it in both directions, switching between modes, step by step and etc.

Map loading:
- Maps are loaded from "examples/" folder and user can choose from menu which one to load.
- Map loading is accessible either from "File" menu or by "Load map" button in center of the screen.

Game recording:
- Game recordings are saved to "saves/" folder and user can choose from menu which one to replay.
- Replaying a game is accessible either from "File" menu or by "Load map" button in center of the screen.

- User can replay game in both directions (forward and backward)
- From start or from the game end. 
- Choose between Normal Mode and Step by Step Mode:
- - In Normal Mode the game is replayed continuously frame after frame.
- - In Step by Step Mode the game is replayed by several steps at a time with a set interval between steps.
- User can also make just one step forward or backward without switching to Step by Step Mode.
- Replay can be paused anytime.

Sources of sprite images:
- object/key.png, object/gate.png - https://stock.adobe.com/
- other sprites - https://github.com/blueskyson/Qt-pac-man

Bibliography:
- QT documentation - https://doc.qt.io/
