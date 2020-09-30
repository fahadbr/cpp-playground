#include <iostream>
#include <string>

using namespace std;

auto do_stuff() -> void;

int main(void) {
  try {
    do_stuff();
  } catch (exception &ex) {
    cerr << ex.what();
  } catch (...) {
    cerr << "caught unknown exception.. exiting";
  }

  return 0;
}

// This class is testing out how a Class
// can be automatically converted into a string
// on assignment
class Stringer {
  public:
    string value;

    operator std::string () const {
      return value;
    }

};

auto do_stuff() -> void {
  string s1{"hello "};
  string s2;

  Stringer stringer{};
  stringer.value = "I'm a Stringer :)";

  s2 = stringer;


  cout << s1 << s2 << endl;
}
