/* Author: Xiaoming Xu
 * Data: 2022/6/27
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
#include "kseq.h"

KSEQ_INIT(gzFile, gzread);

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
	string pwd = "/home/user_home/public/benchmark/src/mergeSeq.cpp";
	string dependency = "kseq.h, zlib, openmp";
	string example = application + " inputList outputDir";
	args.push_back("OriginFileList");
	args.push_back("outputDir");
	descriptions.push_back("input file, fileList, one genome path per line");
	descriptions.push_back("output file path, output file directory");

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
	string outputPath = argv[2];

	ifstream ifs(inputList);
	string line;
	vector<string> fileList;
	while(getline(ifs, line)){
		fileList.push_back(line);
	}
	
//	#pragma omp parallel for num_threads(8)

	int index = 1;
	int innerIndex = 1;
	for(int i = 0; i < fileList.size(); i++){
		if(fileList[i].length() == 0){
			index++;
			innerIndex = 1;
			continue;
		}
		gzFile fp1 = gzopen(fileList[i].c_str(), "r");
		if(!fp1){
			cerr << "cannot open: " << fileList[i] << endl;
			exit(1);
		}
		kseq_t * ks1 = kseq_init(fp1);
		int startPos = fileList[i].find_last_of('/');
		string fileName = fileList[i].substr(startPos+1);
		string outputFile = outputPath + '/' + to_string(index) + '_' + to_string(innerIndex) + '_' + fileName;
		ofstream ofs(outputFile);
		if(!ofs){
			cerr << "error outputFile: " << outputFile << endl;
			exit(1);
		}
		while(1){
			int length = kseq_read(ks1);
			if(length < 0) break;
			string name = ks1->name.s;
			string content = ks1->seq.s;
			ofs << content << endl;
		}
		ofs.close();
		gzclose(fp1);
		kseq_destroy(ks1);
		innerIndex++;
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
