#ifndef _HEX_H
#define _HEX_H

#include <iostream>

#include "gameboard.cpp"
#include "player.cpp"

using namespace std;

class Hex
{
  public:
    Hex(int size = 11) {
      resign = false;
      board = new GameBoard(size);
      players.resize(2);
      players[0] = new Human();
      cout << "Generating AI (this may take a while for larger boards)" << endl;
      players[1] = new AI(size);
      introducePlayers(players[0], players[1]);
      cout << "Who should go first?  Enter 0 for " 
          << players[1]->getName() << ", 1 for you:  ";
      cin >> playerID;
      currentPlayer = players[playerID];
      otherPlayer = players[!playerID];
    }

    ~Hex() {
      delete board;
      delete players[0];
      delete players[1];
    }

    Player* runMatch() {
      while (!board->checkWin()) {
        nextPlayer();
        refresh();
        getMove();
        if (resign) {
          currentPlayer->resign();
          cout << currentPlayer->getName() << " has resigned." << endl;
          return otherPlayer;
        }
      }

      refresh();
      return currentPlayer; 
    }

    void nextPlayer() {
      otherPlayer = currentPlayer;
      playerID = !playerID;
      currentPlayer = players[playerID];
    }

    void getMove() {
      bool moveRecorded = false;
      bool badMove = false;
      int row, column = 0;
      while (!moveRecorded) {
        pair<int,int> foo = currentPlayer->makeMove();
        row = foo.first;
        column = foo.second;
        if (row < 0 || column < 0) {
          resign = true;
          return;
        }
        moveRecorded = board->tryMove(playerID, row, column);
        if (!moveRecorded) {
          if (badMove) {
            resign = true;
            return;
          } else {
            cout << " ** " << currentPlayer->getName() 
                << " has tried to make an illegal move! (" 
                << row << " " << column << ") ** " << endl;
            badMove = true;
          }
        }
      }
      otherPlayer->notify(row, column);
    }

    void introducePlayers(Player* first, Player* second) {
      first->setOpponent(second->getName());
      second->setOpponent(first->getName());
      cout << endl << "Now for the grudge match of the century " 
          << "between " << first->getName() 
          << " and " << second->getName() << "!" << endl << endl;
    }

    void refresh() {
      if (playerID == 0) { // player is human
      cout << (*board) << endl;
      }
    } // redraws the board, later might clear terminal screen, add messages, etc

  private:
    bool resign;
    vector<Player*> players;
    Player* currentPlayer;
    Player* otherPlayer;
    int playerID;
    string message;
    GameBoard* board;
};

#endif  // _HEX_H
