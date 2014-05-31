#include <iostream>
#include <vector>

using namespace std;

class Ufind {
  public:
    Ufind(int insize) {
      size = insize;
      parent = vector<int>(size);
      weight = vector<int>(size);
      reset();
    }

    void reset() {
      for (int i = 0; i < size; i++) {
        parent[i] = i;
        weight[i] = 1;
      }
    }

    bool sameGroup(int a, int b) {
      return leader(a) == leader(b);
    }

    void merge (int a, int b) {
      if (weight[a] > weight[b]) {
        merge_ (a,b);
      } else {
        merge_ (b,a);
      }
    }


    friend ostream& operator<<(ostream& out, Ufind u) {
      for (int i = 0; i < u.size; i++) {
        out << i << "\t" << u.parent[i] << "\t" << u.weight[i] << endl;
      }
      return out;
    }


  private:
    int size;
    vector<int> parent;
    vector<int> weight;

    void merge_ (int a, int b) { // members of b's group join a's
      int aLead = leader(a);
      int bLead = leader(b);
      weight[aLead] += weight[bLead];

      for (int i = 0; i < size; i++) {
        if (leader(i) == bLead) {
          parent[i] = aLead;
        }
      }
    }

    int leader(int a) {
      int cursor = a;
      int next = parent[cursor];
      while (cursor != next) {
        parent[cursor] = parent[next]; // path compression
        cursor = next;
        next = parent[cursor];
      }
      return cursor;
    }

};

#ifdef TESTUFIND
#include <cassert>
int main() {
  Ufind u(10);

  cout << "testing 3 & 4 start in different groups:";
  assert(!u.sameGroup(3,4));
  cout << "  ok!\n" << u;

  cout << "testing 3 & 4 are in same group after merge(3,4):";
  u.merge(3,4);
  assert(u.sameGroup(3,4));
  cout << "  ok!\n" << u;

  cout << "testing 7 & 4 are in same group after merge(7,3)";
  u.merge(7,3);
  assert(u.sameGroup(7,4));
  cout << "  ok!\n" << u;
  
  cout << "More merging!";
  u.merge(6,4);
  u.merge(8,2);
  assert(!u.sameGroup(4,8));
  u.merge(2,9);
  u.merge(7,9);
  assert(u.sameGroup(4,8));
  cout << "  ok!\n" << u;

  cout << "Testing reset:";
  u.reset();
  assert(!u.sameGroup(4,8));
  assert(!u.sameGroup(7,4));
  assert(u.sameGroup(3,3));
  cout << "  ok!\n" << u;
}

#endif
