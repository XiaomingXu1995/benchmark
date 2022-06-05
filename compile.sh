#!/usr/bin/bash
if [ $# -ge 1 ] && [ $1 == "clean" ];
then
	set -x
	rm split-sketch distMatrixList filterList split-fileListGroundTruth
else
	set -x
	g++ -O3 src/split_getSketch.cpp -o split-sketch -fopenmp
	g++ -O3 src/distMatrixList.cpp -o distMatrixList -fopenmp
	g++ -O3 src/filterList.cpp -o filterList
	g++ -O3 src/split_groundTruth.cpp -o split-fileListGroundTruth
fi

