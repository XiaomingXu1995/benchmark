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
#include <utility>
#include <zlib.h>
#include <map>

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
	string pwd = "/home/user_home/public/benchmark/src/formatScatterDraw.cpp";
	string dependency = "boost";
	string example = application + " distList ";
	args.push_back("distList");
	descriptions.push_back("input file, distance file from mash, <seqName1, seqName2, distance, pValue, common/denom> per line in each distance file");

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
	ifstream ifs(inputFile);
	string line;
	vector<string> distFileList;
	while(getline(ifs, line)){
		distFileList.push_back(line);
	}
	ifs.close();

	//unordered_map<int, unordered_map<int, vector<pair<double, double>>>> k_s_distArr;
	map<pair<int, int>, vector<pair<double, double> > > k_s_distArr;

	string seqLength;
	for(int i = 0; i < distFileList.size(); i++){
		string fileName = distFileList[i];
		int startPos = fileName.find_last_of('/');
		fileName = fileName.substr(startPos+1);
		vector<string> vstr;
		boost::split(vstr, fileName, boost::is_any_of("_."), boost::token_compress_on);
		int kmerSize = stoi(vstr[0].substr(1));
		int sketchSize = stoi(vstr[1].substr(1));
		int muta_rate = stoi(vstr[3]);
		seqLength = vstr[6];
		double mutaDistance = (double)muta_rate / 1000.0;
		pair<int, int> p = make_pair(kmerSize, sketchSize);

		ifstream ifs0(distFileList[i]);
		getline(ifs0, line);
		vector<pair<double, double>> distArr;
		while(getline(ifs0, line)){
			stringstream ss;
			string seqName1, seqName2, jaccard;
			double distance, pValue;
			ss << line;
			ss >> seqName1 >> seqName2 >> distance >> pValue >> jaccard;
			distArr.push_back(make_pair(mutaDistance, distance));
		}
		ifs0.close();
		k_s_distArr.insert({p, vector<pair<double, double>>()});
		k_s_distArr[p].insert(k_s_distArr[p].end(), distArr.begin(), distArr.end());
		//cerr << kmerSize << endl;
		//cerr << sketchSize << endl;
		//cerr << muta_rate << endl;
		//cerr << seqLength << endl;
		//break;
	}
	cerr << "the size of k_s_map is: " << k_s_distArr.size() << endl;
	for(auto x : k_s_distArr){
		int k = x.first.first;
		int s = x.first.second;
		string outFile = "all_" + to_string(k)+ '_' + to_string(s) + '_' + seqLength + ".dist";
		ofstream ofs(outFile);
		for(auto y : x.second){
			ofs << y.first << '\t' << y.second << endl;
		}
		ofs.close();
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
