/*
 * Parcial3.c
 *
 * Created: 01/06/2021 8:24:17
 * Author : jessi
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <avr/interrupt.h>


void imprimir();

int tiempo=0;
int s=0;
int min=0;
int paro=0;

int main(void)
{
	
	DDRA=0XFF;
	PORTA=0X00;
	
	DDRB=0XFF;
	PORTB=0X00;
	
	DDRC=0XFF;
	PORTC=0X00;
	
	EIMSK=0X03;		//Habilitar interrupcion 0 (inicio) y 1 (fin)
	EICRA=0X0F;		//Por flanco de subida
	
	sei();
	
	while(1){
		asm("NOP");
		asm("NOP");
	}
	
}
	ISR(INT0_vect){
		
		paro=0;
		TCCR1B=0B100;		//	Prescaler 256 Num ciclos 1s=31.250
		TCNT1H=0xDC;
		TCNT1L=0X01;		//Desborde a 1s
		TIMSK1=0x01;		//Habilita interrupcion por desbordar
		
	}
	ISR(INT1_vect){
		paro=1;
		tiempo=0;
		s=0;
		min=0;
		
	}
	ISR(TIMER1_OVF_vect){
		
		if(paro==0){
			tiempo++;
			
			TCCR1B=0B100;		//	Prescaler 256 Num ciclos 1s=31.250
			TCNT1H=0xDC;
			TCNT1L=0X01;		//Desborde a 1s
			TIMSK1=0x01;		//Habilita interrupcion por desbordar
			
			imprimir();
		}
		
		

	}

	void imprimir(){
		
		min=tiempo/60;
		PORTA=min;
		
		s=tiempo%60;
		int d=s/10;
		PORTB=d;
		int u=s%10;
		PORTC=u;
		
	}

