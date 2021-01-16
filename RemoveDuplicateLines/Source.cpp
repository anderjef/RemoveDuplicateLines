//This program was developed starting July 14, 2020, to parse some number of (ASCII) files for duplicate lines and create numbered output files with the
//duplicate lines removed. Program developed for a Windows system.

//future considerations: optimize performance (parallel handling of files), be able to change the output file type

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream> //stringstream
using namespace std;

void printHelp() {
	cout << "The program can either take its arguments from the command line or from standard input.\n\n";
	cout << "Accepted inputs:\n\tany list of file names with or without their associated path\n\n";
	cout << "Accepted options:\n\t-h\t= displays help\n\t-r\t= changes the line delimiter in subsequent files to carriage return (instead of the default newline character)\n";
}

void handleFile(string inputFileName, char delimiter, unsigned int outputNum) {
	ifstream fileIN(inputFileName, ios::binary);
	if (!fileIN.is_open()) { cerr << "Fatal Error: could not open " << inputFileName << ".\n"; printHelp(); return; }

	const string intermediaryOutputFileString = (outputNum > 0 ? " (" + to_string(outputNum) + ')' : ""); //does not discriminate based on file extension
	const size_t dotIndex = inputFileName.find_last_of(".");
	const string fileExtension = (dotIndex == string::npos ? "" : inputFileName.substr(dotIndex)); //use same file extension as the corresponding input file
	ofstream fileOUT("output" + intermediaryOutputFileString + fileExtension);
	if (!fileOUT.is_open()) { cerr << "Fatal Error: could not open output" << intermediaryOutputFileString << fileExtension << " for writing.\n"; fileIN.close(); return; }

	unordered_map<string, vector<string>> hashmap;
	string line;
	unsigned int duplicatesCount = 0;
	while (!fileIN.eof()) {
		getline(fileIN, line, delimiter);
		if (hashmap.find(line) != hashmap.end()) { //if hashmap has an entry at the index
			bool foundDuplicate = false;
			for (size_t i = 0; i < hashmap[line].size(); ++i) {
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
	cout << "Successfully removed " << duplicatesCount << " duplicate lines in " << inputFileName << " and wrote the result to output" << intermediaryOutputFileString << fileExtension << " (which is in the working directory).\a\n";
}

int main(int argc, char* argv[]) {
	unsigned int fileCount = 0;
	bool haveShownHelp = false;
	char delimiter = '\n';
	if (argc == 1) {
		cout << "List the input file(s) and any flags (\"-h\" for help): ";
		string gotLine;
		getline(cin, gotLine);
		if (gotLine.size() == 0) { cout << "Successfully removed 0 duplicate lines in no files and did not write any results to any output file.";  return 0; }
		stringstream ss(gotLine);
		string arg;
		while (ss >> arg) {
			if (arg == "-r") {
				delimiter = '\r';
			}
			else if (arg == "-h" && !haveShownHelp) {
				printHelp();
				haveShownHelp = true;
			}
			else {
				handleFile(arg, delimiter, fileCount);
				fileCount++;
			}
		}
	}
	else {
		for (int i = 1; i < argc; ++i) {
			if (argv[i] == "-r") {
				delimiter = '\r';
			}
			else if (argv[i] == "-h" && !haveShownHelp) {
				printHelp();
				haveShownHelp = true;
			}
			else {
				handleFile(argv[i], delimiter, fileCount);
				fileCount++;
			}
		}
	}

	return 0;
}