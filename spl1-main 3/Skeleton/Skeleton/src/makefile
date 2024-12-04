
all: clean compile run

compile:
		g++ -o main main.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp
		g++ -o main bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o

run:
	./main config_file.txt

clean:
	rm -f main bin/*.o

