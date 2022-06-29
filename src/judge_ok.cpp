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
	string pwd = "/home/user_home/public/benchmark/src/judge_ok.cpp";
	string dependency = "None";
	string example = application + " bacteria.mst.clust checkStatus.groundTruth bacteria.mst.status.clust";
	args.push_back("clusterFile");
	args.push_back("statusGroundTruth");
	args.push_back("outputFile");
	descriptions.push_back("input file, cluster file from RabbitTClust, <\\t, curId, globalId, length, fileName, seqName, seqComment> per line");
	descriptions.push_back("input file, check_status groundTruth file , <genbank-accession, refseq-accession, taxid, species_taxid, check_status> per line");
	descriptions.push_back("output file, same file format with RabbitTClust file, <\\t, cutrId, golbalId, length, fileName, check_status> per line");

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
	string clusterFile = argv[1];
	string groundTruthFile = argv[2];
	string outputFile = argv[3];
	ifstream ifs0(groundTruthFile);
	string line;
	getline(ifs0, line);//the header line
	unordered_map<string, string> accession_status_map;
	while(getline(ifs0, line)){
		stringstream ss;
		string genbank_accession, refseq_accession, taxid, species_taxid, status;
		ss << line;
		ss >> genbank_accession >> refseq_accession >> taxid >> species_taxid >> status;
		accession_status_map.insert({refseq_accession, status});
	}
	ifs0.close();
	cerr << "the size of accession_status_map is: " << accession_status_map.size() << endl;
	
	ifstream ifs1(clusterFile);
	ofstream ofs(outputFile);
	int numNotStatus = 0;
	while(getline(ifs1, line)){
		if(line[0] == '\t'){
			stringstream ss;
			int curId, globalId;
			string length, fileName, sequenceName;
			ss << line;
			ss >> curId >> globalId >> length >> fileName >> sequenceName;
			int startPos = fileName.find_last_of('/');
			//int startPos = fileName.find_first_of('_');
			int endPos = fileName.find('_', startPos+5);
			string accession = fileName.substr(startPos+1, endPos-startPos-1);
			//cerr << fileName << endl;
			//cerr << startPos << endl;
			//cerr << endPos << endl;
			//cerr << accession << endl;
			//exit(0);

			string status;
			if(accession_status_map.find(accession) != accession_status_map.end()){
				status = accession_status_map[accession];
			}
			else{
				status = "None";
				numNotStatus++;
			}
			ofs << curId << '\t' << globalId << '\t' << length << '\t' << fileName << '\t' << status << endl;
		}
		else{
			ofs << line << endl;
		}
	}
	ifs1.close();
	ofs.close();
	cerr << "the number not have status is: " << numNotStatus << endl;
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
