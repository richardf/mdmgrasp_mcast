all:
	cd lib; make
	g++ -I include *.cpp lib/*.o -o graspmcast -O2 -Wall

clean:
	rm -f *.o *~ graspmcast
	cd lib ; make clean
