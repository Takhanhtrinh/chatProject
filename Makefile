FLAG = -std=c++14 -g
GCC = g++
Buffer.o: Buffer.cpp
	$(GCC) $(FLAG) -c Buffer.cpp -o Buffer.o
User.o: User.cpp
	$(GCC) $(FLAG) -c User.cpp -o User.o
Room.o: Room.cpp
	$(GCC) $(FLAG) -c Room.cpp -o Room.o
Server.o: Server.cpp
	$(GCC) $(FLAG) -c Server.cpp -o Server.o
testPacket: testPacket.cpp Buffer.o User.o
	$(GCC) $(FLAG) -luws -lz -lssl -luv testPacket.cpp -o main Buffer.o User.o
main: Main.cpp User.o Room.o Server.o Buffer.o
	$(GCC) $(FLAG) -luws -lz -lssl -luv Main.cpp -o main Buffer.o User.o Room.o Server.o
clean:
	rm *.o
