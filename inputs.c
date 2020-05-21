#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "inputs.h"

bool centerDown = false;
bool leftDown = false;
bool rightDown = false;

void initControls(){
	DDRE &= ~_BV(BC);  
	PORTE |= _BV(BC);
	DDRC &= ~(_BV(BN)|_BV(BE)|_BV(BS)|_BV(BW));  
	PORTC |= (_BV(BN)|_BV(BE)|_BV(BS)|_BV(BW)); 
}


bool centerPressed(){
	if ((~PINE & _BV(BC)) && !centerDown) {
		centerDown = true;
		return true;
	} else if ((PINE & _BV(BC)) && centerDown){
		centerDown = false;
	}
	return false;
}

bool upPressed(){
	return (~PINC & _BV(BN));
}

bool downPressed(){
	return (~PINC & _BV(BS));
}

bool leftPressed(){
	if ((~PINC & _BV(BW)) && !leftDown) {
		leftDown = true;
		return true;
	} else if ((PINC & _BV(BW)) && leftDown){
		leftDown = false;
	}
	return false;
}

bool rightPressed(){
	if ((~PINC & _BV(BE)) && !rightDown) {
		rightDown = true;
		return true;
	} else if ((PINC & _BV(BE)) && rightDown){
		rightDown = false;
	}
	return false;
}
