/* Author: Xiaoming Xu
 * Data: 2022/6/25
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
	string pwd = "/home/user_home/public/benchmark/src/filter_status.cpp";
	string dependency = "None";
	string example = application + "bacteria.list prokaryote_taxonomy_check.groundTruth bacteria.filter.list";
	args.push_back("bacteria.list");
	args.push_back("prokaryote_taxonomy_check.groundTruth");
	args.push_back("bacteria.filter.list");
	descriptions.push_back("input file, genome file list, one genome path per line");
	descriptions.push_back("input file, taxonomy check status groundTruth file, <genbank-accession, refseq-accession, taxid, species-taxid, check-status> per line");
	descriptions.push_back("output file, genome list with OK status, one genome path per line ");

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
	string inputFileList = argv[1];
	string statusFile = argv[2];
	string outputFile = argv[3];

	ifstream ifs0(statusFile);
	string line;
	getline(ifs0, line);//header line
	unordered_map<string, string> refseqAccession_status_map;
	while(getline(ifs0, line)){
		stringstream ss;
		ss << line;
		string genbank_accession, refseq_accession, status;
		int taxid, species_taxid;
		ss >> genbank_accession >> refseq_accession >> taxid >> species_taxid >> status;
		//refseqAccession_status_map.insert({refseq_accession, status});
		refseqAccession_status_map.insert({genbank_accession, status});
	}
	ifs0.close();
	ifstream ifs1(inputFileList);
	ofstream ofs(outputFile);
	int numIsOk = 0;
	int numNotOk = 0;
	int numNotInStatus = 0;
	while(getline(ifs1, line)){
		int startPos = line.find_last_of('/');
		int endPos = line.find_first_of('_', startPos+5);
		string key = line.substr(startPos+1, endPos-startPos-1);
		if(refseqAccession_status_map.find(key) == refseqAccession_status_map.end()){
			numNotInStatus++;
		}
		else{
			if(refseqAccession_status_map[key] == "OK"){
				ofs << line << endl;
				numIsOk++;
			}
			else{
				numNotOk++;
			}
		}
	}
	ifs1.close();
	cerr << "the number of OK status is: " << numIsOk << endl;
	cerr << "the number is not OK is: " << numNotOk << endl;
	cerr << "the number not in status is: " << numNotInStatus << endl;

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
