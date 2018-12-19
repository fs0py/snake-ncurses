snakemake: main.cpp Player.cpp
	g++ -o snake -lncurses main.cpp Player.cpp
