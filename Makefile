FLAG = -std=c++14 -g
GCC = g++
Buffer.o: Buffer.cpp
	$(GCC) $(FLAG) -c Buffer.cpp -o Buffer.o
testPacket: testPacket.cpp Buffer.o
	$(GCC) $(FLAG) testPacket.cpp -o main Buffer.o
clean:
	rm *.o
