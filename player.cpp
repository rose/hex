#ifndef _PLAYER_H
#define _PLAYER_H

#include <string>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include "board.cpp"
#include "boardsim.cpp"

using namespace std;

class Player {
  public:
    virtual ~Player() {}
    virtual string getName() { return name; }
    virtual pair<int,int> makeMove() = 0; // game requests move from player
    virtual void notify(int row, int col) {}; // game notifies player of opponent's move
    //virtual bool decideSwitch(int row, int col); // decide whether to switch after first move
    virtual void setName() = 0;
    virtual void setOpponent(string name) {
      opponentName = name;
    }
    virtual void resign() = 0;

  protected:
    string name;
    string opponentName;
};


class Human:public Player {
  public:
    Human() {
      setName();
    }

    void setName() {
      cout << endl << "Hi!  Before we start, what's your name?  ";
      cin >> name;
      cout << "Nice to meet you, " << name << "!" << endl;
    }

    void setOpponent(string name) {
      opponentName = name;
      cout << "Your opponent is " << opponentName << endl;
    }

    pair<int, int> makeMove() {
      int row, col;
      cout << name << ", please enter your move as row col " 
          << "(upper left is 0 0), or -1 0 to resign:  ";
      cin >> row >> col;
      return pair<int, int>(row, col);
    }

    void resign() {
      cout << "'Tis better to have played and lost than never "
          << "to have played at all.  Well done." << endl;
    }

    // this isn't so useful now, but if the game was extended
    // to allow network play it could do more.  Probably it would
    // want to take a pointer to the board as an argument so it
    // could print it out
    void notify(int row, int col) {
      cout << opponentName << " has played " << row << " " << col << endl;
    }
};


class AI:public Player {
  public:
    AI(int inSize) {
      setName();
      size = inSize;
      simCount = 10000;
      makeSims();
      makePlayed();
      makeValues();
    }

    ~AI() {
      for (auto sim: sims) {
        delete sim;
      }
    }

    void setName() {
      int foo = rand() % 5;
      switch (foo) {
        case 0 : name = "Your Robotic Overlord"; break;
        case 1 : name = "Hot Rivets"; break;
        case 2 : name = "Bitsy"; break;
        case 3 : name = "The Calculerminator"; break;
        case 4 : name = "Hal"; break;
        default : name = "Does Not Compute";
      }
    }

/*    void showSims() { // used for debugging, set simCount < 10
      for (auto sim: sims) {
        cout << *sim << endl;
        cout << "The winner is: " << (*sim).winner << endl;
      }
      showVals();
    }

    void showVals() {
      cout << "Values calculated: " << endl;
      for (auto row: totalValues) {
        for (auto val: row) {
          cout << val << " ";
        }
        cout << endl;
      }
      cout << "My chance of winning is: " << winCount 
          << " out of " << simCount << endl;
    }
*/
    void resign() {
      cout << "Stupid random number generator, always letting me down." << endl;
    }

    void updateValues() {
      // no optimization here; loops through every cell of
      // every values vector for every sim, every time

      // reset all values to 0
      vector< vector<int> > nextVals;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          totalValues[i][j] = 0;
        }
      }

      // for each simulation, increase the value of an unplayed
      // cell that belonged to the winner of that simulation
      winCount = 0;
      for (auto sim:sims) {
        if ((*sim).winner == ME) winCount++;
        nextVals = *(*sim).getValues();
        for (int i = 0; i < size; i++) {
          for (int j = 0; j < size; j++) {
            if (!played[i][j]) {
              totalValues[i][j] += nextVals[i][j];
            }
          }
        }
      }
    }

    pair<int,int> maxMove() {
      int bestRow, bestCol, thisVal, bestVal = 0;
      for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
          thisVal = totalValues[row][col];
          // take the new value if it's better; if equal, flip a coin
          // this does not provide perfectly random selection between 
          // 3 or more cells with exactly the same value - it favours 
          // the lower right.  Shouldn't affect quality of play.
          if (thisVal > bestVal
              || ((thisVal == bestVal)
                  && (rand() & 1))) {
            bestVal = thisVal;
            bestRow = row;
            bestCol = col;
          }
        }
      }
      return pair<int,int>(bestRow, bestCol);
    }

    pair<int,int> makeMove() {
      if (winCount * 3 < simCount) return pair<int,int>(-1, -1);
      pair<int,int> move = maxMove();
      int row = move.first;
      int col = move.second;
      for (auto sim:sims) {
        (*sim).recordMove(1,row, col);
      }
      played[row][col] = true;
      return move;
    }

    void notify(int row, int col) {
      for (auto sim:sims) {
        (*sim).recordMove(0,row, col);
      }
      played[row][col] = true;
      updateValues();
    }

    void makeSims() {
      sims.resize(simCount);
      for (int i = 0; i < simCount; i++) {
        sims[i] = new BoardSim(size);
      }
    }

    void makePlayed() {
      played.resize(size);
      for (int i = 0; i < size; i++) {
        played[i].resize(size);
      }
    }

    void makeValues() {
      totalValues.resize(size);
      for (int i = 0; i < size; i++) {
        totalValues[i].resize(size);
      }
      updateValues();
    }

  private:
    int winCount;
    vector<vector<int> > totalValues;
    vector<vector<int> > played;
    int simCount;
    int size;
    vector<BoardSim*> sims;
    void updateSims(int move);
};


#ifdef TESTPLAYER
int main() {

}
#endif // TESTPLAYER

#endif // _PLAYER_H
