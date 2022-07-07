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
using namespace std;


int main(int argc, char * argv[]){
	if(argc < 3){
		cerr << "run as: " << argv[0] << " error_rate, genomeLength" << endl;
		return 1;
	}

	int alphaSize = 4;
	double error_rate = stod(argv[1]);
	long genomeLength = stol(argv[2]);

	double exp_value = genomeLength * (1.0 - error_rate) / error_rate;

	double kmerDouble = log(exp_value) / log(alphaSize);
	int k = ceil(kmerDouble);
	cout << "the proper kmer size is: " << k << endl;
	


	return 0;
}
