//Jeffrey Andersen


//future considerations: optimize (parallel handling of files), be able to change the output file type, allow for specifing which duplicate line to keep
//(as opposed to the current always keeping the first)


#include <algorithm> //count and max
#include <fstream>
#include <iomanip> //setw
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility> //pair and make_pair
#include <vector>
using std::cerr;
using std::cin;
using std::count;
using std::cout;
using std::getline;
using std::ifstream;
using std::ios_base;
using std::istringstream;
using std::make_pair;
using std::max;
using std::ofstream;
using std::pair;
using std::setw;
using std::streamsize;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;


void printHelp() {
	cout << "The program can either take its arguments from the command line or from standard input.\n\n";
	cout << "Accepted inputs:\n\tany list of file names with their relative or absolute path\n\n";
	cout << "Accepted options:\n";
	cout << "\t-h\tdisplays help\n";
	cout << "\t-i\ttreats the next argument as if it were a file name (rather than as any option flag)\n";
	cout << "\t-n\tmakes the line delimiter in subsequent files the newline character (which is the default behavior)\n";
	cout << "\t-r\tchanges the line delimiter in subsequent files to carriage return (instead of the default newline character)\n";
	cout << "\t-v\ttoggles whether to additionally print (to standard output) the duplicate lines found for subsequent files and how many of each were removed (verbose mode is off by default)\n"; //future consideration: also print the line number (in the input file) for each duplicate line in verbose mode
}


int handleFile(const string& inputFileName, const unsigned int fileNum, const char delimiter = '\n', const bool isVerboseModeOn = false) { //returns zero on success and some other integer on failure
	ifstream fileIn(inputFileName, ios_base::in | ios_base::binary, _SH_DENYWR);
	if (!fileIn.is_open()) { cerr << "Fatal Error: could not open " << inputFileName << ".\n"; printHelp(); return 1; }

	const string intermediaryOutputFileString = (fileNum > 0 ? " (" + to_string(fileNum) + ')' : ""); //does not discriminate based on file extension
	const string fileExtension = inputFileName.substr(inputFileName.find_last_of(".")); //use same file extension as the corresponding input file
	ofstream fileOut("output" + intermediaryOutputFileString + fileExtension, ios_base::out, _SH_DENYWR);
	if (!fileOut.is_open()) { cerr << "Fatal Error: could not open \"output" << intermediaryOutputFileString << fileExtension << "\" for writing.\n"; fileIn.close(); return 1; }

	unordered_map<string, pair<vector<string>, unsigned int>> duplicateLinesMap; //maps a line of the file to all different lines sharing the same hash and how many lines have hashed to that location //future consideration: review whether the vector of strings being mapped to is really necessary (that is, confirming each mapped line matches what indexed to the location before is truly necessary) as I have assumed so based on long ago coming across two values appearing to map to the same place (actually I believe for the original/lost version of this program)
	unsigned int numDuplicatesLines = 0;
	unsigned int modeCount = 1;
	string line;
	for (getline(fileIn, line, delimiter); !fileIn.eof(); getline(fileIn, line, delimiter)) {
		if (duplicateLinesMap.find(line) != duplicateLinesMap.end()) { //if hashmap has an entry at the index
			bool isADuplicateLine = false; //to be corrected as necessary
			for (size_t i = 0; i < duplicateLinesMap.find(line)->second.first.size(); i++) {
				if (duplicateLinesMap.find(line)->second.first.at(i) == line) { //ensuring that the hashmap at the index is indeed the same line
					duplicateLinesMap.find(line)->second.second++;
					modeCount = max(modeCount, duplicateLinesMap.find(line)->second.second);
					numDuplicatesLines++;
					isADuplicateLine = true;
					break;
				}
			}
			if (!isADuplicateLine) { //line was not found in the hashmap
				cerr << "Warning: " << duplicateLinesMap.find(line)->second.first.at(0) << " and " << line << " were found to map to the same value.\n";
				duplicateLinesMap[line] = make_pair(vector<string>(), 1);
				duplicateLinesMap.find(line)->second.first.push_back(line);
				fileOut << line;
			}
		}
		else { //line was not found in the hashmap
			duplicateLinesMap[line] = make_pair(vector<string>(), 1);
			duplicateLinesMap.find(line)->second.first.push_back(line);
			fileOut << line;
		}
	}
	fileIn.close();
	fileOut.close();
	cout << "Successfully removed " << numDuplicatesLines << " duplicate lines in " << inputFileName << " and wrote the result to \"output" << intermediaryOutputFileString << fileExtension << "\" (in the working directory).\a\n";
	if (isVerboseModeOn) {
		for (auto i = duplicateLinesMap.begin(); i != duplicateLinesMap.end(); i++) {
			if (i->second.second > 1) {
				string formatAdjustedLine = i->first;
				formatAdjustedLine.resize(formatAdjustedLine.length() + count(formatAdjustedLine.begin(), formatAdjustedLine.end(), '\n') + count(formatAdjustedLine.begin(), formatAdjustedLine.end(), '\r')); //allocate space for the current length plus an extra (backslash) character (to be added) before each '\n' and '\r'
				if (formatAdjustedLine.length() != i->first.length()) { //if some '\n' and/or '\r' were found
					for (unsigned int j = i->first.length() - 1, k = formatAdjustedLine.length() - 1; j != k; j--, k--) { //overflow of j and k may occur, but such will lead to the immediate termination of the loop
						switch (formatAdjustedLine[j]) {
						case '\n':
							formatAdjustedLine[k] = 'n';
							formatAdjustedLine[--k] = '\\';
							break;
						case '\r':
							formatAdjustedLine[k] = 'r';
							formatAdjustedLine[--k] = '\\';
							break;
						default:
							formatAdjustedLine[k] = formatAdjustedLine[j];
							break;
						}
					}
				}
				cout << "Removed " << setw(streamsize(floor(log10(modeCount) + 1))) << i->second.second - 1 << " of \"" << formatAdjustedLine << "\"\n";
			}
		}
		cout << '\n';
	}
	return 0;
}


