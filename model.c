#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "model.h"
#include "inputs.h"

#define NUMBER_OF_DEFAULT_GRIDS 8

#define SQUARE_SIZE 8
#define SELECTION_BAR_HEIGHT 30
#define COLUMN_COUNT (LCDHEIGHT/SQUARE_SIZE)
#define ROW_COUNT ((LCDWIDTH-SELECTION_BAR_HEIGHT)/SQUARE_SIZE)

int generation = 0;
bool shouldBegin = false;
bool shouldRun = true;
bool changingColourMode = false;
uint16_t colour = CYAN;

int gridTypeIndex = 0;
int* p;
int s;

bool holdGrid[ROW_COUNT][COLUMN_COUNT];
bool grid[ROW_COUNT][COLUMN_COUNT];

void displayHomeScreen(){
	set_foreground(CYAN);
	display_string("____ ____ _  _ _ _ _ ____ _   _ ____\n");
	display_string("|    |  | |\\ | | | | |__|  \\_/  [__ \n");
	display_string("|___ |__| | \\| |_|_| |  |   |   ___]\n"); 	
	
	display_string("____ ____ _  _ ____    ____ ____    _    _ ____ ____\n");
	display_string("| __ |__| |\\/| |___    |  | |___    |    | |___ |___\n");
	display_string("|__] |  | |  | |___    |__| |       |___ | |    |___\n"); 
	set_foreground(WHITE);
	
	display_string("\n");
	
	display_string("Press ");
	display_green_string("CENTER");
	display_string(" to start/stop the game at any point\n\n\n");
	
	display_string("Press ");
	display_green_string("LEFT");
	display_string(" and ");
	display_green_string("RIGHT");
	display_string(" to move between different preset grids\n\n\n");
	
	display_string("Press ");
	display_green_string("UP");
	display_string(" to enable colour changing mode\n\n\n");
	
	display_string("Press ");
	display_green_string("DOWN");
	display_string(" to disable colour changing mode\n\n\n");
	
	display_string("Press CENTER to proceed");
	
	set_background(GREY);
}

void display_green_string(char *str){
	set_foreground(GREEN);
	display_string(str);
	set_foreground(WHITE);
}

void beginRounds(){
	
	while(true){
		if(!shouldBegin || !shouldRun) continue;
		cli();
		runRound();
		displayCurrentGrid();
		sei();
	}
}

void createGrid(){
	for(int x = 0; x < COLUMN_COUNT; x++){
		for(int y = 0; y < ROW_COUNT; y++){
			rectangle shape = {x * SQUARE_SIZE, x*SQUARE_SIZE+SQUARE_SIZE-1, y * SQUARE_SIZE, y*SQUARE_SIZE+SQUARE_SIZE-1};
			if(isPointInArray(x,y)){
				fill_rectangle(shape,colour);
				grid[y][x] = true;
			}
			else{
				fill_rectangle(shape,BLACK);
				grid[y][x] = false;
			}
		}
	}
}

void createRandomGrid(int odds){
	for(int x = 0; x < COLUMN_COUNT; x++){
		for(int y = 0; y < ROW_COUNT; y++){
			rectangle shape = {x * SQUARE_SIZE, x*SQUARE_SIZE+SQUARE_SIZE-1, y * SQUARE_SIZE, y*SQUARE_SIZE+SQUARE_SIZE-1};
			if(rand() % odds == 1){
				fill_rectangle(shape,colour);
				grid[y][x] = true;
			}
			else{
				fill_rectangle(shape,BLACK);
				grid[y][x] = false;
			}
		}
	}
}

