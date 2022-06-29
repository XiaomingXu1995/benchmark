/* Author: Xiaoming Xu
 * Data: 2022/6/27
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
	string pwd = "/home/user_home/public/benchmark/src/filterEqualClust.cpp";
	string dependency = "None";
	string example = application + " bacteria.clust -l/-i filter.list";
	args.push_back("clustFile");
	args.push_back("option");
	args.push_back("filter.list");
	descriptions.push_back("input file, cluster result file from RabbitTClust");
	descriptions.push_back("input option, cluster option, -l or -i, means sketchByFile or sketchBySequence");
	descriptions.push_back("output file, output fileList with equal possibility");

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
	string clustFile = argv[1];
	string option = argv[2];
	string outputFile = argv[3];
	
	ifstream ifs0(clustFile);
	if(!ifs0){
		cerr << "cannot open: " << clustFile << endl;
		return 1;
	}
	string line;
	vector<vector<string>> clustArr;
	vector<string> curClust;
	getline(ifs0, line);
	while(getline(ifs0, line)){
		if(line.length() == 0) continue;
		if(line[0] != '\t'){
			clustArr.push_back(curClust);
			vector<string>().swap(curClust);
		}
		else{
			stringstream ss;
			int curId, globalID;
			string length, fileName, genomeName;
			ss << line;
			//cerr << line << endl;
			if(option == "-l"){
				ss >> curId >> globalID >> length >> fileName >> genomeName;
				curClust.push_back(fileName);
				//cerr << fileName << endl;
				//exit(0);
			}
			else if(option == "-i"){
				ss >> curId >> globalID >> length >> genomeName;
				curClust.push_back(genomeName);
			}
		}
	}
	if(curClust.size() != 0){
		clustArr.push_back(curClust);
	}

	string outputFileNew = outputFile + ".new";
	ofstream ofs(outputFile);
	ofstream ofs1(outputFileNew);
	for(int i = 0; i < clustArr.size(); i++){
		//cerr << clustArr[i].size() << endl;
		if(clustArr[i].size() != 1)
		{
			for(auto x : clustArr[i]){
				ofs << x << endl;
				ofs1 << x << endl;
			}
			ofs1 << endl;
			//ofs << endl;
		}
	}
	ofs.close();

	cerr << "finished " << endl;


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
