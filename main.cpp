#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdlib.h>
#include <algorithm>
#include <conio.h>
#include <queue>
#include <vector>
#include <map>
#include <chrono>


using namespace std;

struct cor { // for coordiantes
	int y;
	int x;
};

class Tile {  // object for each tile
public:
	int val;
	cor dis;
	Tile(int n, int x, int y)
	{
		dis.y = y;
		dis.x = x;
		val = n;
	}
	Tile()
	{
		dis.y = 0;
		dis.x = 0;
		val = 0;
	}
	void changeCor(int col, int row)
	{
		dis.y = col;
		dis.x = row;
	}
	void changeVal(int c)
	{
		val = c;
	}

};

class Grid { // grid of tiles
public:
	cor cord;
	vector<vector <Tile*>> grid;
	Tile* blank = nullptr;

	void initializeGrid(int y, int x)
	{
		cord.y = y;
		cord.x = x;
		int count = 0;
		for (int i = 0; i < cord.y; i++)
		{
			grid.push_back(vector<Tile*>());
			for (int j = 0; j < cord.x; j++)
			{
				count++;

				if (count < (y * x))
				{
					Tile* tile = new Tile(count, j, i);
					Tile* buffer = tile;
					grid[i].push_back(buffer);

					//delete tile;
				}
				else {
					Tile* temp = new Tile();
					blank = temp;

					grid[i].push_back(blank);
					blank->changeCor(j, i);

					//delete temp;
				}
			}
		}
	}

	Grid()
	{
		cord.y = 3; cord.x = 3;
	}

	Grid(int y, int x)
	{
		initializeGrid(y, x);
	}

