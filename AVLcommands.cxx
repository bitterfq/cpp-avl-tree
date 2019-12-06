//
// Created by aman842 on 11/27/19.
//
#include "json.hpp"
#include "AVL.h"
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;

string keytostring(int key) {
  if (key < 10) {
    std::string stringkey = "0" + to_string(key);
    return stringkey;
  } else {
    std::string stringkey = to_string(key);
    return stringkey;
  }
}

int main(int argc, char **argv) {
  string file = argv[1];
  if (argc < 2) {
    cerr << "Usage: AVLcommands.exe file.json" << endl;
    exit(0);
  }

  std::ifstream fileIn(file);
  if (!fileIn.is_open()) {
    cerr << "Error: cannot open file " << file << endl;
    exit(0);
  }
  json obj;
  fileIn >> obj;
  int opr = obj["metadata"]["numOps"];
  int i = 0;

  AVL tree;
  for (int x = 1; x <= opr; x++) {
    std::string key = keytostring(x);
    for (auto itr = obj[key].begin(); itr != obj[key].end(); ++itr) {
      int keytobeinserted = *itr;
      ++itr;
      string operation = *itr;
      cout << operation << ", " << keytobeinserted << endl;
      if (operation == "Insert") {
        tree.Insert(keytobeinserted);
      }
    }
  }
  cin.get();
}