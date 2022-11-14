
#include <stdint.h>   
#include <pic32mx.h> 
#include <stdbool.h>
#include <stdlib.h> 



int seed = 0;
int seeding = 1;
int timeoutcount = 0;
int scene = 0;
int speed = 0;
int score = 0;
int highscore = 0;

//stdout pointer till null för att kunna använda funktioner i stdlib
void *stdout = (void *) 0;

void user_isr( void )
{
  return;
}

void init( void )
{
	TRISECLR = 0xff; 
	TRISDSET = 0xfe0; // TRISDSET 
	T2CON = 0x70; //1/256 prescale 
	PR2 = (80000000/256)/10; //timeout period 0,1s
	TMR2 = 0;	//reset timer
	T2CONSET = 0x8000; // starta timer
  return;
}


void gameover(void) {
	timeoutcount++;
	
	scene = 2;
	
	//visa GAME OVER text
	if(timeoutcount<5 || (timeoutcount>10 && timeoutcount<17) || (timeoutcount>20 && timeoutcount<27)|| (timeoutcount>30 && timeoutcount<37)||timeoutcount>40){
		display_string(0, "GAME OVER!");
		display_string(1, "SCORE: ");
		display_string(2, itoaconv(score));
		if (score>highscore) {
		display_string(3, "NEW HIGH SCORE!");
	} else {
		display_string(3, "");
	}
	}else{
		display_string(0, "");
		display_string(1, "");
		display_string(2, "");
		display_string(3, "");
	}
	
	display_update();
	
	//Efter 5 sekunder återvänder spelet till main menu
	if (timeoutcount==50) {
		if (score>highscore) {
		highscore = score;
		}
		score = 0;
		timeoutcount = 0;
		
		//återvänd till main menu
		scene=0;
	}
}


void game(void) {
		timeoutcount++;
		
		//ändra hastigheten av de fallande blocken beroende på switcharna
		if (timeoutcount == 16-speed) {
			fall();
			timeoutcount=0;
		}
	
		if(getbtns()>0){
			
		//få block att falla snabbare,BTN1
		if ((getbtns() & 0b1000) == 0b1000) {
			fall();
		}
		
		//rotera block,BTN2
		if((getbtns() & 0b001) == 0b001){
			rotate();
		}
		//gå åt höger, BTN3
		if((getbtns() & 0b010) == 0b010){
			moveRight();
		}
		
		//gå åt vänster, BTN4
		if((getbtns() & 0b100) == 0b100){
			moveLeft();
		}	
	}
	
		//rita alla borders och spel text
		drawBorder();
		drawForm();
		drawNextForm();
		drawFallenblocks();
		drawGameText();
		
		int scorewidth = intWidth(score);
		printScore(scorewidth, score);
		
		int diffwidth = intWidth(speed);
		drawSpeed(diffwidth, speed);
		
		display_image();
}

void showHighscore(void) {
	
		display_string(0, "HIGH SCORE:");
		display_string(1, itoaconv(highscore));
		display_string(2, "");
		display_string(3, "BTN3: MAIN MENU");
		display_update();
		
		//BTN3 gå tillbaka till main meny
		if((getbtns() & 0b010) == 0b010) {
			scene=0;
		}
}

void menu(void) {

		//BTN2 highscore
		if((getbtns() & 0b001) == 0b001) {
			scene=3;
		}
		
		//BTN1 starta spel
		if((getbtns() & 0b1000) == 0b1000){
			if (seeding == 1) {
			seeding = 0;
			srand(seed);
			}
			
			speed = getsw();
			
			//skapa första blocket
			int randomstartshape = rand() % 7;
			
			createForm(randomstartshape);
			
			scene=1; //starta spel
		}
		
		display_string(0, "TETRIS");
		display_string(1, "");
		display_string(2, "BTN2: HIGH SCORE");
		display_string(3, "BTN1: PLAY");
		display_update();
}

/* Kallas iterativt från main.c */
void iterate( void )
{
	
	
	if(IFS(0) & 0x100){	//kolla för en timeout
	IFSCLR(0) = 0x100;	//rensa timeout flag
	switch (scene) {
		case 0:
		menu();
		break;
		case 1:
		game();
		break;
		case 2:
		gameover();
		break;
		case 3:
		showHighscore();
		break;
	}
  }
  
}
