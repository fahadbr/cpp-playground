#include <iostream>
#include <string>

using namespace std;

void do_stuff();
void increment(uint16_t& val, uint8_t count);

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

void do_stuff() {
  uint16_t something{0};
  increment(something, 10);
  cout << "x is now: " << something << "\n";
}

void increment(uint16_t& val, uint8_t count) {
  if (count == 0) {
    return;
  }
  ++val;
  increment(val, --count);
}
