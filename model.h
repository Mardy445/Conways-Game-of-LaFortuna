//Initialises the switches and the ISR
void init(void);

//Displays a home screen with information on controls
void displayHomeScreen(void);

//Displays a green string and then reverts the text colour back to white
void display_green_string(char *str);

//Begins the infinite loop that will allow you to move forward generations for a given grid
void beginRounds(void);

//Create a new initial grid based on what the user has currently selected
void createGrid(void);

//Creates a random grid, where odds represents the chance a square will start off alive (IE odds of 5 means a 1 in 5 chance)
void createRandomGrid(int odds);

//Creates the options bar seen at the bottom of the screen
void createGridOptionsBar(void);

//Displays the current grid after any changes have been made
void displayCurrentGrid(void);

//Sets the new grid when a user moves the options bar to the left or right
void setNewGrid(void);

//Checks if a point is in a pre set array. Used for loading in grids
bool isPointInArray(int x, int y);

//Runs a round on the current grid, moving it forward 1 generation
void runRound(void);

//Updates a grid point based on its neighbour count
void updateHoldGridValue(int x, int y, int neighbourCount);

//Based on the neighbour count, returns true if alive or false if dead
bool determineIfAliveOrDead(int x, int y, int count);

//Gets the current living neightbour count of a point  on the grid
int getLivingNeighbourCount(int x, int y, int iterationsLeft);

int gun[36][2] = {
	{1,5}, {1,6},{2,5},{2,6}, 
	{11,5},{11,6},{11,7},{12,4},{12,8},{13,3},{13,9},{14,3},{14,9},{15,6},{16,4},{16,8},{17,5},{17,6},{17,7},{18,6},
	{21,3},{21,4},{21,5},{22,3},{22,4},{22,5},{23,2},{23,6},{25,1},{25,2},{25,6},{25,7},
	{35,3},{35,4},{36,3},{36,4}
	}; 
	
	
int exploder[12][2] = {
	{18,10},{18,11},{18,12},{18,13},{18,14},
	{20,10},{20,14},
	{22,10},{22,11},{22,12},{22,13},{22,14}
	};
	
int rip[49][2] = {
	{8,20},{9,21},{10,16},{10,17},{10,18},{10,20},{10,23},{10,24},{11,16},{11,19},{11,20},{11,21},{11,22},{12,16},{12,17},{12,18},{12,20},{12,23},{12,24},{13,21},{14,22},
	{20,20},{20,21},{20,22},{20,23},{20,24},{20,25},{21,20},{21,23},{22,20},{22,21},{22,22},{22,24},{22,25},
	{24,20},{24,22},{24,23},{24,24},{24,25},
	{26,20},{26,21},{26,22},{26,23},{26,24},{26,25},{27,20},{27,23},{28,21},{28,22}
	};
	
int spaceship[9][2] = {
	{0,20},{0,22},{1,19},{2,19},{3,19},{3,22},{4,19},{4,20},{4,21}
	};

int firering[42][2] = {
	{15,20},
	{16,18},{16,20},{16,22},
	{17,16},{17,18},{17,20},{17,22},{17,24},
	{18,17},{18,18},{18,20},{18,22},{18,23},
	{19,15},{19,20},{19,25},
	{20,16},{20,17},{20,18},{20,19},{20,21},{20,22},{20,23},{20,24},
	{21,15},{21,20},{21,25},
	{22,17},{22,18},{22,20},{22,22},{22,23},
	{23,16},{23,18},{23,20},{23,22},{23,24},
	{24,18},{24,20},{24,22},
	{25,20}
};
	
