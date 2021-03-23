#build lib2 for
mkdir build-lib2
cd build-lib2

cmake -DCMAKE_BUILD_TYPE=Release ../lib2
cmake --build .
make
