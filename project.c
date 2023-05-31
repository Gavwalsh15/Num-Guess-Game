/*
Computer Engineering 1 Mini-Project 
A game where you guess the number between 0-9 and you have 3 lives 
*/


#include <atmel_start.h>
#include <stdio.h>
#include "util/delay.h"
#include "lcd_Start.h"
#include "keypad_Start.h"

#define LED1 5
#define LED2 4
#define LED3 3
#define SW 7

void Flash();//function to flash LEDs 
void LcdUpdate();//This will display scores 



int main(void)
{
	atmel_start_init();
	keypadConfigure();
	lcd_init(LCD_DISP_ON_CURSOR);
	
	uint8_t keyPosition, randNum, userinput, input ,tries, streak = 0,win = 0, loss = 0;
	uint8_t keypad[] = "123456789*0#";
	uint32_t delay;
	
	
	PORTB_set_pin_dir(LED1, PORT_DIR_OUT);
	PORTB_set_pin_dir(LED2, PORT_DIR_OUT);
	PORTB_set_pin_dir(LED3, PORT_DIR_OUT);
	
	Flash();
	printf("Press Button to Play\n\n");
	LcdUpdate(streak, win, loss);
	while(1)
	{	
		delay = 0;
		tries = 0;
		
		PORTB_set_pin_level(LED1, 1);
		PORTB_set_pin_level(LED2, 1);
		PORTB_set_pin_level(LED3, 1);
		
		while(PORTB_get_pin_level(SW) == 1){ _delay_ms(1); delay++;}//seed for the random number 
			
		while(PORTB_get_pin_level(SW) == 0){}//hold statement 
		srand(delay);
		randNum = rand()%10;//random number less than 10
		
		
		while(tries < 3){
			printf("Please Enter Guess Number %d:\n",tries+1);
		
			keyPosition = keypadScan();
			userinput = keypad[keyPosition];
			
			input = userinput - '0';  //convert from char to int 
		  
			if(input == randNum){printf("You have won\n");win++; streak++;break;}//win 
				
				if(userinput != '*' && userinput != '#'){//stops invalid charaters 
					if(input+1 == randNum || input-1 == randNum){printf("Your guess is + or - 1 from answer\n");}
					else{
						if(input > randNum){printf("The Number is Lower\n");}
						if(input <	randNum){printf("The Number is Higher\n");}
						}
					
					switch(tries){//turn off for when you are wrong 
					case 0: PORTB_set_pin_level(LED1, 0); break;
					case 1: PORTB_set_pin_level(LED2, 0); break;
					case 2: PORTB_set_pin_level(LED3, 0); streak = 0; loss++; break;}//adds loss and resets streak 
					tries++;
				}
		  }
	LcdUpdate(streak, win, loss, randNum);
	printf("Press Button to Play Again\n\n");//play again???
}
}

void Flash()
{
	for(uint8_t x = 0; x<6; x++){
		PORTB_set_pin_level(LED1, 1);
		PORTB_set_pin_level(LED2, 1);
		PORTB_set_pin_level(LED3, 1);
		_delay_ms(200);
		PORTB_set_pin_level(LED1, 0);
		PORTB_set_pin_level(LED2, 0);
		PORTB_set_pin_level(LED3, 0);
		_delay_ms(200);
	}
}

void LcdUpdate(int streak, int win, int loss, int randNum)
{
	char lcdString[41];
	
	if(loss > 0 || win > 0)
	{
		lcd_clrscr();
		sprintf(lcdString,"The Number was %d", randNum);//tells you the answer 
		lcd_puts(lcdString);
		Flash();		
	}
	
	lcd_clrscr();
	sprintf(lcdString,"Streak: %d\nWins: %d Losses: %d", streak, win, loss);//displays scores 
	lcd_puts(lcdString);

}

