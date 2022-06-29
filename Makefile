all: split-sketch distMatrixList filterList split-fileListGroundTruth denseRelation judge-ok map-661k-million filter-status filter-equal-clust mergeSeq
split-sketch: src/split_getSketch.cpp
	g++ -O3 src/split_getSketch.cpp -o split-sketch -fopenmp
distMatrixList: src/distMatrixList.cpp
	g++ -O3 src/distMatrixList.cpp -o distMatrixList -fopenmp
filterList: src/filterList.cpp
	g++ -O3 src/filterList.cpp -o filterList
split-fileListGroundTruth: src/split_groundTruth.cpp
	g++ -O3 src/split_groundTruth.cpp -o split-fileListGroundTruth
denseRelation: src/denseRelation.cpp
	g++ -O3 src/denseRelation.cpp -o denseRelation
judge-ok: src/judge_ok.cpp
	g++ -O3 src/judge_ok.cpp -o judge-ok
map-661k-million: src/map_661k_million.cpp
	g++ -O3 src/map_661k_million.cpp -o map-661k-million
filter-status: src/filter_status.cpp
	g++ -O3 src/filter_status.cpp -o filter-status
filter-equal-clust: src/filterEqualClust.cpp
	g++ -O3 src/filterEqualClust.cpp -o filter-equal-clust
mergeSeq: src/mergeSeq.cpp
	g++ -O3 src/mergeSeq.cpp -o mergeSeq -lz -fopenmp

clean:
	rm split-sketch distMatrixList filterList split-fileListGroundTruth denseRelation judge-ok map-661k-million filter-status filter-equal-clust mergeSeq
	
