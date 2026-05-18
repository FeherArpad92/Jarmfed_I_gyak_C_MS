/******************************************************************************
 * Created: 
 * Author :
******************************************************************************/
 /******************************************************************************
* Include files
******************************************************************************/
#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/delay.h>


/******************************************************************************
* Macros
******************************************************************************/
#define OSSZEADAS 1
#define KIVONAS 2
#define SZORZAS 4
#define OSZTAS 8

/******************************************************************************
* Constants
******************************************************************************/


/******************************************************************************
* Global Variables
******************************************************************************/
int irany=0;
int timer_cnt=0;
int timer_task_10ms=0, timer_task_100ms=0, timer_task_500ms=0;
int PB0_pushed = 0, PB1_pushed = 0, PB2_pushed = 0, PB3_pushed = 0, PB4_pushed = 0;
int szam1=0, szam2=0;
int muvelet=OSSZEADAS;

/******************************************************************************
* External Variables
******************************************************************************/


/******************************************************************************
* Local Function Declarations
******************************************************************************/
void port_init(void);
void timer_init(void);
int check_timer_compare(void);

/******************************************************************************
* Local Function Definitions
******************************************************************************/
void port_init(void)
{
	DDRA=0xff;
	PORTA = 0x00;
	
	DDRB = (0<<PB0) | (0<<PB1) | (0<<PB2) | (0<<PB3) | (0<<PB4);
	PORTB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4);
	
	DDRD = 0xff;
	DDRE = 0xff;
}

void timer_init(void)
{
	TCCR0A = (1<<WGM01) | (0<<WGM00) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	OCR0A = 77;
}

int check_timer_compare(void)
{
	if((TIFR0 & (1<<OCF0A)) == (1<<OCF0A))
	{
		TIFR0 = TIFR0 | (1<<OCF0A);
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
	PORTA = 0x01;
	timer_init();
	
	/* Replace with your application code */
	while(1)
	{
		
		if(check_timer_compare())
		{
			timer_cnt++;
			
			if((timer_cnt % 1) == 0) timer_task_10ms =1;
			if((timer_cnt % 10) == 0) timer_task_100ms =1;
			if((timer_cnt % 50) == 0) timer_task_500ms =1;
			
			if(timer_task_10ms)
			{
				PORTD = szam1 | (szam2<<4);
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
						PORTA = szam1-szam2;
						break;
					}
					case SZORZAS:
					{
						PORTA = szam1*szam2;
						break;
					}
					case OSZTAS:
					{
						PORTA = szam1/szam2;
						break;
					}
					
				}
				
				if((PINB & (1<<PB0)) == 0 && PB0_pushed == 0)
				{
					szam1++;
					if(szam1>15) szam1=0;
					PB0_pushed = 1;
				}
				if((PINB & (1<<PB0)) == (1<<PB0) && PB0_pushed == 1) PB0_pushed=0;
				
				if((PINB & (1<<PB1)) == 0 && PB1_pushed == 0)
				{
					szam1--;
					if(szam1<0) szam1 = 15;
					PB1_pushed = 1;
				}
				if((PINB & (1<<PB1)) == (1<<PB1) && PB1_pushed == 1) PB1_pushed=0;
				
				if((PINB & (1<<PB2)) == 0 && PB2_pushed == 0)
				{
					szam2++;
					if(szam2>15) szam2=0;
					PB2_pushed = 1;
				}
				if((PINB & (1<<PB2)) == (1<<PB2) && PB2_pushed == 1) PB2_pushed=0;
				
				if((PINB & (1<<PB3)) == 0 && PB3_pushed == 0)
				{
					szam2--;
					if(szam2<0) szam2 = 15;
					PB3_pushed = 1;
				}
				if((PINB & (1<<PB3)) == (1<<PB3) && PB3_pushed == 1) PB3_pushed=0;
				
				if((PINB & (1<<PB4)) == 0 && PB4_pushed == 0)
				{
					muvelet = muvelet<<1;
					if(muvelet == 0x10) muvelet=OSSZEADAS;
					PB4_pushed = 1;
				}
				if((PINB & (1<<PB4)) == (1<<PB4) && PB4_pushed == 1) PB4_pushed=0;
				timer_task_10ms=0;
			}
			
			if(timer_task_100ms)
			{
				
				timer_task_100ms=0;
			}
			
			if(timer_task_500ms)
			{
				//if(irany==0)
				//{
					//PORTA = PORTA << 1;
				//}
				//else
				//{
					//PORTA = PORTA >> 1;
				//}
				//
				//if(PORTA==0x80) irany=1;
				//if(PORTA==0x01) irany=0;
				timer_task_500ms=0;
			}
			
			
		}
		

			
			

		
		//PORTA = PORTA ^ 0xff;
		//PORTA = PORTA * 2;
		
		
		
		//_delay_ms(50);
		//for(long int i=0;i<100000;i++);
	}
}


/******************************************************************************
* Interrupt Routines
******************************************************************************/