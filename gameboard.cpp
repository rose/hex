#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include "board.cpp"

const int UNPLAYED = 2;
using namespace std;

class GameBoard:public Board {
  public:
    GameBoard(int sizeIn):Board(sizeIn) {
      fill();
      makeUfinds();
    }

    ~GameBoard() {
      delete ufind[0];
      delete ufind[1];
    }

    void fill() {
      for (int i = 0; i < size; i++) {
        board[i].resize(size);
        for (int j = 0; j < size; j++) {
          board[i][j] = UNPLAYED;
        }
      }
    }

    bool tryMove(int player, int row, int col) {
      if (inBounds(row) && inBounds(col)) {
        if (unoccupied(row, col)) {
          recordMove(player, row, col);
          return true;
        }
      }
      return false;
    }

    bool checkWin() { 
      if (ufind[0]->sameGroup(north, south)) return true;
      if (ufind[1]->sameGroup(east, west)) return true;
      return false;
    }

    void recordMove(int player, int row, int col) {
      board[row][col] = player;
      mergeNeighbours(player, row, col);
    } 

    bool unoccupied(int row, int col) {
      return board[row][col] == UNPLAYED;
    } 

    // the next two functions were a lot prettier in my mind
    void tryMerge(int player, int move1, int move2) {
      int row1 = move1 / size;
      int col1 = move1 % size;
      int row2 = move2 / size;
      int col2 = move2 % size;

      if (board[row1][col1] == player 
          && board[row2][col2] == player) {
        ufind[player]->merge(move1, move2);
      }
    }

    void mergeNeighbours(int player, int row, int col) {
      int move = moveIndex(row, col);
      int nw = moveIndex(row-1, col);
      int ne = moveIndex(row-1, col+1);
      int w = moveIndex(row, col-1);
      int e = moveIndex(row, col+1);
      int sw = moveIndex(row+1, col-1);
      int se = moveIndex(row+1, col);

      if (inBounds(col-1)) tryMerge(player, move,w);
      if (inBounds(col+1)) tryMerge(player, move,e);
      if (inBounds(row - 1)) {
        tryMerge(player, move, nw);
        if (inBounds(col + 1)) {
          tryMerge(player, move, ne);
        }
      }
      if (inBounds(row + 1)) {
        tryMerge(player, move, se);
        if (inBounds(col - 1)) {
          tryMerge(player, move, sw);
        }
      }
    }

    void makeUfinds() {
      // used by checkwin; ufind[0] keeps track of what nodes are
      // connected to the north & south edges by player 1's moves
      // ufind[1] does the same for player 2 with east & west

      north = size * size;
      south = north + 1;
      east = north + 2;
      west = north + 3;

      ufind[0] = new Ufind(size * size + 4);
      int nextHexOnNorth = 0; // start in upper left, move right
      int nextHexOnSouth = size * (size - 1); // start in lower left, move right

      ufind[1] = new Ufind(size * size + 4);
      int nextHexOnEast = size - 1; // start in upper right, move down
      int nextHexOnWest = 0; // start in upper left, move down

      for (int i = 0; i < size; i++) {
        ufind[0]->merge(north, nextHexOnNorth);
        nextHexOnNorth += 1; // moving right :)
        ufind[0]->merge(south, nextHexOnSouth);
        nextHexOnSouth += 1; 
        ufind[1]->merge(east, nextHexOnEast);
        nextHexOnEast += size; // moving down to next row
        ufind[1]->merge(west, nextHexOnWest);
        nextHexOnWest += size; 
      }
    }

    char cell(int row, int col) {
      switch (board[row][col]) {
        case 0: return 'O'; break;
        case 1: return 'X'; break;
        default: return '.'; break;
      }
    }

    friend ostream& operator<<(ostream& out, GameBoard& b)
    {
      out << endl;

      for (int i = 0; i < b.size; i++ ) {
        cout << i << "   ";
      }

      out << endl << endl;

      for (int i = 0; i < b.size; i++) {
        // each line is indented a little further than the last
        for (int j = 0; j < 2 * (i+1); j++) {
          out << " ";
        }
      
        // display the cells and their connections
        for (int j = 0; j < b.size; j++) {
          out << b.cell(i,j) << "   ";
        }
        out << i << endl;

        // connections between rows
        if (i != b.size - 1) {
          out << endl;
        }
      }
      return out;
    }

  protected:
    Ufind *ufind[2];
};

#endif // _GAMEBOARD_H
