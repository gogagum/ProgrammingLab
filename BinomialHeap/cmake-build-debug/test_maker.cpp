//
// Created by gogagum on 26.11.18.
//

#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::string;
using std::fstream;
using std::rand;
using std::sort;

string int_to_str(unsigned int num){
  string ret = "";
  if (num == 0) {
    return "0";
  }
  while (num != 0) {
    ret = (char)('0' + num % 10) + ret;
    num /= 10;
  }
  return ret;
}

bool comp(int a1, int a2) {
  return a1 > a2;
}

void make_extract_root_test(int size_of_test, string test_file_name) {
  fstream test_file;
  test_file.open(test_file_name, std::fstream::in | std::fstream::out | std::fstream::app);
  test_file << size_of_test << '\n';
  
  std::vector<int> inserted_elements;
  
  for (int i = 0; i < size_of_test; i++ ) {
    int type_of_test = rand() % 2;
    if ( type_of_test == 0 || inserted_elements.empty() ) {
      int inserted_element = rand();
      test_file << "0 " << inserted_element << '\n';
      inserted_elements.push_back(inserted_element);
    }
    else {
      sort(inserted_elements.begin(), inserted_elements.end(), comp);
      test_file << "1 " << *(inserted_elements.rbegin()) << '\n';
      inserted_elements.pop_back();
    }
  }
  
  test_file.close();
}

void make_insert_test(int size_ot_test, string test_file_name) {
  fstream test_file;
  test_file.open(test_file_name, std::fstream::in | std::fstream::out | std::fstream::app);
  test_file << size_ot_test << '\n';
  int min = rand();
  
  if (size_ot_test > 0) {
    test_file << min << ' ' << min << '\n';
  }
  for (int i = 1; i < size_ot_test; ++i) {
    int new_element = rand();
    test_file << new_element << ' ';
    if (new_element < min) {
      min = new_element;
    }
    test_file << min << '\n';
  }
  test_file.close();
};

void make_insert_tests() {
  for (int i = 1; i <= 20; i++) {
    make_insert_test(1 << i, "insert_tests/test_" + int_to_str(i) + ".txt");
  }
}

void make_extract_root_tests() {
  for (int i = 1; i <= 20; i++) {
    make_extract_root_test(1 << i, "extract_root_tests/test_" + int_to_str(i) + ".txt");
  }
}

int main() {
  make_insert_tests();
  make_extract_root_tests();
  return 0;
}

