#build gateway for
mkdir build-gateway
cd build-gateway

cmake -DCMAKE_BUILD_TYPE=Release ../gateway
cmake --build .
make
