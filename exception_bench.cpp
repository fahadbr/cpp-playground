#include <iostream>
#include <string>
#include <regex>
#include <boost/regex/v4/regex.hpp>

using namespace std;

void except();
void noex();

int main(int argc, char *argv[]) {
  try {
    auto f = except;
    if (argc != 2) {
      cerr << "need exactly 1 argument which is an integer. provided " << argc - 1 << endl;
    }

    int test_case {atoi(argv[1])};
    switch (test_case) {
      case 1:
        f = except;
        break;
      case 2:
        f = noex;
        break;
      default:
        cerr << "unsupported test case: " << test_case << endl;
        return 1;
    }

    for (int i = 0; i < 1000000; i++) {
      f();
    }
  } catch (exception &ex) {
    cerr << ex.what();
  } catch (...) {
    cerr << "caught unknown exception.. exiting" << endl;
  }

  return 0;
}

void HandleError(const char* err) {
  //cout << "handling error: " << err;
}

void except_call() {
  throw std::runtime_error("some error");
}


void except() {
  try {
    except_call();
  } catch (std::runtime_error &e) {
    HandleError(e.what());
  }
}

struct ErrorWrapper {
  const char* error;
  bool val;
};

ErrorWrapper noex_call() {
  return ErrorWrapper{"some error", false};
}

void noex() {
  auto ret = noex_call();
  if (!ret.val) {
    HandleError(ret.error);
  }

}
