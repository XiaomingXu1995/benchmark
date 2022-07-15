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
	string pwd = "/home/user_home/public/benchmark/src/seqNameMapAccession.cpp";
	string dependency = "kseq.h";
	string example = application + " bacteria.list seqNameAccession.map";
	args.push_back("fileList");
	args.push_back("mapFile");
	descriptions.push_back("input file, input refseq bacteria genome fileList, one genome path per line");
	descriptions.push_back("output file, sequence map assembly accession, one map per line");

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
	string outputFile = argv[2];

	ifstream ifs0(inputFile);
	string line;
	vector<string> fileList;
	while(getline(ifs0, line)){
		fileList.push_back(line);
	}
	ifs0.close();

	unordered_map<string, string> seqName_accession_map;
	unordered_map<string, int> accession_number_map;
	#pragma omp parallel for num_threads(8)
	for(int i = 0; i < fileList.size(); i++){
		int startPos = fileList[i].find_last_of('/');
		int endPos = fileList[i].find_first_of('_', startPos+5);
		string accession = fileList[i].substr(startPos+1, endPos-startPos-1);
		gzFile fp1 = gzopen(fileList[i].c_str(), "r");
		if(!fp1){
			cerr << "cannot open: " << fileList[i] << endl;
			continue;
		}
		kseq_t *ks1 = kseq_init(fp1);
		int index = 0;
		while(1){
			int length = kseq_read(ks1);
			if(length < 0) break;
			string name = ks1->name.s;
			name = '>' + name;
			#pragma omp critical
			{
				seqName_accession_map.insert({name, accession});
			}
			index++;
		}
		#pragma omp critical
		{
			accession_number_map.insert({accession, index});
		}
		gzclose(fp1);
		kseq_destroy(ks1);
	}

	cerr << "the total number of sequence is: " << seqName_accession_map.size() << endl;
	cerr << "the total number of accession is: " << accession_number_map.size() << endl;
	cerr << "the total number of files is: " << fileList.size() << endl;

	ofstream ofs(outputFile);
	for(auto x : seqName_accession_map){
		string seqName = x.first;
		string accession = x.second;
		int number = accession_number_map[accession];
		ofs << seqName << '\t' << accession << '\t' << number << endl;
	}
	ofs.close();
	cerr << "finished!" << endl;
	
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
