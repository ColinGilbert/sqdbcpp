sqlite3.o: sqlite3.c sqlite3.h
	gcc -g -c sqlite3.c

sqdb.o: sqdb.cpp sqdb.h
	g++ -g -c sqdb.cpp

main.o: main.cpp
	g++ -g -c main.cpp

main: main.o sqdb.o sqlite3.o
	g++ -o main -g main.o sqlite3.o sqdb.o -ldl -lpthread

sqdb_unittest.o: sqdb_unittest.cpp 
	g++ -c -g `gtest-config --cppflags --ldflags` sqdb_unittest.cpp

check: sqdb.o sqdb_unittest.o 
	g++ -o sqdb_unittest -g `gtest-config --cppflags --ldflags` sqdb_unittest.o sqlite3.o sqdb.o -lgtest -lgtest_main -ldl -lpthread 
	LD_LIBRARY_PATH=/home/cosmin/local/lib:$LD_LIBRARY_PATH ./sqdb_unittest