	void showGrid()
	{
		//refresh();
		for (int y = 0; y < grid.size(); y++)
		{
			for (int x = 0; x < grid[y].size(); x++)
			{

				cout << grid[y][x]->val << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

Grid goal_state()
{
	int max_y = 3;
	int max_x = 3;
	Grid goal(max_y, max_x);

	return goal;
}

Grid doable()
{
	// {{*, 1, 2}, {4, 5, 3}, {7, 8, 6}}
	int max_y = 3;
	int max_x = 3;
	Tile* buffer = new Tile;
	Tile* bl = buffer;
	bl->changeCor(0, 0);
	bl->changeVal(0);

	Grid m;

	vector<vector<Tile*>> med{ {bl, new Tile(1, 1, 0), new Tile(2, 2, 0) },
						{new Tile(4, 0, 1),new Tile(5, 1, 1),new Tile(3, 2, 1)},
						{new Tile(7, 0, 2),new Tile(8, 1, 2),new Tile(6, 2, 2)} };

	m.grid = med;
	m.blank = bl;

	//delete buffer;
	return m;
}

Grid medium()
{
	// {{1, 2, 3,}, {4, 5, 8}, {6, 7, 0}} 
	int max_y = 3;
	int max_x = 3;
	Tile* bl = new Tile;
	bl->changeCor(2, 2);
	bl->changeVal(0);

	Grid m;

	vector<vector<Tile*>> med{ {new Tile(1, 0, 0), new Tile(2, 1, 0), new Tile(3, 2, 0) },
						{new Tile(4, 0, 1), new Tile(5, 1, 1) ,new Tile(8, 2, 1)},
						{new Tile(6, 0, 2),new Tile(7, 1, 2), bl } };

	m.grid = med;
	m.blank = bl;

	return m;
}

Grid ohboy()
{
	// {{8, 7, 1}, {6, *, 2}, {5, 4, 3}} 
	int max_y = 3;
	int max_x = 3;
	Tile* bl = new Tile;
	bl->changeCor(1, 1);
	bl->changeVal(0);

	Grid m;

	vector<vector<Tile*>> med{ {new Tile(8, 0, 0), new Tile(7, 1, 0), new Tile(1, 2, 0) },
						{new Tile(6, 0, 1), bl ,new Tile(2, 2, 1)},
						{new Tile(5, 0, 2),new Tile(4, 1, 2),new Tile(3, 2, 2)} };

	m.grid = med;
	m.blank = bl;

	return m;

}

Grid customGrid()
{
	int max_y = 3;
	int max_x = 3;
	const int gridSize = 9;
	int userGridVal[gridSize];
	cout << "Enter your puzzle, use a zero to represent the blank.\n"
		<< "Enter the first row, use space or tabs between numbers.\n"
		<< "Enter the second row, use space pr tabs between numbers.\n"
		<< "Enter the third arow, use space or tabs between numbers.\n\n";
	Grid m;
	for (int i = 0; i < gridSize; i++)
	{
		std::cin >> userGridVal[i];
	}

	vector<vector<Tile*>> tileGrid;

	int count = 0;

	for (int y = 0; y < max_y; y++)
	{
		tileGrid.push_back(vector<Tile*>());

		for (int x = 0; x < max_x; x++)
		{

			if (userGridVal[count] == 0)
			{
				Tile* bl = new Tile(0, y, x);
				tileGrid[y].push_back(bl);
				m.blank = bl;
				bl->changeCor(y, x);
			}
			else
			{
				tileGrid[y].push_back(new Tile(userGridVal[count], x, y));

			}

			count++;
		}
	}
	m.grid = tileGrid;
	return m;
}

class Node {
public:
	vector<char> state;
	int misplaced = 0;
	int distance = 0;
	int depth = 0;
	int h; //heuristic
	Node* Parent = nullptr;
	Grid currentGrid;
	int y;
	int x;
	vector<Node*> children;
	void setHeuristic(int h)
	{
		this->h = depth + h;
	}
	void setState(Grid g)
	{
		for (int y = 0; y < g.cord.y; y++)
		{
			for (int x = 0; x < g.cord.x; x++)
			{
				state.push_back(g.grid[y][x]->val);
			}
		}
	}
	void calcStateval(Grid i, Grid g) // misplaced tile
	{
		//misplaced = (i.cord.y * i.cord.x);
		for (int y = 0; y < i.cord.y; y++)
		{
			for (int x = 0; x < i.cord.x; x++)
			{

				if (i.grid[y][x]->val != g.grid[y][x]->val && (i.grid[y][x]->val != 0))
				{
					misplaced++;
				}
			}
		}
	}
	void calcDistance(Grid i)
	{
		int temp_x = 0;
		int temp_y = 0;
		map<int, cor> goalMap;
		cor one = { 0,0 };
		cor two = { 0,1 };
		cor three = { 0, 2 };
		cor four = { 1, 0 };
		cor five = { 1, 1 };
		cor six = { 1, 2 };
		cor seven = { 2, 0 };
		cor eight = { 2, 1 };
		goalMap[1] = one;
		goalMap[2] = two;
		goalMap[3] = three;
		goalMap[4] = four;
		goalMap[5] = five;
		goalMap[6] = six;
		goalMap[7] = seven;
		goalMap[8] = eight;


		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (i.grid[y][x]->val != 0)
				{
					temp_y = abs(goalMap[i.grid[y][x]->val].y - i.grid[y][x]->dis.y);
					temp_x = abs(goalMap[i.grid[y][x]->val].x - i.grid[y][x]->dis.x);
					distance += (temp_x + temp_y);
				}

			}
		}
	}
	void showState()
	{
		for (int i = 0; i < state.size(); i++)
		{
			cout << state[i] << " ";
		}
		cout << endl;
	}
	void showVal()
	{
		cout << depth << endl;
	}
	Node(Grid i, Grid g)
	{
		setState(i);
		calcStateval(i, g);
		calcDistance(i);
		currentGrid = i;


	}
	void setBlank(int y, int x)
	{
		this->y = y;
		this->x = x;
	}
};

class Problem {
public:

	Grid initial;
	Grid win;
	Node* root;
	Problem(Grid s, Grid w)
	{
		initial = s;
		win = w;
		root = new Node(initial, win);
	}

	bool move_up()
	{
		int y = initial.blank->dis.y;
		int x = initial.blank->dis.x;
		if (y > 0)
		{
			initial.blank->changeCor(y - 1, x); // hard swap coordiantes
			initial.grid[y - 1][x]->changeCor(y, x);
			swap(initial.grid[y][x], initial.grid[y - 1][x]);
		}
		//initial.showGrid();
		//cout << "Moved up\n\n";
		return winCheck();
	}

	bool move_down()
	{
		int y = initial.blank->dis.y;
		int x = initial.blank->dis.x;

		if (y < initial.cord.y - 1)
		{
			initial.blank->changeCor(y + 1, x); // hard swap coordiantes
			initial.grid[y + 1][x]->changeCor(y, x);
			swap(initial.grid[y][x], initial.grid[y + 1][x]);

		}

		//initial.showGrid();
		//cout << "Moved down\n\n";
		return winCheck();
	}

	bool move_left()
	{
		int y = initial.blank->dis.y;
		int x = initial.blank->dis.x;

		if (x > 0)
		{

			initial.blank->changeCor(y, x - 1); // hard saswap coordiantes
			initial.grid[y][x - 1]->changeCor(y, x);

			swap(initial.grid[y][x], initial.grid[y][x - 1]);

		}

		//initial.showGrid();
		//cout << "Moved left\n\n";
		return winCheck();
	}

	bool move_right()
	{
		int y = initial.blank->dis.y;
		int x = initial.blank->dis.x;

		if (x < initial.cord.x - 1)
		{

			initial.blank->changeCor(y, x + 1); // hard swap coordiantes
			initial.grid[y][x + 1]->changeCor(y, x);

			swap(initial.grid[y][x], initial.grid[y][x + 1]);
		}


		//initial.showGrid();
		//cout << "Moved right\n\n";
		return winCheck();
	}

	bool winCheck()
	{
		bool youWon = true;


		for (int y = 0; y < initial.cord.y; y++)
		{
			for (int x = 0; x < initial.cord.x; x++)
			{
				if (initial.grid[y][x]->val != win.grid[y][x]->val)
				{

					youWon = false;
				}
			}
		}

		return youWon;
	}

	bool gridCheck(Grid i, Grid g)
	{

		for (int y = 0; y < initial.cord.y; y++)
		{
			for (int x = 0; x < initial.cord.x; x++)
			{
				if (i.grid[y][x]->val != g.grid[y][x]->val)
				{
					return false;
				}
			}
		}

		return true;
	}
};

Grid initial_state() // the initial state is derived from the goal state to ensure that it is solvable
{
	Problem t(goal_state(), goal_state());
	t.move_left();
	t.move_left();

	return t.initial;
}

int game(Problem p) // function to run problem with user input
{
	system("cls");
	p.initial.showGrid();

	bool win = false;

	char press;
	do
	{
		press = _getch();


		switch (press)
		{

		case 'w':
		{
			p.move_up();
			win = p.winCheck();
			system("cls");
			p.initial.showGrid();
		}
		break;

		case 's':
		{
			p.move_down();
			win = p.winCheck();
			system("cls");
			p.initial.showGrid();
		}
		break;
		case 'a':
		{
			p.move_left();
			win = p.winCheck();
			system("cls");
			p.initial.showGrid();
		}
		break;
		case 'd':
		{
			p.move_right();
			win = p.winCheck();
			system("cls");
			p.initial.showGrid();

		}
		break;
		}

	} while (win == false);

	return true;
}

bool dequeChecker(deque<Node*> d, Node* n, Problem p)
{
	if (d.empty())
		return false;

	for (int i = 0; i < d.size(); i++)
	{
		if (p.gridCheck(d.at(i)->currentGrid, n->currentGrid))
			return true;
	}
	return false;
}

Node* astarH(deque<Node*>& d)
{
	int astar = 2147483647;
	Node* temp = nullptr;
	int popIndex = 0;
	for (int i = 0; i < d.size(); i++)
	{
		int curr = d.at(i)->misplaced + d.at(i)->depth;
		if (curr < astar)
		{
			astar = curr;
			temp = d.at(i);
			popIndex = i;
			//cout << "the new min distance is: " << distance << endl;
		}
	}
	d.erase(d.begin() + popIndex);
	return temp;
}

Node* astarD(deque<Node*>& d)
{
	int astar = 2147483647;
	Node* temp = nullptr;
	int popIndex = 0;

	for (int i = 0; i < d.size(); i++)
	{
		int curr = d.at(i)->distance + d.at(i)->depth;
		if (curr < astar)
		{
			astar = curr;
			temp = d.at(i);
			popIndex = i;
			//cout << "the new min distance is: " << distance << endl;
		}
	}
	d.erase(d.begin() + popIndex);
	return temp;
}

bool search(Problem p, int option)
{
	int depth = 0;
	deque<Node*> frontier;
	deque<Node*> explored;
	Node* current = p.root;
	bool win = false;
	frontier.push_back(current);
	current->y = p.initial.blank->dis.y; // keep track of blank after problem updates
	current->x = p.initial.blank->dis.x;
	int ySize = 2; // keeps track of y and x bounds
	int xSize = 2; //
	int numNode = 0;

	do
	{
		numNode++;

		if (frontier.empty())
			return false;
		std::cout << "Expanding node\n";

		if (p.winCheck()) // solves the trivial case
		{
			win = true;
			break;
		}
		switch (option)
		{
		case 1:
		{
			explored.push_back(frontier.front());
			frontier.pop_front();
		}
		break;
		case 2:
			explored.push_back(astarH(frontier));
			break;
		case 3:
			explored.push_back(astarD(frontier));
			break;
		}

		depth = explored.back()->depth;
		depth++;
		cout << "node: " << numNode << endl;
		cout << "depth: " << depth << endl;

		p.initial = explored.back()->currentGrid;
		p.initial.blank->changeCor(explored.back()->y, explored.back()->x);

		p.initial.showGrid();

		int y = p.initial.blank->dis.y; // keeps track of y and x values for blank
		int x = p.initial.blank->dis.x; //

		if (p.winCheck())
		{
			win = true;
			break;
		}

		if (y > 0) // up bounds
		{
			p.move_up();
			Node* temp = new Node(p.initial, p.win);
			if (!dequeChecker(frontier, temp, p) && !dequeChecker(explored, temp, p))
			{
				//	std::cout << "up added to the frontier\n";
				temp->depth = depth;
				temp->setBlank(p.initial.blank->dis.y, p.initial.blank->dis.x);
				frontier.push_back(temp);
			}

			p.move_down();
		}

		if (y < ySize) //down bounds
		{
			p.move_down();
			Node* temp = new Node(p.initial, p.win);
			if (!dequeChecker(frontier, temp, p) && !dequeChecker(explored, temp, p))
			{
				//	std::cout << "down added to the frontier\n";
				temp->depth = depth;
				temp->setBlank(p.initial.blank->dis.y, p.initial.blank->dis.x);
				frontier.push_back(temp);
			}

			p.move_up();
		}

		if (x > 0) //left bounds
		{
			p.move_left();
			Node* temp = new Node(p.initial, p.win);
			if (!dequeChecker(frontier, temp, p) && !dequeChecker(explored, temp, p))
			{
				temp->setBlank(p.initial.blank->dis.y, p.initial.blank->dis.x);
				temp->depth = depth;
				//	std::cout << "left added to the frontier\n";
				frontier.push_back(temp);
			}
			p.move_right();
		}

		if (x < xSize) // right bounds
		{
			p.move_right();
			Node* temp = new Node(p.initial, p.win);
			if (!dequeChecker(frontier, temp, p) && !dequeChecker(explored, temp, p))
			{
				temp->setBlank(p.initial.blank->dis.y, p.initial.blank->dis.x);
				temp->depth = depth;
				// cout << "right added to the frontier\n";
				frontier.push_back(temp);
			}
			p.move_left();
		}
	} while (!win);
	return win;
}

int main()
{
	Grid userGrid;
	int userinput1 = 0;
	int userinput2 = 0;

	cout << "Welcome to 862-192-127 8 puzzle solver.\n"
		<< "Type \"1\" to use default puzzle, or \"2\" to enter your own puzzle.\n";
	std::cin >> userinput1;

	if (userinput1 == 1)
	{
		int userGridChoice = 0;
		cout << "Choose difficulty.\n";
		cout << "1) easy\n2) doable\n3) medium\n4) ohboy\n\n";
		cin >> userGridChoice;

		switch (userGridChoice)
		{
		case 1:
			userGrid = initial_state();
			break;
		case 2:
			userGrid = doable();
			break;
		case 3:
			userGrid = medium();
			break;
		case 4:
			userGrid = ohboy();
			break;
		}
	}
	else if (userinput1 == 2)
	{
		userGrid = customGrid();
	}
	Problem myP(userGrid, goal_state());

	cout << "Enter your choice of algorithm.\n"
		<< "1) Uniform Cost Search\n"
		<< "2) A* with the Misplaced Tile heuristic\n"
		<< "3) A* with Euclidean distance heuristic\n"
		<< "4) Game mode\n\n";
	std::cin >> userinput2;

	auto begin = chrono::high_resolution_clock::now();
	switch (userinput2)
	{
	case 1:
	{
		if (search(myP, 1))
			cout << "Goal!!!\n";
		else cout << "Failure!!!\n";
	}
	break;
	case 2:
	{
		if (search(myP, 2))
			cout << "Goal!!!\n";
		else cout << "Failure!!!\n";
	}
	break;
	case 3:
	{
		if (search(myP, 3))
			cout << "Goal!!!\n";
		else cout << "Failure!!!\n";
	}
	break;
	case 4:
	{
		if (game(myP))
			cout << "You Win!!!\n";
	}
	break;
	}

	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);

	cout << "Elapsed time in nanoseconds: " << elapsed.count() * 1e-9 << endl;
	return 0;
}