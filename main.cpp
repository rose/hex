#include <iostream>

#include "hex.cpp"

int main(int argc, char* argv[])
{
  srand(time(0));
  int size;
  cout << endl << "Board size (not over 19): ";
  cin >> size;
  Hex hex(size);
  Player* winner = hex.runMatch();
  cout << winner->getName() << " has soundly thrashed his her or its opponent!  Huzzah!" << endl;

  return 0;
}
