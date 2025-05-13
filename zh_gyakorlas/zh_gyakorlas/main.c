/******************************************************************************
 * Created: 
 * Author :
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>

/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint16_t timer_cnt=0;
uint8_t timer_task_10ms=0, timer_task_100ms=0,timer_task_250ms=0, timer_task_500ms=0, timer_task_1s=0;
uint8_t PB7_pushed=0,PB0_pushed=0;
uint8_t szamlalo=0;
uint8_t ido=0;
uint8_t szamlalas_enged=0;
/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/
void timer_init(void);
uint8_t check_timer_compare(void);
void port_init(void);
/******************************************************************************
* Local Function Definitions
******************************************************************************/

uint8_t check_timer_compare(void)
{
	if((TIFR0 & (1<<OCF0A)) == (1<<OCF0A))
	{
		TIFR0 = TIFR0 | (1<<OCF0A);
		return 1;
	}
	return 0;
}

void timer_init(void)
{
	//TCCR0A = 0b00001101;
	TCCR0A = (0<<FOC0A) | (0<<WGM00) | (0<<COM0A1) | (0<<COM0A0) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
	//OCR0A = 77;
	OCR0A = 155;
}

void port_init(void)
{
	DDRA=0xff; // A port összes lába kimenet
	
	DDRF = (1<<PF3) | (1<<PF2) | (1<<PF1) | (1<<PF0); // F port alsó 4 bitje kimenet
	
	DDRB = (0<<PB7)| (0<<PB0); // B port 7-es és 0-ás bitje bemenet
	PORTB = (1<<PB7) | (1<<PB0); // B port 7-es és 0-ás bitjén felhúzó ellenállás
	
	DDRD = 0xff;
}

/******************************************************************************
* Function:         int main(void)
* Description:      main function
* Input:            
* Output:           
* Notes:            
******************************************************************************/
int main(void)
{
	port_init();
	timer_init();
	
	while(1)
	{
		if(check_timer_compare())
		{
			timer_cnt++;
			if((timer_cnt % 2) == 0) timer_task_10ms =1;
			if((timer_cnt % 20) == 0) timer_task_100ms =1;
			if((timer_cnt % 50) == 0 ) timer_task_250ms =1;
			if((timer_cnt % 100) == 0) timer_task_500ms =1;
			if((timer_cnt % 200) == 0) timer_task_1s =1;
			
		}
		if(timer_task_10ms)
		{
			PORTF ^=(1<<PF0);
			
			if((PINB & (1<<PB7)) == 0 && PB7_pushed == 0)
			{
				szamlalo++;
				PB7_pushed=1;
			}
			if((PINB & (1<<PB7)) == (1<<PB7) && PB7_pushed == 1) PB7_pushed = 0;
			
			if((PINB & (1<<PB0)) == 0 && PB0_pushed == 0)
			{
				if(szamlalas_enged==0)
				{
					szamlalas_enged=1;
				}
				else
				{
					szamlalas_enged=0;
				}
				
				PB0_pushed=1;
			}
			if((PINB & (1<<PB0)) == (1<<PB0) && PB0_pushed == 1) PB0_pushed = 0;
			
			PORTA = szamlalo<<3;
			
			timer_task_10ms=0;
		}
		
		if(timer_task_100ms)
		{
			PORTF ^=(1<<PF1);
			timer_task_100ms=0;
		}
		
		if(timer_task_250ms)
		{
			if(szamlalas_enged==1) ido++;
			if(szamlalas_enged==0) PORTD = ido;
			timer_task_250ms=0;
		}
		
		if(timer_task_500ms)
		{
			PORTF ^=(1<<PF2);
			timer_task_500ms=0;
		}
		if(timer_task_1s)
		{
			PORTF ^=(1<<PF3);
			timer_task_1s=0;
		}
	}
}


/******************************************************************************
* Interrupt Routines
******************************************************************************/
