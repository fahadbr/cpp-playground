#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

auto do_stuff() -> void;
auto modify_vector_ref(vector<int>& v, int index, int val) -> void;
auto modify_vector_val(vector<int> v, int index, int val) -> void;
auto print_vector(const vector<int>& v) -> void;

int main(void) {
  try {
    do_stuff();
  } catch (exception& ex) {
    cerr << ex.what();
  } catch (...) {
    cerr << "caught unknown exception.. exiting";
  }

  return 0;
}

auto do_stuff() -> void {
  vector<int> v{4, 5, 6, 7, 8};

  // modifying a vector by value doesnt work
  // if we pass by value, the entire vector is copied into the function
  // parameter despite the undelying data being allocated on the heap
  cout << "modifying vector by val\n";
  modify_vector_val(v, 2, 100);

  cout << "modifying vector by ref\n";  // this will
  modify_vector_ref(v, 3, 100);         // modify the original vector

  cout << "final vector value\n";
  print_vector(v);
}

auto modify_vector_ref(vector<int>& v, int index, int val) -> void {
  v[index] = val;
  print_vector(v);
}

auto modify_vector_val(vector<int> v, int index, int val) -> void {
  v[index] = val;
  print_vector(v);
}

auto print_vector(const vector<int>& v) -> void {
  for (int i = 0; i < v.size(); ++i) {
    cout << "v[" << i << "] = " << v[i] << ", ";
  }
  cout << endl;
}