void handleArgument(const string& argument, const unsigned int fileNum, bool& haveShownHelp, bool& doIgnoreNextFlag, bool& isVerboseModeOn, char& delimiter) {
	if (doIgnoreNextFlag) {
		doIgnoreNextFlag = false;
		handleFile(argument, fileNum, delimiter, isVerboseModeOn);
	}
	else if (argument.length() == 2 && argument[0] == '-') {
		switch (argument[1]) {
		case 'h':
			if (!haveShownHelp) {
				haveShownHelp = true;
				printHelp();
			}
			break;
		case 'i':
			doIgnoreNextFlag = true;
			break;
		case 'n':
			delimiter = '\n';
			break;
		case 'r':
			delimiter = '\r';
			break;
		case 'v':
			isVerboseModeOn = !isVerboseModeOn;
			break;
		default:
			handleFile(argument, fileNum, delimiter, isVerboseModeOn);
			break;
		}
	}
	else {
		handleFile(argument, fileNum, delimiter, isVerboseModeOn);
	}
}


int main(int argc, char* argv[]) {
	bool haveShownHelp = false, doIgnoreNextFlag = false, isVerboseModeOn = false;
	char delimiter = '\n';
	for (int i = 1; i < argc; i++) {
		handleArgument(argv[i], i - 1, haveShownHelp, doIgnoreNextFlag, isVerboseModeOn, delimiter);
	}
	if (argc == 1) {
		cout << "List the input file(s) and any flags (\"-h\" for help): ";
		string arg;
		getline(cin, arg);
		if (arg.empty()) { cout << "Successfully removed 0 duplicate lines in no files and did not write any results to any output file.\a\n"; return 0; }
		unsigned int numFiles = 0;
		for (istringstream iss(arg); iss >> arg; numFiles++) {
			handleArgument(arg, numFiles, haveShownHelp, doIgnoreNextFlag, isVerboseModeOn, delimiter);
		}
	}

	return 0;
}