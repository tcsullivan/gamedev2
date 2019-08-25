INC = -I..
FLG = -std=gnu++11

all:
	g++ $(INC) $(FLG) -c ../entityx/help/Pool.cc -o Pool.o
	g++ $(INC) $(FLG) -c ../entityx/help/Timer.cc -o Timer.o
	g++ $(INC) $(FLG) -c ../entityx/Event.cc -o Event.o
	g++ $(INC) $(FLG) -c ../entityx/Entity.cc -o Entity.o
	g++ $(INC) $(FLG) -c ../entityx/System.cc -o System.o
	ar rvs ../../libentityx.a *.o

