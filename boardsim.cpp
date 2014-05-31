#ifndef _BOARDSIM_H
#define _BOARDSIM_H

#include "board.cpp"
#include <ctime>
#include <cstdlib>

using namespace std;

class BoardSim:public Board {

#define ME 1
#define OPP 0

  public:

    BoardSim(int sizeIn):Board(sizeIn) {
      // make the board and fill it with random values
      // store information about AI's connected cells
      fill();
      myCells = new Ufind(size * size + 2);
      makeUfinds();
      calculateConnections();

      // see who won and value cells appropriately
      values.resize(size);
      for (int i = 0; i < size; i++) {
        values[i].resize(size);
      }
      updateWinner();
      setValues();
    }

    ~BoardSim() {
      delete myCells;
    }

    void fill() {
      for (int i = 0; i < size; i++) {
        board[i].resize(size);
        int someInt = rand();
        for (int j = 0; j < size; j++) {
          board[i][j] = (someInt >> j) & 1;
        }
      }
    }

    void recordMove(int player, int row, int col) {
      if (board[row][col] != player) {
        board[row][col] = player;
        if (player == ME) {
          mergeNeighbours(row, col);
          updateWinner();
        } else { // can't unmerge, need to recalculate from start
          myCells->reset();
          makeUfinds();
          calculateConnections();
          updateWinner();
        }
      }
    }

    void mergeNeighbours(int row, int col) {
      mergeSouthWest(row, col);
      mergeNorthEast(row, col);
    }

    void mergeNorthEast(int row, int col) {
      // merge a cell with its north, northeast, and east neighbours
      int move = moveIndex(row, col);
      int nw = moveIndex(row-1, col);
      int ne = moveIndex(row-1, col+1);
      int e = moveIndex(row, col+1);

      if (inBounds(col+1)) tryMerge(move,e);
      if (inBounds(row - 1)) {
        tryMerge(move, nw);
        if (inBounds(col + 1)) {
          tryMerge(move, ne);
        }
      }
    }

    void mergeSouthWest(int row, int col) {
      // merge a cell with its south, southwest, and west neighbours
      //
      // mergeSouthWest is used when constructing a ufind from scratch
      // for a full board.  It will be called on *every* cell I own
      // so we only check half the neighbours for each cell.
      //
      // for example, this cell doesn't need to check its northwest
      // neighbour, because it is that cell's southeast neighbour, so
      // the merge will happen on that cell's turn if it should.
      
      int move = moveIndex(row, col);
      int w = moveIndex(row, col-1);
      int sw = moveIndex(row+1, col-1);
      int se = moveIndex(row+1, col);

      if (inBounds(col-1)) tryMerge(move,w);
      if (inBounds(row + 1)) {
        tryMerge(move, se);
        if (inBounds(col - 1)) {
          tryMerge(move, sw);
        }
      }
    }

    void tryMerge(int move1, int move2) {
      // move1 is always ME
      int row2 = move2 / size;
      int col2 = move2 % size;

      if (board[row2][col2] == ME) {
        myCells->merge(move1, move2);
      }
    }

    void calculateConnections() {
      for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
          if (board[row][col] == ME) {
            mergeSouthWest(row, col);
          }
        }
      }
    }

    void makeUfinds() {
      // unlike Board, BoardSim only maintains one ufind
      // because it is always full, we have only to check
      // whether the AI has won.

      // imaginary east and west nodes that are connected to all
      // nodes on their respective sides
      east = size * size;
      west = east + 1;

      int nextEast = size - 1;
      int nextWest = 0;
      
      for (int i = 0; i < size; i++) {
        myCells->merge(east, nextEast);
        myCells->merge(west, nextWest);
        nextEast += size;
        nextWest += size;
      }
    }

    void updateWinner() {
      // hard coded that ai is playing e-w.  
      // would need to change to play ai-ai games
      int oldWinner = winner;
      winner = myCells->sameGroup(east,west);
      if (oldWinner != winner) setValues();
    }

    void setValues() {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          values[i][j] = 0;
          if (winner == board[i][j]) {
            values[i][j] = (winner==board[i][j]);
          }
        }
      }
    }

    vector<vector<int> >* getValues() { return &values; }

    bool value(int row, int col) {
      // return whether this square belongs to the winning player
      return (winner == board[row][col]);
    }

    int winner;
    vector<vector<int> > values;
  private:
    Ufind* myCells;
};

#ifdef TESTBSIM

int main() {
  srand(time(0));
  for (int i = 0; i < 5; i++) {
    BoardSim foo(7);
    cout << foo << endl;
    cout << "Da winnah! " << foo.winner << endl;
    cout << "Values: " << endl;
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        cout << foo.values[i][j] << " ";
      }
      cout << endl;
    }
  }

  for (int i = 0; i < 4; i++) {
    cout << endl << endl << "******************" << endl;
    BoardSim baz(5);
    cout << baz << endl;
    cout << "Winner is " << baz.winner << endl;
    int loser = !baz.winner;
    while (loser != baz.winner) {
      int row = rand() % 5;
      int col = rand() % 5;
      baz.recordMove(loser, row, col);
      cout << "giving loser " << row << " " << col << endl;
    }
    cout << baz << endl;
    cout << "Winner is " << baz.winner << endl;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        cout << baz.values[i][j] << " ";
      }
      cout << endl;
    }
  }
}
#endif // TESTBSIM
#endif // _BOARDSIM_H
