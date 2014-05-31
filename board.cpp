#ifndef _BOARD_H
#define _BOARD_H

#include <iostream>
#include <vector>
#include "ufind.cpp"
#include <tuple>

using namespace std;
class Board {
  public:
    Board(int sizeIn) {
      size = sizeIn;
      board.resize(size);
    }

    //virtual void recordMove(int player, pair<int,int> move) = 0;
    virtual ~Board() {}

  protected:
    virtual void fill() = 0;
    bool inBounds(int n) { return (n >= 0 && n < size); }
    int moveIndex(int row, int col) { return row * size + col; }
    int north, south, east, west;
    int size;
    vector<vector<int> > board;
};

#endif //_BOARD_H
