#include <iostream>
#include <string>
#include <regex>
#include <boost/regex/v4/regex.hpp>

using namespace std;

bool func1(const string&);
bool func2(const string&);
bool func3(const string&);
bool func4(const string&);

int main(int argc, char *argv[]) {
  try {
    auto f = func1;
    if (argc != 2) {
      cerr << "need exactly 1 argument which is an integer. provided " << argc - 1 << endl;
    }

    int test_case {atoi(argv[1])};
    switch (test_case) {
      case 1:
        f = func1;
        break;
      case 2:
        f = func2;
        break;
      case 3:
        f = func3;
        break;
      case 4:
        f = func4;
        break;
      default:
        cerr << "unsupported test case: " << test_case << endl;
    }

    string date_str = "2020-06-30";
    for (int i = 0; i < 1000000; i++) {
      if (!f(date_str)) {
        cerr << "matching func failed to match" << endl;
        return 1;
      }
    }
  } catch (exception &ex) {
    cerr << ex.what();
  } catch (...) {
    cerr << "caught unknown exception.. exiting" << endl;
  }

  return 0;
}

bool func1(const string& date_str){
  if (date_str[0] >= '0' && date_str[0] <= '9' &&
    date_str[1] >= '0' && date_str[1] <= '9' &&
    date_str[2] >= '0' && date_str[2] <= '9' &&
    date_str[3] >= '0' && date_str[3] <= '9' &&
    date_str[4] == '-' &&
    date_str[5] >= '0' && date_str[5] <= '1' &&
    date_str[6] >= '0' && date_str[6] <= '9' &&
    date_str[7] == '-' &&
    date_str[8] >= '0' && date_str[8] <= '3' &&
    date_str[9] >= '0' && date_str[9] <= '9') {
    return true;
  }
  return false;
}

bool func2(const string& date_str){
  static const regex re{"[0-9]{4}-[0-9]{2}-[0-9]{2}", regex::optimize};
  return regex_match(date_str, re);
}

bool func3(const string& date_str){
  static const boost::regex re{"[0-9]{4}-[0-9]{2}-[0-9]{2}", boost::regex::optimize};
  return boost::regex_match(date_str, re);
}

bool func4(const string& date_str) {
  static const boost::regex re{"[0-9]{4}-[0-9]{2}-[0-9]{2}", boost::regex::optimize};
  return boost::regex_match(date_str.c_str(), re);
}
