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
	string pwd = "/home/user_home/public/demo_cpp/benchmark/src/split_getSketch.cpp";
	string example = application + " 37803.fileList 37803_sketchPath";
	string dependency = "openmp";
	args.push_back("genomesFileList");
	args.push_back("outputSketchPath");
	descriptions.push_back("input fileList, one genome file path per line");
	descriptions.push_back("output sketch file(.msh) path, each genome get one .msh sketch file");

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
	string outputPath = argv[2];
	ifstream ifs(inputFile);
	string line;
	int index = 1;
	vector<string> fileList;
	string mkdirection = "mkdir -p " + outputPath;
	system(mkdirection.c_str());
	while(getline(ifs, line)){
		//string outName = outputPath + '/' + to_string(index++) + ".file";	
		int startPos = line.find_last_of('/');
		int endPos = line.find_first_of('_', startPos + 5);
		string accession = line.substr(startPos+1, endPos - startPos-1);
		string outName = outputPath + '/' + accession;
		fileList.push_back(outName);
		ofstream ofs(outName);
		ofs << line << endl;
		ofs.close();
	}
	cerr << "finish split file" << endl;
	int threadNumber = get_nprocs_conf();
	#pragma omp parallel for num_threads(threadNumber) schedule(dynamic)
	for(int i = 0; i < fileList.size(); i++){
		string inputName = fileList[i];
		string cmd = "mash sketch -l " + inputName;
		system(cmd.c_str());
		string cmd2 = "rm " + inputName;
		system(cmd2.c_str());
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
