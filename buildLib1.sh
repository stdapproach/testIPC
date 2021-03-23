#build lib1 for
mkdir build-lib1
cd build-lib1

cmake -DCMAKE_BUILD_TYPE=Release ../lib1
cmake --build .
make
