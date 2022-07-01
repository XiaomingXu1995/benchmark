/* Author: Xiaoming Xu
 * Data: 2022/6/30
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
#include "kseq.h"
#include <zlib.h>

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
	string pwd = "/home/user_home/public/benchmark/src/mergeFASTA.cpp";
	string dependency = "kseq.h, zlib";
	string example = application + " bacteria.list bacteria_total.fna";
	args.push_back("file.list");
	args.push_back("output.fna");
	descriptions.push_back("input file, input genome file path, one genome path per line");
	descriptions.push_back("output fila, output genome file in FASTA format, each sequence is a genome");

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

	ifstream ifs(inputFile);
	if(!ifs)
	{	
		cerr << "cannot open: " << inputFile << ", exit!" << endl;
		return 1;
	}
	string line;
	vector<string> fileList;
	while(getline(ifs, line))
	{
		fileList.push_back(line);
	}

	FILE *fp = fopen(outputFile.c_str(), "w");
	uint64_t totalLength = 0;
	int seqIndex = 0;

	#pragma omp parallel for num_threads(8)
	for(int i = 0; i < fileList.size(); i++)
	{
		gzFile fp1 = gzopen(fileList[i].c_str(), "r");
		if(!fp1)
		{
			cerr << "cannot open: " << fileList[i] << ", exit! " << endl;
			continue;
			//return 1;
		}
		kseq_t * ks1 = kseq_init(fp1);
		int index = 0;
		string writeBuffer("");
		while(1)
		{
			int length = kseq_read(ks1);
			if(length < 0) break;
			string name = ks1->name.s;
			string comment = ks1->comment.s;
			string content = ks1->seq.s;
			string headLine = '>' + name + ' ' + comment +'\n';
			if(index == 0)//first sequence
			{
				writeBuffer += headLine;
			}
			for(int j = 0; j < length; j+=80)
			{
				string tmpContent = content.substr(j, 80);
				writeBuffer += tmpContent + '\n';
			}
			index++;
		}
		#pragma omp critical
		{
			fwrite(writeBuffer.c_str(), sizeof(char), writeBuffer.length(), fp);
			seqIndex++;
		}
		//string().swap(headLine);
		gzclose(fp1);
		kseq_destroy(ks1);
	}//end for
	cerr << "the totalNumber of sequence is: " << seqIndex << endl;
	fclose(fp);
	cerr << "after fclose " << endl;

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
