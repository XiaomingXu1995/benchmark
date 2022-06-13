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

void printInfo(string pwd, string dependancy, string example, vector<string> args, vector<string> descriptions);

int main(int argc , char *argv[]){
	string application = argv[0];
	vector<string> args, descriptions;
	args.push_back(application);
	descriptions.push_back("the application name");

	//========= parameters need changing ========
	//The example is with parameters of specific numbers.
	//args is the tutorial names.
	string pwd = "/home/user_home/public/demo_cpp/benchmark/distMatrixList.cpp";
	string dependancy = "mash, openmp, system(cmd)";
	string example = application + " 37803.mshList 150000.msh 37803_150000+distDir 37803_150000_dense";
	args.push_back("mshList");
	args.push_back("ref.msh");
	args.push_back("distDir");
	args.push_back("denseResult");
	descriptions.push_back("sketch file list, one genome sketch per line");
	descriptions.push_back("reference sketch file, including multiple genomes sketch informations");
	descriptions.push_back("distance outputFile directory, to save the distance matrix output result");
	descriptions.push_back("dense result(outputFile prefix, outputFile is denseResult.sorted , get dense information of each distance matrix");

	//-------- no changing -----------
	assert(args.size() == descriptions.size());
  if(argc != args.size()) {
		printInfo(pwd, dependancy, example, args, descriptions);
    return 1;
  }
	else if(argc == 2 && (argv[1] == "-h" || argv[1] == "--help"))
	{
		printInfo(pwd, dependancy, example, args, descriptions);
		return 1;
	}

	//======== specific implement ========
	string inputList = argv[1];
	string refMSTFile = argv[2];
	string outputDir = argv[3];
	string denseResult = argv[4];
	ifstream ifs(inputList);
	string mshFile;
	vector<string> mshList;
	while(getline(ifs, mshFile)){
		mshList.push_back(mshFile);
	}
	ifs.close();

	string cmd0 = "mkdir -p " + outputDir;
	system(cmd0.c_str());
	
	vector<string> distFileList;
	int threadNumber = get_nprocs_conf();
	#pragma omp parallel for num_threads(threadNumber) schedule(dynamic)
	for(int i = 0; i < mshList.size(); i++){
		string curMshFile = mshList[i];
		int startPos = curMshFile.find_last_of('/');
		int endPos = curMshFile.find_last_of('.');
		string distName = curMshFile.substr(startPos+1, endPos-startPos-1);
		string outDist = outputDir + '/' + distName +".dist";
		string sortDist = outputDir + '/' + distName + ".sorted.dist";
		string cmd1 = "mash dist " + curMshFile + ' ' + refMSTFile + " >" + outDist;
		system(cmd1.c_str());
		string cmd2 = "sort -k 3 -g " + outDist + " >" + sortDist;
		system(cmd2.c_str());
		string cmd3 = "rm " + outDist;
		system(cmd3.c_str());
		cerr << "finish distance computing of the: " << i << '\t' << curMshFile << endl;
		#pragma omp critical
		{
			distFileList.push_back(sortDist);
		}
	}

	ofstream ofs(denseResult);
	#pragma omp parallel for num_threads(threadNumber) schedule(dynamic)
	for(int i = 0; i < distFileList.size(); i++){
		string curFile = distFileList[i];
		ifstream ifs0(curFile);
		string line;
		int curDense = 0;
		while(getline(ifs0, line)){
			stringstream ss;
			string name1, name2;
			double dist;
			ss << line;
			ss >> name1 >> name2 >> dist;
			if(dist > 0.05) break;
			curDense++;
		}
		#pragma omp critical
		{
		ofs << curFile << '\t' << curDense << endl;
		cerr << "finished the: " << i << '\t' << curFile << '\t' << curDense << endl;
		}
	}
	ofs.close();
	string denseResultSorted = denseResult + ".sorted";
	string cmd4 = "sort -k 2 -g -r " + denseResult + " >" + denseResultSorted;
	system(cmd4.c_str());
	string cmd5 = "rm " + denseResult;
	system(cmd5.c_str());

	return 0;
}

void printInfo(string pwd, string dependancy, string example, vector<string> args, vector<string> descriptions){
	assert(args.size() == descriptions.size());
	cerr << endl;
	cerr << "example: " << example << endl;
	cerr << endl;
	cerr << "source file path: " << pwd << endl;
	cerr << endl;
	cerr << "dependancy: " << dependancy << endl;
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
