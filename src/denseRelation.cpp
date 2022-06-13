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
	string pwd = "/home/user_home/public/benchmark/src/denseRelation.cpp";
	string dependency = "None";
	string example = application + " denseList filterList relatedFileList";
	args.push_back("denseList");
	args.push_back("filterList");
	args.push_back("relatedFileList");
	descriptions.push_back("inputFile, dense file list, pair<assessionInfo, dense> per line in each dense file");
	descriptions.push_back("outputFile, the unrelated species list to delete the distMatrix for saving HDD space, these species have no dense with the reference msh(all distance are over species distance threshold(0.05)");
	descriptions.push_back("outputFile, the related accessions and dense with the reference msh");

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
	string inputList = argv[1];
	string filterList = argv[2];
	string relatedList = argv[3];
	ifstream ifs(inputList);
	string fileName;
	vector<string> fileList;
	while(getline(ifs, fileName)){
		fileList.push_back(fileName);
	}
	ifs.close();
	
	ofstream ofs1(filterList);
	ofstream ofs2(relatedList);
	for(int i = 0; i < fileList.size(); i++){
		ifstream ifs0(fileList[i]);
		string line0;
		string relatedFile0;
		int dense0;
		getline(ifs0, line0);
		stringstream ss0;
		ss0 << line0;
		ss0 >> relatedFile0 >> dense0;
		if(dense0 == 0){
			//this species has no relation with the reference msh
			ofs1 << fileList[i] << endl;
			continue;
		}
		else{
			ofs2 << relatedFile0 << '\t' << dense0 << endl;
			while(getline(ifs0, line0)){
				stringstream ss;
				string relatedFile;
				int dense;
				ss << line0;
				ss >> relatedFile >> dense;
				if(dense == 0)
					break;
				ofs2 << relatedFile << '\t' << dense << endl;
			}
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
