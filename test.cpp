#include <iostream>
using namespace std;

class Foo {
  public:
    Foo() { baz(); }
    virtual void baz() { cout << "hello world!"; }
};

class FooChild:public Foo {
  public:
    FooChild() { a = 5; }
    void baz() { cout << "hello world, a is " << a; }
    int a;
};

int main() {
  FooChild boopsie;
}
