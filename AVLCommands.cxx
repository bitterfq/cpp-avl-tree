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

// thought this function was really clever, but its kinda useless for cases of 1000's.
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

  //  std::string key = keytostring(x);
  for (auto itr = obj.begin(); i < opr; itr++) {
    string opnum = itr.key();
    string operation = obj[opnum]["operation"];
    // cout << operation << ", " << keytobeinserted << endl;
    if (operation == "Insert") {
      int keytobeinserted = obj[itr.key()]["key"].get<int>();
      tree.Insert(keytobeinserted);
    }
    i++;
  }

  tree.flipheight();
  cout << tree.JSON();

}