#build tester for
mkdir build-tester
cd build-tester

cmake -DCMAKE_BUILD_TYPE=Release ../tester
cmake --build .
make
