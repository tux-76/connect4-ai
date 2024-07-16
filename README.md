# Connect-4 AI
C++ Artificial Intelligence Program for Playing Connect-4 Using the Minimax Algorithm

## Program overview
This algorithm searches the possible outcomes of any Connect-4 board and selects the best possible move for the current player. It generally searches to depth 8, but it optimizes the depth as the game progresses.

The user interface runs in the terminal. It is more focused on exploring the capabilities of the _Minimax_ algorithm rather than providing a clean and smooth user interface, although I plan to make a user-friendly program to play multiple turn-based games like this in the future.

## Install (Linux & MacOS)
To install on Linux or MacOS:
1. Be sure you have the `g++` compiler and that it supports C++11 or later. The `make` and `git` packages are also required.
2. Clone the repository. `git clone https://github.com/tux-76/Connect4-AI`
3. Make the file from the release directory. `make -C Release all`
4. Run the executable. `./Release/Connect4-AI`

**NOTE:** If you are on _Windows_ or this method doesn't work for you, try using an IDE or your own compiler to compile the source code (in the src folder), ignoring the makefile.

## Controls
Before each move you will be presented with the current Connect-4 board, with blank spaces being represented by a `-`, like so:

```
6 5 4 3 2 1 0
- - - o - - -
- - - x - - -
- - - o - - -
- - - x - - -
- - - o x - -
- - - x o x -
Next: o
```

**X** represents _Player 1_, and **O** represents _Player 2_

You may enter a move by selecting the number of the column you would like to "drop" the piece into. The column numbers are listed at the top. Please note that they are zero-indexed from right to left.

The AI can play for both X and O (so can the user). **You must press `Enter` _without_ entering a move for an AI selection.**
So if the user wants to play **O** against the AI as **X**, he will press `Enter` **without** a move every time it is **X**'s turn, and enter a move every time it is **O**'s turn.

## The algorithm behind the AI
The AI in this program is built around the **Minimax** algorithm. _Minimax_ is a type of algorithm that allows computers to determine the best possible move for any state of a turn-based game. It essentially scans the possible outcomes of any _game state_ and determines which outcome is most desirable and achievable, given that the other player is also playing optimally. For any _game state_ (the arrangement of pieces on the board), the computer first gets the possible plays of the next player. After it gets the likelyhood that Player 1 will win—known as the _state value_—for each of the child _game states_ (either by computing that the game is over or by running the Minimax algorithm recursively), it then selects either the _minimum_ or the _maximum_ of all those values based on who's turn it is (Player 1 wants the outcome with the _greatest_ value, wheras his opponent, Player 2, wants the most _negative_ value). This function will run itself again until it reaches the maximum depth, which is necessary because a full calculation would take too long. Additionally, this program performs a mathematical calculation after every process to optimize the depth so that the next calculation will take about 1 second to complete, maximizing the power of the AI when the game is further along. Thus, for any game state, the _Minimax_ algorithm determines the likelyhood that Player1 will win given both players play optimally. Selecting the best move, then, is as simple as running _Minimax_ for each possible move and returning the most desirable one.
