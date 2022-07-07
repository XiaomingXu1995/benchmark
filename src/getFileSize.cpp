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
#include <math.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <stdint.h>

#include <sys/stat.h>
using namespace std;


int main(int argc, char * argv[]){
	if(argc < 2) {
		cerr << "run as: " << argv[0] << " fileList" << endl;
		return 1;
	}

	string inputFile = argv[1];
	ifstream ifs(inputFile);
	string line;
	vector<string> fileList;
	while(getline(ifs, line)){
		fileList.push_back(line);
	}
	ifs.close();

	vector<uint64_t> fileSizeArr;
	uint64_t totalSize = 0;
	int fileNumber = fileList.size();
	for(int i = 0; i < fileList.size(); i++){
		struct stat statbuf;
		stat(fileList[i].c_str(), &statbuf);
		uint64_t fileSize = statbuf.st_size;
		fileSizeArr.push_back(fileSize);
		totalSize += fileSize;
	}
	uint64_t averageSize = totalSize / fileNumber;
	std::sort(fileSizeArr.begin(), fileSizeArr.end());
	uint64_t maxSize = *(fileSizeArr.end()-1);
	uint64_t minSize = *(fileSizeArr.begin());
	for(int i = 0; i < fileSizeArr.size(); i++){
		cout << fileSizeArr[i] << endl;
	}
	
	cerr << "the totalSize is: " << totalSize << endl;
	cerr << "the average size is: " << averageSize << endl;
	cerr << "the max size is: " << maxSize << endl;
	cerr << "the min size is: " << minSize << endl;
	
	return 0;
}
