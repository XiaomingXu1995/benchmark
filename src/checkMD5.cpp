/* Author: Xiaoming Xu
 * Data: 2022/6/29
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
	string pwd = "/home/user_home/public/benchmark/src/checkMD5.cpp";
	string dependency = "None";
	string example = application + " md5Text";
	args.push_back("md5TextFile");
	descriptions.push_back("input file, md5 text file, <md5, filePath> per line");

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
	string md5TextFile = argv[1];

	unordered_map<int, unordered_set<string>> idMD5Map;
	unordered_map<int, vector<string>> idClustMap;

	ifstream ifs(md5TextFile);
	string line;
	int preIndex = 1;
	while(getline(ifs, line)){
		stringstream ss; 
		string md5, filePath;
		ss << line;
		ss >> md5 >> filePath;
		int startPos = filePath.find_last_of('/');
		int endPos = filePath.find_first_of('_', startPos+1);
		string indexStr = filePath.substr(startPos+1, endPos-startPos-1);
		int curIndex = stoi(indexStr);
		idMD5Map.insert({curIndex, unordered_set<string>()});
		idMD5Map[curIndex].insert(md5);
		idClustMap.insert({curIndex, vector<string>()});
		idClustMap[curIndex].push_back(filePath);
	}

	for(auto x : idMD5Map){
		int clustIndex = x.first;
		if(x.second.size() == 1){
			for(auto y : idClustMap[clustIndex]){
				cout << *(x.second.begin()) << '\t' << y << endl;
			}
			cout << endl;
		}
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
