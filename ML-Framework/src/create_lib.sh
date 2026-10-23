rm -f *.o
rm -f ../lib/Lib_ML_Framework.a
g++ -fopenmp -O3 -DDEVELOPMENT_MODE -I../include -c *.cpp
ar rcs ../lib/Lib_ML_Framework.a *.o
