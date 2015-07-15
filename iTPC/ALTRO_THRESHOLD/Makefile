CPPFLAGS = `root-config --cflags`
LDFLAGS = -m32 `root-config --libs` -lGeom -lTable

all: altroEffDraw.o
	g++ $(LDFLAGS) -o runEff altroEffDraw.o

altroEffDraw.o: altroEffDraw.cc
	g++ $(CPPFLAGS) -c $^ -I.
