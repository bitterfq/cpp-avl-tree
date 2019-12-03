//
// Created by aman842 on 11/27/19.
//
#include <vector>
#include "json.hpp"
#include <iostream>
#include "AVL.h"
#include <fstream>

using json = nlohmann::json;
using namespace std;
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
  for (auto itr = obj.begin(); i <= opr - 1; ++itr) {
    for (auto autoitr = obj[itr.key()].begin(); autoitr != obj[itr.key()].end(); autoitr++) {

      /*auto keyitr = (*autoitr).begin();
      int key = *keyitr;
      cout << key << endl;
      ++keyitr;
      auto operation  = (*autoitr).end();
      string opr = *operation;
      cout << opr << endl;*/
      i++;
    }
  }
}
