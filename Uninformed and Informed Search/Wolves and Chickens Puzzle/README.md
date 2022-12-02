Description
In the wolves and chickens puzzle, C chickens and W wolves must cross from the right bank of a river to the left bank using a boat. The boat holds a maximum of two animals. In addition, the boat cannot cross the river by itself and must have at least one animal on board to drive it. This problem seems simple except for the following key constraint. If there are chickens present on a bank, there cannot be more wolves than chickens, otherwise the wolves will eat the chickens.

In this assignment, you will write code to solve the chickens and wolves puzzle using uninformed and informed search algorithms. The algorithms you will implement are breadth-first search, depth-first search, iterative deepening depth-first search and A-star search. Your code will print out the states along the solution path from the start state to the goal state. If no such path exists, your code must print out a no solution found message. In addition, your code must also print out the number of search nodes expanded. You will need to know this number to fill out a table comparing the different algorithms.

File Format
The state of a game is represented by two comma-separated lines in a file. The first line stores the number of chickens, the number of wolves, and the number of boats on the left bank. The second line stores the number of chickens, the number of wolves, and the number of boats on the right bank. Since there is only one boat in the puzzle, the number of boats could be interpreted as a Boolean flag indicating the presence of the boat.

For example, the initial state

0,0,0
3,3,1

represents 3 chickens, 3 wolves and the boat being on the right bank of the river and nothing on the left bank.

Your program will take as input a starting state file and a goal state file. Your code must be able to read the start and goal state files.

Requirements
You may the following programming languages: Java, C, C++, Python. If you use any other programming language, it must be available on the ENGR systems and you must contact the Teaching Assistants for approval. Your code should take the following command line arguments:
< initial state file > < goal state file > < mode > < output file >

The mode argument is either:

bfs (for breadth-first search)
dfs (for depth-first search)
iddfs (for iterative deepening depth-first search)
astar (for A-Star search below)
I. Uninformed Search
You will implement Breadth-First Search, Depth-First Search and Iterative-Deepening Depth First Search. Use the pseudocode for the GraphSearch function in either the slides or the textbook to help you design the algorithm. If you don't follow the GraphSearch pseudocode, your algorithms may not work properly.

There are specific requirements regarding the uniformed search algorithms that you must follow in this assignment:

In order to generate successors for a state, you need to do an action, generate the next state, then add that state to a list of successors. To make things consistent for grading, we require you to generate successor states using the following order on the actions:
Put one chicken in the boat
Put two chickens in the boat
Put one wolf in the boat
Put one wolf and one chicken in the boat
Put two wolves in the boat
If any of these actions are not valid for the current state, ignore that action and move on to the next one. To illustrate the successor generation, suppose we start with nothing on the left bank and 3 chickens, 3 wolves and 1 boat on the right bank. The successor states will be:

Left Bank: 0 chickens, 1 wolf, 1 boat Right Bank: 3 chickens, 2 wolves, 0 boat
Left Bank: 1 chicken, 1 wolf, 1 boat Right Bank: 2 chickens, 2 wolves, 0 boat
Left Bank: 0 chickens, 2 wolves, 1 boat Right Bank: 3 chickens, 1 wolf, 0 boat
Notice that the actions for putting one chicken in the boat and putting two chickens in the boat are invalid because it would result in more wolves on the right bank than chickens.

You will need a counter for the number of nodes expanded. Update this counter after you remove the node from the frontier and just before you call the expand function (note that if it is a goal node, you don't need to expand it and hence don't count the goal node as an expanded node). At the end of the graph search, print out the number of nodes that were expanded.
You need to print the solution path which shows how to move the chickens and wolves from the start state to the goal state. Please print this to the screen and to an output file. Make sure it is in the right order!
II. Informed Search
In this part, you will implement A-star search. As with uninformed search, there are specific requirements that you must follow:

When generating successors for a state, do it in the same order as for the Uninformed Search algorithms above.
Keep track of the number of nodes expanded in the same way as specified for the Uninformed Search above.
You will need a priority queue. You may use built-in code from your programming language (eg. Java) for the priority queue.
You will need a heuristic for A-star search. Pick one but make sure it is admissible.
You need to print the solution path which shows how to move from the start state to the goal state. Please print this to the screen and to an output file. Make sure it is in the right order!



Use the following command to compile the program 

g++ -std=c++11 prog.cpp -o game

The program take the following command line arguments

./game < initial state file > < goal state file > < mode > < output file >

