/* Author: Xiaoming Xu
 * Data: 2022/6/24
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
	string pwd = "/home/user_home/public/benchmark/src/map_661k_million.cpp";
	string dependency = "mash";
	string example = application + " million.biosample File2_taxid_lineage_661K.txt 661k_in_million.txt";
	args.push_back("million.biosample");
	args.push_back("File2_taxid_lineage_661K.txt");
	args.push_back("661k_in_million.txt");
	descriptions.push_back("input file, ground truth for million genbank, only one column <biosample>");
	descriptions.push_back("input file, 661k genome groundTruth ,first line is <sample_id>");
	descriptions.push_back("output file, 661k genome sample_id in million genbank biosample");

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
	string millionFile = argv[1];
	string inputFile = argv[2];
	string outputFile = argv[3];
	ifstream ifs0(millionFile);
	string line;
	getline(ifs0, line);//header line
	unordered_set<string> sampleID_set;
	while(getline(ifs0, line)){
		stringstream ss;
		string sample_id;
		ss << line;
		ss >> sample_id;
		sampleID_set.insert(sample_id);
	}
	ifs0.close();
	ifstream ifs1(inputFile);
	ofstream ofs(outputFile);
	getline(ifs1, line);//header line
	ofs << line << endl;
	int numNotInMillion = 0;
	int numInMillion = 0;
	while(getline(ifs1, line)){
		stringstream ss;
		string sample_id;
		ss << line;
		ss >> sample_id;
		if(sampleID_set.find(sample_id) == sampleID_set.end()){
			numNotInMillion++;
		}
		else{
			ofs << line << endl;
			numInMillion++;
		}
	}
	ifs1.close();
	ofs.close();
	cerr << "number not in in million is: " << numNotInMillion << endl;
	cerr << "number in million is: " << numInMillion << endl;

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
