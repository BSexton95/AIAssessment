# AI Assessment

This is a sample C++ project setup with [raylib][raylib] for Visual
Studio 2019. Raylib is a simple game programming framework that is designed to
be friendly to beginners. It is created by [Ramon Santamaria
(@raysan5)][raysan].

This version is compatible with any documentation referring to Raylib 3.0.

It is primarily intended for use by students in the Game Programming course at
the Seattle Campus of the Academy of Interactive Entertainment.

[raylib]:https://github.com/raysan5/raylib
[raysan]:https://github.com/raysan5

## How to get the game
- Download the latest version
- Unzip the file
- Double click raygame.exe to play

## How to play
The goal of this game is to demonstrate AI steering behaviors and path finding. There are three enemy ships in the game to demonstrate AI.
The enemy ship on the left side demonstrates both wandering behaviour and fleeing behaviour. When the player ship collides with this ship the game
will close indicating you have won the game. The enemy ship on the right demonstrates both wandering and seeking behaviour. When the player ship 
collides with this ship the player ship will respawn at the bottom of the map. The enemy ship in the top middle of the map demonstrates AI path
finding. The player ship will respawn at the bottom of the map when colliding with this ship as well.

### Controls
| Key | Description |
| ----|-------------|
| W | Move Up |
| A | Move Left |
| S | Move Down |
| D | Move Right |

## Known Bugs
The enemy ships can sometimes get stuck on the blue walls. Application must be restarted if demonstration needs to be seen.
