//This program was developed starting July 14, 2020, to parse a(n ASCII) file for duplicate lines and create an output.csv with them removed. Program
//developed for a Windows system.

//future considerations: handle multiple input files, optimize performance, be able to change the output file type

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

int main(int argc, char* argv[]) {
	string inputFilename;
	if (argc == 1) {
		cout << "Please name the input file (including path): ";
		getline(cin, inputFilename);
	}
	else {
		inputFilename = argv[1];
	}
	ifstream fileIN(inputFilename, ios::binary);
	if (!fileIN.is_open()) { cerr << "Fatal Error: could not open " << inputFilename << ".\n"; return 1; }
	ofstream fileOUT("output.csv");
	if (!fileOUT.is_open()) { cerr << "Fatal Error: could not open output.csv for writing.\n"; fileIN.close(); return 2; }

	unordered_map<string, vector<string>> hashmap;
	string line;
	unsigned int duplicatesCount = 0;
	while (!fileIN.eof()) {
		getline(fileIN, line);
		if (hashmap.find(line) != hashmap.end()) { //if hashmap has an entry at the index
			bool foundDuplicate = false;
			for (int i = 0; i < hashmap[line].size(); ++i) {
				if (hashmap[line].at(i) == line) { //ensuring that the hashmap at the index is indeed the same line
					duplicatesCount++;
					foundDuplicate = true;
					break;
				}
			}
			if (!foundDuplicate) {
				hashmap[line].push_back(line);
				fileOUT << line;
			}
		}
		else { //line was not found in the hashmap
			hashmap[line].push_back(line);
			fileOUT << line;
		}
	}

	fileIN.close();
	fileOUT.close();
	cout << "Successfully removed " << duplicatesCount << " duplicate lines and wrote the result to output.csv (which is in the working directory).\a\n";
	return 0;
}