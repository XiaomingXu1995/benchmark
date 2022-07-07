/* Author: Xiaoming Xu
 * Data: 2022/5/31
 *
 * 
 */
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cassert>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <sys/sysinfo.h>
#include <omp.h>
#include <set>
#include <math.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <zlib.h>



using namespace std;

void printInfo(string pwd, string dependency, string example, vector<string> args, vector<string> descriptions);

int main(int argc , char *argv[]){
	string application = argv[0];
	vector<string> args, descriptions;
	args.push_back(application);
	descriptions.push_back("the application name");

	//========= parameters need changing ========
	//The example is with parameters of specific numbers.
	//args is the tutorial names.
	string pwd = "/home/user_home/public/benchmark/src/filterSeqN.cpp";
	string dependency = "None";
	string example = application + " fileList ";
	args.push_back("FileList");
	descriptions.push_back("input file, fileList to be filtered, one file path per line");

	//-------- no changing -----------
	assert(args.size() == descriptions.size());
  if(argc != args.size()) {
		printInfo(pwd, dependency, example, args, descriptions);
    return 1;
  }
	else if(argc == 2 && (argv[1] == "-h" || argv[1] == "--help"))
	{
		printInfo(pwd, dependency, example, args, descriptions);
		return 1;
	}

	//======== specific implement ========
	string inputFile = argv[1];
	ifstream ifs(inputFile);
	string line;
	vector<string> fileList;
	while(getline(ifs, line)){
		fileList.push_back(line);
	}
	ifs.close();
	for(int i = 0; i < fileList.size(); i++){
		ifstream ifs0(fileList[i]);
		string outputFile = fileList[i] + ".filter";
		ofstream ofs0(outputFile);
		while(getline(ifs0, line)){
			string curLine = "";
			for(int j = 0; j < line.length(); j++){
				if(line[j] != 'N'){
					curLine += line[j];
				}
			}
			ofs0 << curLine << endl;
		}
		ifs0.close();
		ofs0.close();
	}

  return 0;
}

void printInfo(string pwd, string dependency, string example, vector<string> args, vector<string> descriptions){
	assert(args.size() == descriptions.size());
	cerr << endl;
	cerr << "example: " << example << endl;
	cerr << endl;
	cerr << "source file path: " << pwd << endl;
	cerr << endl;
	cerr << "dependency: " << dependency << endl;
	cerr << endl;
	cerr << "run as: ";
	for(int i = 0; i < args.size(); i++){
		cerr << args[i] << ' ';
	}
	cerr << endl;
	for(int i = 0; i < args.size(); i++){
		fprintf(stderr, "\tThe %d paramter(%s) is %s\n", i, args[i].c_str(), descriptions[i].c_str());
	}
}
