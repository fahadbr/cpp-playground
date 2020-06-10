#include <iostream>
#include <string>

using namespace std;

void do_stuff();

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
  using ssize_t = string::size_type;

  const string s{"this is the full string"};
  const char delim = ' ';

  ssize_t start_pos = s.find_first_not_of(delim, 0);
  ssize_t delim_pos = s.find_first_of(delim, start_pos);

  while (start_pos != string::npos || delim_pos != string::npos) {
    const ssize_t end_pos{delim_pos == string::npos ? s.length() : delim_pos};
    //string_view sv{&s.at(start_pos), end_pos - start_pos};
    const char* sv = &s.at(start_pos);
     //string sv{&s.at(start_pos), end_pos - start_pos};
    // string sv = s.substr(start_pos, end_pos - start_pos);

    // cout << "string_view: " << sv << '\n';

    const string string_from_view{sv, end_pos - start_pos};
    cout << "resulting string: " << string_from_view << '\n';

    start_pos = s.find_first_not_of(delim, delim_pos);
    delim_pos = s.find_first_of(delim, start_pos);
    // cout << "start_pos: " << start_pos << ", delim_pos: " << delim_pos << endl;
  }
}
