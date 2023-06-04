# Hexxagon game documentation

## HEXXAGONE GAME

This is C++ project implementing game logic, bot player and visual representation of hexxagon game.
It fully corresponds to game rules with 2 players and the main idea to full game board with yours pieces.
Project consist this classes:

@tableofcontents 

@section Board
Representing 17X9 size game board, with SquareVal variables for recognition type of square on this logical board based all gaming mechanics.
@section SquareVal
Enum class, it enums play role of type of the Board piece. @see Board
@section Player
Enum class that has 2 values for 2 of the players.

@section GameFormat
Enum class for setting the game format.
1. PVP - player vs player
2. PVAI - player vs AI @see AI
@section Game
Class that rules all game logic and has all methods for controlling game rules and win conditionals.
@section AI 
Class for crating bot that calculates most valuable moves to beat you in the game;)
@section StartWindow
clas that create graphical start menu window.
@section GraphicalBoard
GIU representation of logical board consist of choosing game mode menu then board of hexagons and labels with total score for players.