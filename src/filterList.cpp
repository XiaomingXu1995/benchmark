/* Author: Xiaoming Xu
 * Data: 2022/6/3
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


using namespace std;

void printInfo(string pwd, string example, vector<string> args, vector<string> descriptions);

int main(int argc , char *argv[]){
	string application = argv[0];
	vector<string> args, descriptions;
	args.push_back(application);
	descriptions.push_back("the application name");

	//========= parameters need changing ========
	//The example is with parameters of specific numbers.
	//args is the tutorial names.
	string pwd = "/home/user_home/public/demo_cpp";
	string example = application + " million.list 375528.list 375528.file.list";
	args.push_back("totalFile.list");
	args.push_back("fileter.list");
	args.push_back("output.list");
	descriptions.push_back("total genome file list");
	descriptions.push_back("assembly_accession list to filter, each line is the assembly_accession");
	descriptions.push_back("output filtered genome file list");

	//-------- no changing -----------
	assert(args.size() == descriptions.size());
  if(argc != args.size()) {
		printInfo(pwd, example, args, descriptions);
    return 1;
  }
	else if(argc == 2 && (argv[1] == "-h" || argv[1] == "--help"))
	{
		printInfo(pwd, example, args, descriptions);
		return 1;
	}

	//======== specific implement ========
	string genomeList = argv[1];
	string filterList = argv[2];
	string outputFile = argv[3];
	ifstream ifs0(genomeList);
	string line;
	unordered_map<string, string> accession_pwd_map;
	while(getline(ifs0, line)){
		int startPos = line.find_last_of('/');
		int endPos = line.find_first_of('_', startPos+5);
		string accession = line.substr(startPos+1, endPos-startPos-1);
		accession_pwd_map.insert({accession, line});
	}
	ifs0.close();

	ifstream ifs1(filterList);
	ofstream ofs0(outputFile);
	while(getline(ifs1, line)){
		string pwd = accession_pwd_map[line];
		if(pwd.length() != 0)
			ofs0 << pwd << endl;
	}
	ifs1.close();
	ofs0.close();

  return 0;
}

void printInfo(string pwd, string example, vector<string> args, vector<string> descriptions){
	assert(args.size() == descriptions.size());
	cerr << endl;
	cerr << "example: " << example << endl;
	cerr << endl;
	cerr << "source file path: " << pwd << endl;
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
