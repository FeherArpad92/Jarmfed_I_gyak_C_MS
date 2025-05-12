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
#define  OSSZEADAS 1
#define  KIVONAS 2
#define  SZORZAS 4
#define  OSZTAS 8

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
uint16_t timer_cnt=0;
uint8_t timer_task_10ms=0, timer_task_100ms=0, timer_task_500ms=0, timer_task_1s=0;
uint8_t PB0_pushed = 0,PB1_pushed = 0,PB2_pushed = 0,PB3_pushed = 0,PB4_pushed = 0;
uint8_t szam1=0, szam2=0;
uint8_t muvelet = OSSZEADAS;

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
	TCCR0A = (0<<WGM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	OCR0A = 77;
}

void port_init(void)
{
	DDRA = 0xff;
	DDRF = (1<<PF3) | (1<<PF2) | (1<<PF1) | (1<<PF0);
	DDRB = (0<<PB4) | (0<<PB3) | (0<<PB2) | (0<<PB1) | (0<<PB0);
	PORTB = (1<<PB4) | (1<<PB3) | (1<<PB2) | (1<<PB1) | (1<<PB0);
	
	DDRD = 0xff;
	DDRE = 0x0f;
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
	/* Replace with your application code */
	while(1)
	{
		if(check_timer_compare())
		{
			timer_cnt++;
			timer_task_10ms = 1;
			if((timer_cnt % 10)==0) timer_task_100ms = 1;
			if((timer_cnt % 50)==0) timer_task_500ms = 1;
			if((timer_cnt % 100)==0) timer_task_1s = 1;
		}

		if(timer_task_10ms)
		{
			if((PINB & (1<<PB0))==0 && PB0_pushed == 0)
			{
				szam1++;
				if(szam1>0x0f) szam1=0;
				PB0_pushed = 1;
			}
			if((PINB & (1<<PB0)) == (1<<PB0) && PB0_pushed == 1) PB0_pushed = 0;
			
			if((PINB & (1<<PB1))==0 && PB1_pushed == 0)
			{
				szam1--;
				if(szam1==0xff) szam1 = 0x0f;
				PB1_pushed = 1;
			}
			if((PINB & (1<<PB1)) == (1<<PB1) && PB1_pushed == 1) PB1_pushed = 0;
			
			if((PINB & (1<<PB2))==0 && PB2_pushed == 0)
			{
				szam2++;
				if(szam2>0x0f) szam2=0;
				PB2_pushed = 1;
			}
			if((PINB & (1<<PB2)) == (1<<PB2) && PB2_pushed == 1) PB2_pushed = 0;
			
			if((PINB & (1<<PB3))==0 && PB3_pushed == 0)
			{
				szam2--;
				if(szam2 ==0xff) szam2 = 0x0f;
				PB3_pushed = 1;
			}
			if((PINB & (1<<PB3)) == (1<<PB3) && PB3_pushed == 1) PB3_pushed = 0;
			
			if((PINB & (1<<PB4))==0 && PB4_pushed == 0)
			{
				muvelet = muvelet *2;
				if(muvelet>8) muvelet =OSSZEADAS;
				PB4_pushed = 1;
			}
			if((PINB & (1<<PB4)) == (1<<PB4) && PB4_pushed == 1) PB4_pushed = 0;
			
			PORTD = (szam1 | (szam2<<4));
			PORTE = muvelet;
			
			switch(muvelet)
			{
				case OSSZEADAS:
				{
					PORTA = szam1+szam2;
					break;
				}
				case KIVONAS:
				{
					PORTA =szam1-szam2;
					break;
				}
				case SZORZAS:
				{
					PORTA =szam1*szam2;
					break;
				}
				case OSZTAS:
				{
					PORTA =szam1/szam2;
					break;
				}
			}
			
			PORTF ^=(1<<PF0);
			timer_task_10ms=0;
		}
		if(timer_task_100ms)
		{
			PORTF ^=(1<<PF1);
			timer_task_100ms=0;
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




