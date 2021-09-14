#include "header.h"

using namespace std;

// PUSH STARTING POSITION TO STACKS
maze_obj::maze_obj()
{
	pos_path.push(make_pair(DEFAULTY, DEFAULTX));
	pos_visited.push(make_pair(DEFAULTY, DEFAULTX));
}
// CREATING MAZEGRID
void maze_obj::drawMaze(int height, int width)
{
    try
    {
	    labyrinth *temps = new labyrinth();
    
	    for(int y = 0; y < height; y++)
	    {
            vector<char> col;
		    for(int x = 0; x < width; x++)
		    {
			    if(x == 0 && y == 1)
			    {
                    col.push_back('S');
			    }
			    else if(x == width - 1 && y == height - 2)
			    {
                    col.push_back(temps->END);
			    }
			    else
			    {
                    col.push_back(temps->WALL);
			    }
		    }
            temps->maze.push_back(col);
	    }
        my_maze = temps;
    }
    catch(...)
    {
        cerr << "Bad alloc, not enough memory!" << endl;
        exit(EXIT_FAILURE);
    }
}
// GENERATES THE RANDOM MAZE
void maze_obj::generateMaze(int height, int width)
{
    // MAKING SURE THERE IS A MAZEGRID BEFORE MODIFYING IT
    this->drawMaze(height, width);

    for(size_t y = DEFAULTY; y < my_maze->maze.size();)
    {
        for(size_t x = DEFAULTX; x <= my_maze->maze[y].size();)
        {
            // SET START POINT
            if((x == DEFAULTX) && (y == DEFAULTY))
            {
                my_maze->maze[y][x] = my_maze->START;
                pos_path.push(make_pair(y, x));
            }
            // UPDATE CURRENT POSITION
            x = DEFAULTX;
            y = DEFAULTY;
			
			// CHECK FOR NEXT MOVES
			n_moves = Check_for_dir(x, y, n_moves, false);
            
            // GENERATE AN RANDOM DIRECTION
            if(n_moves >= 1)
                R_pick(x, y, false);
            else
            {
                // IF BACKTRACKING LEADS BACK TO THE BEGINNING
                if(my_maze->maze[y][x] == my_maze->START)
                {
                    return;
                }
                pos_path.pop();
                pos_path.pop();
                DEFAULTY = pos_path.top().first;
                DEFAULTX = pos_path.top().second;
            }
            n_moves = 0;
        }
    }
}
// MAZE SOLVER
void maze_obj::solver()
{
    for(size_t y = DEFAULTY; y < my_maze->maze.size();)
    {
        for(size_t x = DEFAULTX; x < my_maze->maze[y].size();)
        {	
            // IF NEXT MOVE COULD FINISH THE MAZE, DO IT
            if(my_maze->maze[y][x + 1] == my_maze->END)
            {
                cout << "Solution found!" << endl;
                pos_path.push(make_pair(y, x + EASTX - 1));
                pos_path.push(make_pair(y, x + EASTX));
                return;
            }
            
            // CHECK FOR NEXT MOVES
            n_moves = Check_for_dir(x, y, n_moves, true);
            
            // GENERATE A RANDOM DIRECTION
            if(n_moves >= 1)
                R_pick(x, y, true);
            else
            {
                // IF BACKTRACKING LEADS TO THE BEGINNING, NO SOLUTION
                if(my_maze->maze[y][x] == my_maze->START)
                {
                    cout << "Solution not found!" << endl;
                    pos_path.pop();
                    return;
                }
                // DEADEND - BACKTRACKING - REPLACE WALK SYMBOL WITH PATH
                my_maze->maze[pos_path.top().first][pos_path.top().second] = my_maze->PATH;
                pos_path.pop();
                my_maze->maze[pos_path.top().first][pos_path.top().second] = my_maze->PATH;
                pos_path.pop();
                DEFAULTY = pos_path.top().first;
                DEFAULTX = pos_path.top().second;
            }
            // UPDATE CURRENT POSITION
            x = DEFAULTX;
            y = DEFAULTY;
            n_moves = 0;
        }
    }
}
// CHECK WHAT THE NEXT DIRECTION COULD BE
int maze_obj::Check_for_dir(int x, int y, int n_moves, bool solver)
{
    // TYPECONVERSION FROM SIZE_T TO INT
	int sizey = static_cast<int>(my_maze->maze.size());
    int sizex = static_cast<int>(my_maze->maze[y].size());

	// GO NORTH
	if((y + NORTHY > 0) && ((x > 0) && (x < sizex - 1)))
	{
		// NORTH WALK IS POSSIBLE FOR THE SOLVER
		if(solver == true)
		{
			if((my_maze->maze[y + NORTHY + 1][x] == my_maze->PATH) && (b_pos_visited(x, y + NORTHY + 1) == false))
			{
				walkNorth = true;
				n_moves++;
			}
		}
        // NORTH WALK IS POSSIBLE FOR THE GENERATOR
		else
		{
			if(my_maze->maze[y + NORTHY][x] == my_maze->WALL)
			{
				walkNorth = true;
				n_moves++;
			}
		}
    }
    // GO EAST
    if((x + EASTX < sizex - 1) && ((y > 0) && (y < sizey - 1)))
    {
		// EAST WALK IS POSSIBLE FOR THE SOLVER
		if(solver == true)
		{
			if((my_maze->maze[y][x + EASTX - 1] == my_maze->PATH) && (b_pos_visited(x + EASTX - 1, y) == false))
			{
				walkEast = true;
				n_moves++;
			}
		}
        // EAST WALK IS POSSIBLE FOR THE GENERATOR
		else
		{
			if(my_maze->maze[y][x + EASTX] == my_maze->WALL)
			{
				walkEast = true;
				n_moves++;
			}
		}
    }
    // GO SOUTH
    if((y + SOUTHY < sizey - 1) && ((x > 0) && (x < sizex - 1)))
    {
		// SOUTH WALK IS POSSIBLE FOR THE SOLVER
        if(solver == true)
        {
			if((my_maze->maze[y + SOUTHY - 1][x] == my_maze->PATH) && (b_pos_visited(x, y + SOUTHY - 1) == false))
			{
				walkSouth = true;
				n_moves++;
			}
		}
        // SOUTH WALK IS POSSIBLE FOR THE GENERATOR
        else
        {
			if(my_maze->maze[y + SOUTHY][x] == my_maze->WALL)
			{
				walkSouth = true;
				n_moves++;
			}
		}
    }
    // GO WEST
	if((x + WESTX > 0) && ((y > 0) && (y < sizey - 1)))
    {
		// WEST WALK IS POSSIBLE FOR THE SOLVER
        if(solver == true)
        {
			if((my_maze->maze[y][x + WESTX + 1] == my_maze->PATH) && (b_pos_visited(x + WESTX + 1, y) == false))
			{
				walkWest = true;
				n_moves++;
			}
		}
        // WEST WALK IS POSSIBLE FOR THE GENERATOR
        else
        {
			if(my_maze->maze[y][x + WESTX] == my_maze->WALL)
			{
				walkWest = true;
				n_moves++;
			}
		}
    }
    return n_moves;
}
// GENERATES AN RANDOM DIRECTION
void maze_obj::R_pick(int x, int y, bool solver)
{
    // VARIBLE THAT HOLDS THE RANDOM PICK
    int choice;

    // VARIBLES TO STORE IMPORTANT DATA ABOUT THE DIRECTION MOVEMENT
    int dirx;
    int diry;
    int prevx;
    int prevy;

    while(true)
    {
        // GENERATES AN RANDOM INT BETWEEN 1-4
        choice = rand () % 4 + 1;

        if(choice == 1)
        {
            if(walkNorth == true)
            {
                // VARIBLES TO KEEP TRACK OF NORTH MOVEMENT
                dirx = 0;
                diry = NORTHY;
                prevx = 0;
                prevy = 1;
                
                // MAKING THE MOVEMENT
                R_pick_dir(x, y, dirx, diry, prevx, prevy, solver);
                break;
            }
        }
        else if(choice == 2)
        {
            if(walkEast == true)
            {
                // VARIBLES TO KEEP TRACK OF EAST MOVEMENT
                dirx = EASTX;
                diry = 0;
                prevx = -1;
                prevy = 0;
                
                // MAKING THE MOVEMENT
                R_pick_dir(x, y, dirx, diry, prevx, prevy, solver);
                break;
            }
        }
        else if(choice == 3)
        {
            if(walkSouth == true)
            {
                // VARIBLES TO KEEP TRACK OF SOUTH MOVEMENT
                dirx = 0;
                diry = SOUTHY;
                prevx = 0;
                prevy = -1;

                // MAKING THE MOVEMENT
                R_pick_dir(x, y, dirx, diry, prevx, prevy, solver);
                break;
            }
        }
        else if(choice == 4)
        {
            if(walkWest == true)
            {
                // VARIBLES TO KEEP TRACK OF WEST MOVEMENT
                dirx = WESTX;
                diry = 0;
                prevx = 1;
                prevy = 0;
                
                // MAKING THE MOVEMENT
                R_pick_dir(x, y, dirx, diry, prevx, prevy, solver);
                break;
            }
        }
    }
    // MAKE ALL THE DIRECTIONS UNVALID FOR NEXT ITERATION
    walkNorth = false;
    walkEast = false;
    walkSouth = false;
    walkWest = false;
}
// DOING THE ACTUELLY MOVEMENT
void maze_obj::R_pick_dir(int x, int y, int dirx, int diry, int prevx, int prevy, bool solver)
{
    // PUSHING THE NEW POSITION TO THE STACK
    pos_path.push(make_pair(y + diry + prevy, x + dirx + prevx));
    pos_path.push(make_pair(y + diry, x + dirx));

    // IF THE SOLVER IS USING THIS FUNCTION
    if(solver == true)
    {
        // MARK THE POSITION AS VISITED
        pos_visited.push(make_pair(y + diry + prevy, x + dirx + prevx));
        pos_visited.push(make_pair(y + diry, x + dirx));

        // CHANGES THE SYMBOL TO WALK 'x'
        my_maze->maze[y + diry + prevy][x + dirx + prevx] = my_maze->WALK;
        my_maze->maze[y + diry][x + dirx] = my_maze->WALK;
    }
    // IF THE GENERATOR USES THIS FUNCTION
    else
    {
        // CHANGES THE SYMBOL TO PATH ' '
        my_maze->maze[y + diry + prevy][x + dirx + prevx] = my_maze->PATH;
        my_maze->maze[y + diry][x + dirx] = my_maze->PATH;
    }
    // UPDATING CURRENT POS
    DEFAULTY = pos_path.top().first;
    DEFAULTX = pos_path.top().second;
}
// GETOPT - OPTIONS
void maze_obj::Find_option(int argc, char *argv[])
{
    srand(time(NULL));
    
    // SHORT DESCRIPTION
    if(argc == 1)
    {
        cout << "********************************************" << endl;
        cout << "*           Labyrinth - v.1.0.0            *" << endl;
        cout << "********************************************" << endl;
        cout << "*                                          *" << endl;
        cout << "*  A simple program to generate an maze    *" << endl;
        cout << "* in diffrent ways in a noninteractiv mode *" << endl;
        cout << "*                                          *" << endl;
        cout << "********************************************" << endl;
        cout << "*        Created by Joakim Söderlund       *" << endl;
        cout << "********************************************" << endl;
    }
    else if(argc >= 2 && argc <= 8)
    {
        Pick_option(argc, argv); 
    }
    else
    {
        Print_usages();
    }
}
// PICK AN OPTION
void maze_obj::Pick_option(int argc, char *argv[])
{
    // TURNS OFF OPTGET ERROR MESSAGES
    opterr = 0;

    // MAKING LONG OPTIONS BE DETECTED AS SHORT
    const option long_option[] = {
        {"version", no_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {"size", required_argument, nullptr, 's'},
        {"columns", required_argument, nullptr, 'c'},
        {"rows", required_argument, nullptr, 'r'},
        {"input", required_argument, nullptr, 'i'},
        {"output", required_argument, nullptr, 'o'},
        {"print", no_argument, nullptr, 'p'},
        {"check", no_argument, nullptr, 'f'},
        {nullptr, no_argument, nullptr, 0}
    };

    // USED FOR CONTROLLING -C -R ARGUMENTS
    bool skip_n_option = false;

    // USED TO CHECK IF THERE HAS BEEN A OPTION BEFORE
    bool first_option = false;

    // USED TO STORE NEXT ARGUMENT
    char next_arg;
    char next_arg_long;

    int option;

    // UNTIL GETOPT RETURNS -1 BECAUSE NO MORE ARGUMENTS WAS INPUTTED
    while((option = getopt_long(argc, argv, "vhs:c:r:i:o:pf", long_option, nullptr)) != -1)
    {
        switch(option)
        {
            case 'v':
                cout << "Version - 1.0.0\n";
                break;

            case 'h':
                Print_usages();
                break;

            case 's':
                generateMaze(Check_if_valid(optarg), Check_if_valid(optarg));
                first_option = true;
                break;

            case 'c':
                // TO CHECK IF -R HAS ALREADY BEEN DONE
                if(skip_n_option == false)
                {
                    // IF ARGC IS BIGGER THEN 3 GRAB THE NEXT OPTION
                    if(argc > 3)
                    {
                        // BAD SOLUTION BUT FOUND NO OTHER WAY
                        next_arg = argv[3][1];
                        next_arg_long = argv[3][2];
                    }
                    // IF NEXT ARGUMENT IS 'r' ARGV[4] CONTAIN THE HEIGHT
                    if(next_arg == 'r' || next_arg_long == 'r')
                    {
                        // INPUT HEIGHT/WIDTH
                        generateMaze(Check_if_valid(argv[4]), Check_if_valid(optarg));
                        skip_n_option = true;
                        first_option = true;
                        break;
                    }
                    else
                    {
                        cerr << "Unable to create maze with width only, try combining -c and -r" << endl;
                        cerr << "For further information type \"./Labyrinth -h\" " << endl;
                        exit(EXIT_FAILURE);
                    }
                }
                break;

            case 'r':
                // TO CHECK IF -C HAS ALREADY BEEN DONE
                if(skip_n_option == false)
                {
                    // IF ARGC IS BIGGER THEN 3 GRAB THE NEXT OPTION
                    if(argc > 3)
                    {
                        // BAD SOLUTION BUT FOUND NO OTHER WAY
                        next_arg = argv[3][1];
                        next_arg_long = argv[3][2];
                    }
                    // IF NEXT ARGUMENT IS 'c' ARGV[4] CONTAIN THE WIDTH
                    if(next_arg == 'c' || next_arg_long == 'c')
                    {
                        generateMaze(Check_if_valid(optarg), Check_if_valid(argv[4]));
                        skip_n_option = true;
                        first_option = true;
                        break;
                    }
                    else
                    {
                        cerr << "Unable to create maze with height only, try combining -r and -c" << endl;
                        cerr << "For further information type \"./Labyrinth -h\" " << endl;
                        exit(EXIT_FAILURE);
                    }
                }
                break;

            case 'i':
                Input_file(optarg);
                if(my_maze->maze.size() < 1)
                {
                    cerr << "(" << optarg << ") is empty!" << endl;
                    cerr << "For further information type \"./Labyrinth -h\" " << endl;
                    exit(EXIT_FAILURE);
                }
                first_option = true;
                break;

            case 'o':
                // IF o ISNT THE FIRST ARGUMENT
                if(first_option == true)
                    Print_to_file(optarg);
                else
                {
                    cerr << "Unable to output nonexisting maze to (" << optarg << ")" << endl;
                    cerr << "For further information type \"./Labyrinth -h\" " << endl;
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                // IF p ISNT THE FIRST ARGUMENT
                if(first_option == true)
                    Print_maze();
                else
                {
                    cerr << "Unable to output maze or file content before input or maze creation." << endl;
                    cerr << "For further information type \"./Labyrinth -h\" " << endl;
                    exit(EXIT_FAILURE);
                }
                break;

            case 'f':
                // IF f ISNT THE FIRST ARGUMENT
                if(first_option == true) 
                    solver();
                else
                {
                    cerr << "Unable to check for solution before maze created" << endl;
                    cerr << "For further information type \"./Labyrinth -h\" " << endl;
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                cout << "Invalid option!" << endl;
                cout << "For more information try \"./Labyrinth -h\"" << endl;
                break;
        }
    }
}
// OUTPUTS THE MAZE TO FILE OR CONSOL
void maze_obj::Print_to_file(string fileName)
{
    ofstream file (fileName);

    if(file.is_open())
    {
        for(size_t y = 0; y < my_maze->maze.size(); y++)
        {
            for(size_t x = 0; x < my_maze->maze[y].size(); x++)
            {
                file << my_maze->maze[y][x];
            }
            file << "\n";
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file!" << endl;
        Print_maze();
    }
}
// INPUT FROM FILE
void maze_obj::Input_file(string fileName)
{
    ifstream fileInput (fileName);
    if(!fileInput.is_open())
    {
        cerr << "Unable to open or locate (" << fileName << ")" << endl; 
        exit(EXIT_FAILURE);
    }

    labyrinth *temps = new labyrinth();
    string inputString;

    while(getline(fileInput ,inputString))
    {
        vector<char> temp;
        for(size_t i = 0; i < inputString.length(); i++)
        {
            temp.push_back(inputString[i]); 
        }
        temps->maze.push_back(temp);
    }
    fileInput.close();
    my_maze = temps;
}
// CHECK IF POSITION HAS BEEN VISITED ALREADY
bool maze_obj::b_pos_visited(int x, int y)
{
    bool visited = false;
    stack<pair<int, int>> pos_temp;

    while(!pos_visited.empty())
    {
        // IF THE POSITION HAS ALREADY BEEN VISITED
        if(y == pos_visited.top().first && x == pos_visited.top().second)
        {
            visited = true;
        }
        pos_temp.push(make_pair(pos_visited.top().first, pos_visited.top().second));
        pos_visited.pop();
    }
    pos_visited = pos_temp;
    return visited;
}
// CHECK IF VALID INPUT
int maze_obj::Check_if_valid(char *optarg)
{
    int tempoptarg;
    try
    {
        tempoptarg = stoi(optarg);
        if(tempoptarg < 5 || tempoptarg > 101)
        {
            cerr << "Unable to create maze, size must be between 5-101" << endl;
            exit(EXIT_FAILURE);
        }
    }
    catch(...)
    {
        cerr << "Invalid argument [" << optarg << "], argument must be an integrer between 5-101" << endl;
        exit(EXIT_FAILURE);
    }
    return stoi(optarg);
}
// OUTPUT MAZE TO CONSOL
void maze_obj::Print_maze()
{	
    for(size_t y = 0; y < my_maze->maze.size(); y++)
    {
        for(size_t x = 0; x < my_maze->maze[y].size(); x++)
        {
            cout << my_maze->maze[y][x];
        }
        cout << endl;
    }
}
// OUTPUT HELP TO CONSOL
void maze_obj::Print_usages()
{
	cout << "******************************************************************" << endl;
	cout << "*                Labyrinth - Created by josd1802                 *" << endl;
	cout << "******************************************************************" << endl;
	cout << "*                                                                *" << endl;
	cout << "* --version  ||  -v. Outputs the version.                        *" << endl;
	cout << "* --help     ||  -h. Outputs the valid arguments.                *" << endl;
	cout << "* (--size    ||  -s)N. Creates a maze with the size N. Odd int   *" << endl;
	cout << "* (--columns ||  -c)W. Creates a maze with the width W. Odd int  *" << endl;
	cout << "* (--rows    ||  -r)H. Creates a maze with the height H. Odd int *" << endl;
	cout << "* (--input   ||  -i)file. Use the file as input.                 *" << endl;
	cout << "* (--output  ||  -o)file. Use the file as output.                *" << endl;
    cout << "* --print    ||  -p. Outputs the maze or file content            *" << endl;
	cout << "* --check    ||  -f. Outputs Solution found, if there is one     *" << endl;
	cout << "*                                                                *" << endl;	
	cout << "******************************************************************" << endl;
}
