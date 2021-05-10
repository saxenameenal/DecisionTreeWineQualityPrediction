
winequality: winequality.cpp
	g++ -std=c++11 winequality.cpp -o winequality.bin

clean:
	rm winequality.bin *.txt 