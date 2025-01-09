sudo apt-get update

sudo apt install libopenmpi-dev

sudo apt install g++

g++ serial.cpp -o serial

mpicxx parallel.cpp -o parallel

./serial

mpirun -np 4 ./parallel 

mpirun -np 8 --oversubscribe ./parallel