void createGridOptionsBar(){
	rectangle shape = {0, LCDHEIGHT, LCDWIDTH-SELECTION_BAR_HEIGHT, LCDWIDTH};
	fill_rectangle(shape,GREY);
	if(gridTypeIndex > 0){
		display_string_xy("<", 5, (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
	}
	if(gridTypeIndex < NUMBER_OF_DEFAULT_GRIDS - 1){
		display_string_xy(">", LCDHEIGHT - 9, (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
	}
}

void setNewGrid(){
	shouldRun = false;
	generation = 0;
	createGridOptionsBar();
	switch (gridTypeIndex){
		case(2):
			display_string_xy("Exploder", (LCDHEIGHT/2)-(8*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			p = &exploder[0][0];
			s = 12;
			createGrid();
			break;
		case(1):
			display_string_xy("Gun", (LCDHEIGHT/2)-(3*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			p = &gun[0][0];
			s = 36;
			createGrid();
			break;
		case(0):
			display_string_xy("RIP John Conway: 1937 - 2020", (LCDHEIGHT/2)-(28*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			p = &rip[0][0];
			s = 49;
			createGrid();
			break;
		case(3):
			display_string_xy("Spaceship", (LCDHEIGHT/2)-(9*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			p = &spaceship[0][0];
			s = 9;
			createGrid();
			break;
		case(4):
			display_string_xy("Ring of Fire", (LCDHEIGHT/2)-(12*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			p = &firering[0][0];
			s = 42;
			createGrid();
			break;
		case(5):
			display_string_xy("Random 1/5", (LCDHEIGHT/2)-(10*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			createRandomGrid(5);
			break;
		case(6):
			display_string_xy("Random 1/3", (LCDHEIGHT/2)-(10*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			createRandomGrid(3);
			break;
		case(7):
			display_string_xy("Random 1/2", (LCDHEIGHT/2)-(10*2), (LCDWIDTH-(SELECTION_BAR_HEIGHT/2)-3));
			createRandomGrid(2);
			break;
	}
}

bool isPointInArray(int x, int y){
	for(int i = 0; i < s*2; i += 2){
		if(*(p + i) == x && *(p + i + 1) == y){
			return true;
		}
	}
	return false;
}

void displayCurrentGrid(void){
	for(int x = 0; x < COLUMN_COUNT; x++){
		for(int y = 0; y < ROW_COUNT; y++){
			if(holdGrid[y][x] != grid[y][x]){
				rectangle shape = {x * SQUARE_SIZE, x*SQUARE_SIZE+SQUARE_SIZE-1, y * SQUARE_SIZE, y*SQUARE_SIZE+SQUARE_SIZE-1};
				fill_rectangle(shape, holdGrid[y][x] ? colour : BLACK);
			}
		}
	}
	
	char str[18];
	sprintf(str, "Generation: %d", generation);
	display_string_xy(str, 0, 0);
	memcpy(grid,holdGrid,ROW_COUNT*COLUMN_COUNT);
}

/*
void runRound(void){
	generation++;
	for(int x = 0; x < COLUMN_COUNT; x++){
		for(int y = 0; y < ROW_COUNT; y++){
			updateHoldGridValue(x,y,0);
		}
	}
}
*/

void runRound(void){
	generation++;
	//While this code looks ugly, it was made this way in an attempt to improve the speed of rounds.
	updateHoldGridValue(0, 0, 3);
	updateHoldGridValue(0, ROW_COUNT-1, 3);
	updateHoldGridValue(COLUMN_COUNT-1, 0, 3);
	updateHoldGridValue(COLUMN_COUNT-1, ROW_COUNT-1, 3);
	
	for(int y = 1; y < ROW_COUNT-1; y++){
		updateHoldGridValue(0,y,5);
		updateHoldGridValue(COLUMN_COUNT-1,y,5);
	}
	for(int x = 1; x < COLUMN_COUNT-1; x++){
		updateHoldGridValue(x,0,5);
		updateHoldGridValue(x,ROW_COUNT-1,5);
	}
	
	for(int x = 1; x < COLUMN_COUNT-1; x++){
		for(int y = 1; y < ROW_COUNT-1; y++){
			updateHoldGridValue(x,y,8);
		}
	}
}

void updateHoldGridValue(int x, int y, int neighbourCount){
	holdGrid[y][x] = determineIfAliveOrDead(x,y,getLivingNeighbourCount(x,y,neighbourCount));
}

bool determineIfAliveOrDead(int x, int y, int count){
	return grid[y][x] ? count == 2 || count == 3 : count == 3;
}

int getLivingNeighbourCount(int x, int y, int iterationsLeft){
	int count = 0;
	for(int xChange = x-1; xChange <= x+1; xChange++){
		for(int yChange = y-1; yChange <= y+1; yChange++){
			if(!((xChange == x && yChange == y) || xChange < 0 || xChange >= COLUMN_COUNT || yChange < 0 || yChange >= ROW_COUNT)){
				count += grid[yChange][xChange] ? 1 : 0;
				iterationsLeft--;
			}
			
			if(count == 4 || count + iterationsLeft == 1){
				return count;
			}
		}
	}
	
	return count;
}

void init(void) {
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
    init_lcd();
	initControls();

	
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = 7;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12) | (1 << CS10);  
	TIMSK1 |= _BV(OCIE1A);
}

void main(void) {
	srand (time(NULL));
	cli();
	init();
	sei();
	displayHomeScreen();
	beginRounds();
}

ISR( TIMER1_COMPA_vect ) {
	if(shouldBegin){
		if(shouldRun && changingColourMode){
			colour += 1000 % 65535;
		}
		if(centerPressed()){
			shouldRun = !shouldRun;
		}
		if(leftPressed() && gridTypeIndex > 0){
			gridTypeIndex--;
			setNewGrid();
		}
		if(rightPressed() && gridTypeIndex < NUMBER_OF_DEFAULT_GRIDS-1){
			gridTypeIndex++;
			setNewGrid();
		}
		if(upPressed()){
			changingColourMode = true;
		}
		if(downPressed()){
			changingColourMode = false;
		}
	}
	else{
		if(centerPressed()){
			clear_screen();
			setNewGrid();
			shouldBegin = true;
		}
	}
}