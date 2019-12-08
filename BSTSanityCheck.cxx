#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>

#include "BST.h"
#include "AVL.h"

#define SAMPLE_SIZE 1000
#define NUM_TESTS 10000

int main() {

	std::mt19937_64 rng(time(0));
	// Create uniform distribution
	std::uniform_int_distribution<int> unif(
		std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
	std::uniform_int_distribution<int> op(0,10);

	std::vector<int> sampleData, AVLSortedData;
	sampleData.reserve(SAMPLE_SIZE);
	AVLSortedData.reserve(SAMPLE_SIZE);
	std::cout << "Running tests..." << std::flush;

	for (unsigned int sample = 0; sample < NUM_TESTS; sample++) {
		AVL Tree;
		// On size_t usage here: https://stackoverflow.com/questions/131803/unsigned-int-vs-size-t
		for (size_t i = 0; i < SAMPLE_SIZE; i++) {
//            std::cout <<i;

            if (op(rng) == 0 && !Tree.empty()) {
//                std::cout << "-1";
				Tree.Delete(sampleData.back());     // delete doesnt work well because we were not supposed to implement it but we tried regardless.
//                std::cout << "-1" << std::endl;

                sampleData.pop_back();
			} else {
//                std::cout << "-2" << std::endl;
                // Add random integer to array
				int x = unif(rng);
				Tree.Insert(x);
				sampleData.push_back(x);
			}
		}

        while (!Tree.empty()) {
            AVLSortedData.push_back((Tree.DeleteMin()));
        }

		std::sort(sampleData.begin(), sampleData.end());
		assert(sampleData == AVLSortedData);

        while (!Tree.empty()) {
            // 1 check for balance factor of all nodes
            assert(Tree.check_balance(sampleData.back()) == 1);

            // 2 check for height of all nodes
            assert(Tree.check_height(sampleData.back()) == 1);

            sampleData.pop_back();
        }




		AVLSortedData.clear();
		sampleData.clear();
		if (sample % (NUM_TESTS / 10) == 0) {
			std::cout << "." << std::flush;
		}
	}


	std::cout << "Tests complete.\n";

}