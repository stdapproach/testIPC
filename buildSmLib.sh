#build smLib for
mkdir build-smLib
cd build-smLib

cmake -DCMAKE_BUILD_TYPE=Release ../smLib
cmake --build .
make
