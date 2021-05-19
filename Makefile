all:
	g++ -std=c++17 *.cpp -o app -lcurses -lpthread

test:
	g++ -std=c++17 test.cpp Lumberjack.cpp Nature.cpp Resources.cpp SawmillManager.cpp Transport.cpp Sawmill.cpp UI.cpp Tree.cpp Manager.cpp -o app -lcurses -lpthread