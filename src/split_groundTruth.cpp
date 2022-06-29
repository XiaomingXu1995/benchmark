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
	string pwd = "/home/user_home/public/benchmark/src/split_groundTruth.cpp";
	string dependency = "groundTruth (format in <accession, taxid, organism_name>)";
	string example = application + " million.groundTruth 375528.fileList resultDir";
	args.push_back("totalGroundTruth");
	args.push_back("fileList");
	args.push_back("resultDir");
	descriptions.push_back("the groundTruth for all genomes, including accession, species_taxid, organism_name");
	descriptions.push_back("the fileList need to be split by the species_taxid, one genome path per line");
	descriptions.push_back("the result directory, one file means a species genome fileList, one genome path per line in the file and the file name is the genomeNumber_orgnanismName.filelist");

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
	string groundTruthFile = argv[1];
	string totalFileList = argv[2];
	string outputDir = argv[3];

	//get groundtruth
	ifstream ifs0(groundTruthFile);
	string line;
	unordered_map<string, int> accession_taxid_map;
	unordered_map<int, string> taxid_organism_map;
	getline(ifs0, line);//get the table head
	while(getline(ifs0, line)){
		stringstream ss;
		string accession, organismName(""), tmpStr;
		int taxid;
		ss << line;
		ss >> accession >> taxid; 
		while(ss>>tmpStr){
		organismName += tmpStr + '_';
		}
		organismName = organismName.substr(0, organismName.length()-1);
		accession_taxid_map.insert({accession, taxid});
		taxid_organism_map.insert({taxid, organismName});
	}
	ifs0.close();
	cerr << "the size of accession groundTruth is: " << accession_taxid_map.size() << endl;
	cerr << "the size of taxid groundTruth is: " << taxid_organism_map.size() << endl;

	ifstream ifs1(totalFileList);
	unordered_map<int, vector<string>> taxid_accessionArr_map;
	while(getline(ifs1, line)){
		int startPos = line.find_last_of('/');
		int endPos = line.find_first_of('_', startPos+5);
		string accession = line.substr(startPos+1, endPos-startPos-1);
		int taxid = accession_taxid_map[accession];
		vector<string> curGenomes;
		taxid_accessionArr_map.insert({taxid, curGenomes});
		taxid_accessionArr_map[taxid].push_back(line);
	}
	cerr << "the size of taxid_accessionArr_map is: " << taxid_accessionArr_map.size() << endl;

	string cmd0 = "mkdir -p " + outputDir;
	system(cmd0.c_str());
	unordered_set<string> nameSet;
	string finalName;
	for(auto x : taxid_accessionArr_map){
		int taxid = x.first;
		string organismName = taxid_organism_map[taxid];
		if(nameSet.find(organismName) == nameSet.end()){
			finalName = organismName;
			nameSet.insert(finalName);
		}
		else{
			int index = 0;
			finalName = organismName + '_' + to_string(index);
			while(nameSet.find(finalName) != nameSet.end()){
				index++;
				finalName = organismName + '_' + to_string(index);
			}
			nameSet.insert(finalName);
		}
		vector<string> fileList = x.second;
		int fileListSize = fileList.size();
		//string outName = outputDir + '/' + finalName + to_string(fileListSize) + ".fileList";
		string outName = outputDir + '/' + to_string(fileListSize) + '-' + finalName + ".fileList";
		ofstream ofs2(outName);
		for(int j = 0; j < fileList.size(); j++){
			ofs2 << fileList[j] << endl;
		}
		ofs2.close();
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
