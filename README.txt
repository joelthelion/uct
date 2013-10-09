COPYRIGHT NOTICE

    Copyright Pierre Gueth and Joel Schaerer, 2009

    uct is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uct is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uct.  If not, see <http://www.gnu.org/licenses/>.

SYNOPSIS

    This is a generic framework for implementing UCT-based AIs for two
    player games. It is based on the Upper Confidence bounds applied to Trees
    algorithm, as described here:

    http://hal.inria.fr/docs/00/12/15/16/PDF/RR-6062.pdf

    This algorithm is famous for having improved the strength of Go AIs by at
    least ten kyus, which is *a lot*. The algorithm is very general, and can
    be used to create decent AI for two-player games such as chess, checkers,
    go, othello, etc. All you need to do in order to implement a new game is 
    to write a class with a very minimal interface: with a given position,
    test for a win, give possible moves, play a random move and a few others.
    You can check the complete interface in board.h. Once the game knowledge
    is described in this class, UCT will do the hard part for you, ie. chosing
    the best move :) Even better, it will demonstrably play the best move,
    given infinite time.

BUILD INSTRUCTIONS

    Make sure you have CMake and Qt installed. Then:
    ccmake . && make

IMPLEMENTATION

    This library is divided in two pieces: the generic UCT algorithm, in the
    files uct.cpp and uct.h, which you don't need to change if you just want
    to implement a new game. The other files are either game descriptions or
    human interfaces (this version is shipped with sample implementations of
    connect 4 and othello, with a QT Gui for the connect 4 game).

    Adding a new game in a nutshell:
    - copy boardc4.h and boardc4.cpp
    - rename MoveC4 and BoardC4 to match the classes of your game.
    - write the implementation in the cpp file (this is the hardest step).
    - add the cpp file in the CMakeLists.txt.
    - add your game name in the Game enum in utils.h.
    - add parsing code in parse_game in utils.cpp.
    - ...
    - profit
