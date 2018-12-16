CC=g++
JSONPACKAGE=/usr/local/include
CFLAGS=-Wall -I$(JSONPACKAGE)
SHARED=src/polygon_operations/conversion.cpp

compute:
	$(CC) -o out/compute.o src/polygon_operations/compute.cpp $(SHARED) $(CFLAGS)

