#pragma once
#ifndef HEADER_H_
#define HEADER_H_

#include <cstdio>
#include <utility>
#include <stack>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <fstream>

class maze_obj
{
	public:
            // MAZE KONSTRUCTION
            maze_obj();
			void drawMaze(int height, int width);
            void generateMaze(int height, int width);
            void solver();

            // DIRECTION HANDLING FUNCTIONS
            int Check_for_dir(int x, int y, int n_moves, bool solver);
			void R_pick(int x, int y, bool solver);
            void R_pick_dir(int x, int y, int dirx, int diry, int prevx, int prevy, bool solver);

            // FILE HANDLING FUNCTIONS
            void Input_file(std::string fileName);
            void Print_to_file(std::string fileName);

            // OPTGET FUNCTIONS
            void Find_option(int argc, char *argv[]);
            void Pick_option(int argc, char *argv[]);

            // CONTROLL FUNCTIONS
            int Check_if_valid(char *optarg);
			bool b_pos_visited(int x, int y);

            // PRINT FUNCTIONS
            void Print_usages();
            void Print_maze();

	private:
			// START POS
			size_t DEFAULTY = 1;
			size_t DEFAULTX = 1;

			// MOVEMENT VARIBLES
			const int NORTHY = -2;
			const int EASTX = 2;
			const int WESTX = -2;
			const int SOUTHY = 2;

            // CONTROLLS WHERE TO MOVE
			bool walkNorth = false;
			bool walkEast = false;
			bool walkSouth = false;
			bool walkWest = false;

            // KEEPS TRACK OF HOW MANY MOVES ARE POSSIBLE FROM CURRENT POSITION
			int n_moves = 0;

			// POS_PATH HOLDS THE SOLUTION FOR THE MAZE
			std::stack<std::pair<int, int> > pos_path;

			// POS_VISITED KEEPS TRACK OF VISITED POSITIONS THROUGHOUT THE SOLVER
			std::stack<std::pair<int, int> > pos_visited;

            // HOLDS MY MAZE CHARS AND 2D VECTOR/MAZE
			struct labyrinth
			{
				const char WALK = 'x';
				const char WALL = '.';
				const char PATH = ' ';
				const char START = '+';
				const char END = 'X';
                		std::vector<std::vector<char> > maze;
			};
            		labyrinth* my_maze;
};
#endif // HEADER_H_
