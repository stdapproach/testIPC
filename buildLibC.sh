#build libC for
mkdir build-libC
cd build-libC

cmake -DCMAKE_BUILD_TYPE=Release ../libC
cmake --build .
make
