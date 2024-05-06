/******************************************************************************
 * Created: 
 * Author :
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>
#include <inttypes.h>

/******************************************************************************
* Macros
******************************************************************************/
#define OSSZEADAS 1
#define KIVONAS 2
#define OSZTAS 4
#define SZORZAS 8

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint16_t timer_cnt=0;
uint8_t timer_task_10ms=0, timer_task_100ms=0, timer_task_500ms=0, timer_task_1s=0;
uint8_t PB0_pushed = 0, PB1_pushed = 0, PB2_pushed = 0, PB3_pushed = 0, PB4_pushed = 0;
uint8_t szam1=0, szam2=0;
uint8_t muvelet = OSSZEADAS;
/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);
void timer_init(void);
uint8_t check_timer_compare(void);
/******************************************************************************
* Local Function Definitions
******************************************************************************/

/*****************************************************************************
* Function:         void port_init(void)
* Description:      I/O portok inicializálása
* Input:
* Output:
* Notes:
******************************************************************************/
void port_init(void)
{
	DDRA = 0xff;
	DDRF = (1<<PA3) | (1<<PA2) | (1<<PA1) | (1<<PA0);
	PORTF = (1<<PA3) | (1<<PA2) | (1<<PA1) | (1<<PA0);
	
	DDRB = (0<<PB0) | (0<<PB1) | (0<<PB2) | (0<<PB3) | (0<<PB4);
	PORTB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4);
	
	DDRD = 0xff;
	DDRE = 0xff;
}

/******************************************************************************
* Function:         void timer_init(void)
* Description:      Timer 0 inicializálása
* Input:
* Output:
* Notes:
******************************************************************************/
void timer_init(void)
{
	TCCR0 = (0<<WGM00) | (1<<WGM01) | (1<<CS02) | (1<<CS01) | (1<<CS00);
	OCR0 = 77;
}

/******************************************************************************
* Function:         uint8_t check_timer_compare(void)
* Description:      Timer jelzés kiolvasása
* Input:
* Output:			1, ha a timer jelez, 0 ha nem
* Notes:
******************************************************************************/
uint8_t check_timer_compare(void)
{
	if((TIFR & (1<<OCF0)) == (1<<OCF0))
	{
		TIFR = TIFR | (1<<OCF0);
		return 1;
	}
	return 0;
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
	
	//Végtelen ciklus
	while(1)
	{
		if(check_timer_compare())
		{
			timer_cnt++;
			if((timer_cnt % 1) == 0) timer_task_10ms=1;
			if((timer_cnt % 10) == 0) timer_task_100ms=1;
			if((timer_cnt % 50) == 0) timer_task_500ms=1;
			if((timer_cnt % 100) == 0) timer_task_1s=1;
		}
		
		if(timer_task_10ms)
		{
			PORTF ^= (1<<PF0);
			
			if((PINB & (1<<PB0)) == 0 && PB0_pushed == 0)
			{
				szam1++;
				if(szam1>0x0f) szam1 = 0;
				PB0_pushed = 1;
			}
			if((PINB & (1<<PB0)) == (1<<PB0) && PB0_pushed == 1) PB0_pushed = 0;
			
			
			if((PINB & (1<<PB1)) == 0 && PB1_pushed == 0)
			{
				szam1--;
				if(szam1==255) szam1 = 0x0f;
				PB1_pushed = 1;
			}
			if((PINB & (1<<PB1)) == (1<<PB1) && PB1_pushed == 1) PB1_pushed = 0;
			
			if((PINB & (1<<PB2)) == 0 && PB2_pushed == 0)
			{
				szam2++;
				if(szam2>0x0f) szam2 = 0;
				
				PB2_pushed = 1;
			}
			if((PINB & (1<<PB2)) == (1<<PB2) && PB2_pushed == 1) PB2_pushed = 0;
			
			if((PINB & (1<<PB3)) == 0 && PB3_pushed == 0)
			{
				szam2--;
				if(szam2==255) szam2 = 0x0f;
				PB3_pushed = 1;
			}
			if((PINB & (1<<PB3)) == (1<<PB3) && PB3_pushed == 1) PB3_pushed = 0;
			
			if((PINB & (1<<PB4)) == 0 && PB4_pushed == 0)
			{
				muvelet=muvelet << 1;
				if(muvelet>8) muvelet = 1;
				PB4_pushed = 1;
			}
			if((PINB & (1<<PB4)) == (1<<PB4) && PB4_pushed == 1) PB4_pushed = 0;
			
			
			PORTD = (szam1 | (szam2<<4));
			PORTE = muvelet;
			
			switch(muvelet)
			{
				case OSSZEADAS:
				{
					PORTA = szam1 + szam2;
					break;
				}
				case KIVONAS:
				{
					PORTA = szam1 - szam2;
					break;
				}
				case SZORZAS:
				{
					PORTA = szam1 * szam2;
					break;
				}
				case OSZTAS:
				{
					PORTA = szam1 / szam2;
					break;
				}
			}
			timer_task_10ms = 0;
		}
		
		if(timer_task_100ms)
		{
			PORTF ^= (1<<PF1);
			timer_task_100ms =0;
		}
		
		if(timer_task_500ms)
		{
			PORTF ^= (1<<PF2);
			timer_task_500ms=0;
		}
		
		if(timer_task_1s)
		{
			PORTF ^= (1<<PF3);
			timer_task_1s=0;
		}

	}
}
/******************************************************************************
* Interrupt Routines
******************************************************************************/