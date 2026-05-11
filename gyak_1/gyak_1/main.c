/*
 * gyak_1.c
 *
 * Created: 4/20/2026 5:20:26 PM
 * Author : feherarpad
 */ 

#include <avr/io.h>


int main(void)
{
	DDRB = 0b11111111;
	//PORTB = 0b10000001;
	//PORTB = 0xFF;
	//PORTB = 255;
	
	PORTB = (1<<PB2) | (1<<PB7);
	PORTB = 0b10000100;
	unsigned long int i =0;
    /* Replace with your application code */
    while (1) 
    {
		for(i=0;i<40000;i++);
		
		PORTB ^= 0x01;
    }
}

