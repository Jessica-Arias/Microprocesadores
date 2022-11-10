/*
 * Laboratorio2.c
 *
 * Created: 20/05/2021 6:03:59 p. m.
 * Author : Samantha
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.c"


int n, v, j=1, p=0;
char a, c, pos=0b1001;
char enter=13;


int RecibirPasos(int n);
void EnviarPasos (int n);

int RecibirVelocidad (int v);
void EnviarVelocidad (int v);

char RecibirDireccion (char a);
void EnviarDireccion (char a);

void verificar();
char ContinuarCorregir();
void reiniciar();

void controlMotor (int n,char a,int v);

void USART_Transmit(unsigned char data);

int main(void)
{
	n=0;
	a=' ';
	v=0;
	
	asm("LDI R16,0xFF");   //DDRA = 0xFF;(0x01)			//Puerto A es salida
	asm("OUT 0x01,R16");
	
	asm("LDI R16,0x00");	//PORTA = 0x00;	(0x02)		//Se inicia en 0
	asm("OUT 0x02, R16");
	
	
	inicializarLCD();
	
	asm("LDI R16,0x00");     //UBRR0=0x067;UBRR0H(0xC5)   // BAUDIOS=9600
	asm("STS 0xC5,R16");
	
	asm("LDI R16,0x67");	//UBRR0L(0xC4)
	asm("STS 0xC4,R16");
	
	
	asm("LDI R16,0x06");		//UCSR0C=0x06;(0xC2)	//TAMAÑO CARACTER=8 BIT (UCSZN)
	asm("STS 0xC2,R16");
	
	asm("LDI R16,0x98");		//UCSR0B=0x98;(0xC1)	//RXCIE (INTERRUPCION RX- BIT 7)
	asm("STS 0xC1,R16");		//RXEN, TXEN (ENABLE RX Y TX BIT 4,3)
					
	
	asm("LDI R16,0x01");		//SMCR=1;(0x33)		//HABILITAR SLEEP
	asm("OUT 0x33,R16");
	
	
	USART_Transmit('I');
	USART_Transmit('N');
	USART_Transmit('G');
	USART_Transmit('R');
	USART_Transmit('E');
	USART_Transmit('S');
	USART_Transmit('E');
	USART_Transmit(' ');
	USART_Transmit('N');
	USART_Transmit('U');
	USART_Transmit('M');
	USART_Transmit('E');
	USART_Transmit('R');
	USART_Transmit('O');
	USART_Transmit(' ');
	USART_Transmit('D');
	USART_Transmit('E');
	USART_Transmit(' ');
	USART_Transmit('P');
	USART_Transmit('A');
	USART_Transmit('S');
	USART_Transmit('O');
	USART_Transmit('S');
	USART_Transmit(' ');
	USART_Transmit('(');
	USART_Transmit('1');
	USART_Transmit('-');
	USART_Transmit('9');
	USART_Transmit(')');
	USART_Transmit(':');
	USART_Transmit(enter);
	
	sei();
	
	while (1){
		asm("SLEEP");	//TAREA DE FONDO
		asm("NOP");
		asm("NOP");
	}
	
}

ISR(USART0_RX_vect){
	if (j==1)
	{
		n=RecibirPasos(n);
	}
	else if (j==2)
	{
		v=RecibirVelocidad(v);
		
	}else if (j==3)
	{
		a=RecibirDireccion(a);
	}else if (j==4)
	{
		c=ContinuarCorregir( c);
		if (c==1)
		{
			controlMotor(n, a, v);
		}
		
	}else if(j==5)
	{
		
		reiniciar();
	}
}

void controlMotor (int n,char a,int v){
	
	char const HORARIO[8] = {	0b1000, 0b1100, 0b0100,
								0b0110, 0b0010, 0b0011,
								0b0001, 0b1001};
	
	char const ANTIH[8] ={  0b0001, 0b0011,
							0b0010, 0b0110, 0b0100,
							0b1100, 0b1000, 0b1001};
	
	
	if (a=='i'|| a=='I')
	{
		
		for (int x = 0; x < 8; x++) {
			if (ANTIH[x] == pos){
			p=x+1;		
			}
			
		}
		
		for (int i=0; i<n;i++)
		{
			if (p>=8)
			{
				p=0;
			}
			if (v==1)
			{
				PORTA=ANTIH[p];
				_delay_ms(2500);
			}
			else if (v==2){
				PORTA=ANTIH[p];
				_delay_ms(2000);
			}
			else if (v==3)
			{
				PORTA=ANTIH[p];
				_delay_ms(1500);
			}
			else {
				
				PORTA=ANTIH[p];
				_delay_ms(1000);
				
			}
			
			pos=ANTIH[p];
			
			p++;
		}
		}else {
			
			
			for (int x = 0; x < 8; x++) {
				if (HORARIO[x] == pos){
					p=x+1;
				}
				
			}
			
			
		for (int i=0; i<n;i++)
		{
					
			if (p>=8)
			{
				p=0;
			}
			if (v==1)
			{
				PORTA=HORARIO[p];
				_delay_ms(2500);
			}
			else if (v==2){
				PORTA=HORARIO[p];
				_delay_ms(2000);
			}
			else if (v==3)
			{
				PORTA=HORARIO[p];
				_delay_ms(1500);
			}
			else {
				
				PORTA=HORARIO[p];
				_delay_ms(1000);
				
			}
			
			pos=HORARIO[p];
			
			p++;
		}
		
	}
	
	
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) ) 
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

int RecibirPasos(int n){
	
	n=UDR0;
	USART_Transmit(n);
	USART_Transmit(enter);
	
	if (n>=49 && n<=57)
	{
		
		USART_Transmit('I');
		USART_Transmit('N');
		USART_Transmit('G');
		USART_Transmit('R');
		USART_Transmit('E');
		USART_Transmit('S');
		USART_Transmit('E');
		USART_Transmit(' ');
		USART_Transmit('V');
		USART_Transmit('E');
		USART_Transmit('L');
		USART_Transmit('O');
		USART_Transmit('C');
		USART_Transmit('I');
		USART_Transmit('D');
		USART_Transmit('A');
		USART_Transmit('D');
		USART_Transmit(' ');
		USART_Transmit('(');
		USART_Transmit('1');
		USART_Transmit(' ');
		USART_Transmit('M');
		USART_Transmit('I');
		USART_Transmit('N');
		USART_Transmit(',');
		USART_Transmit(' ');
		USART_Transmit('4');
		USART_Transmit(' ');
		USART_Transmit('M');
		USART_Transmit('A');
		USART_Transmit('X');
		USART_Transmit(')');
		USART_Transmit(':');
		USART_Transmit(enter);

		j++;
		
		EnviarPasos(n);
		
		n=n-48;
		
		}else{
		
		n=0;
		verificar();
	}
	
	return n;
	
}

int RecibirVelocidad (int v){
	
	v=UDR0;
	USART_Transmit(v);
	USART_Transmit(enter);
	
	if (v>=49 && v<=52)
	{
		
		USART_Transmit('I');
		USART_Transmit('N');
		USART_Transmit('G');
		USART_Transmit('R');
		USART_Transmit('E');
		USART_Transmit('S');
		USART_Transmit('E');
		USART_Transmit(' ');
		USART_Transmit('D');
		USART_Transmit('I');
		USART_Transmit('R');
		USART_Transmit('E');
		USART_Transmit('C');
		USART_Transmit('C');
		USART_Transmit('I');
		USART_Transmit('O');
		USART_Transmit('N');
		USART_Transmit(' ');
		USART_Transmit('(');
		USART_Transmit('I');
		USART_Transmit(' ');
		USART_Transmit('o');
		USART_Transmit(' ');
		USART_Transmit('D');
		USART_Transmit(')');
		USART_Transmit(':');
		USART_Transmit(enter);
		
		j++;
		
		EnviarVelocidad(v);
		
		v=v-48;
		
		}else{
		
		v=0;
		verificar();
		
	}
	return v;
}

char RecibirDireccion (char a){

	a=UDR0;
	USART_Transmit(a);
	USART_Transmit(enter);
	
	if (a=='D' || a=='I' || a=='d' || a=='i')
	{
		USART_Transmit('I');
		USART_Transmit('N');
		USART_Transmit('G');
		USART_Transmit('R');
		USART_Transmit('E');
		USART_Transmit('S');
		USART_Transmit('E');
		USART_Transmit(' ');
		USART_Transmit('1');
		USART_Transmit(' ');
		USART_Transmit('P');
		USART_Transmit('A');
		USART_Transmit('R');
		USART_Transmit('A');
		USART_Transmit(' ');
		USART_Transmit('C');
		USART_Transmit('O');
		USART_Transmit('N');
		USART_Transmit('T');
		USART_Transmit('I');
		USART_Transmit('N');
		USART_Transmit('U');
		USART_Transmit('A');
		USART_Transmit('R');
		USART_Transmit(' ');
		USART_Transmit('o');
		USART_Transmit(' ');
		USART_Transmit('0');
		USART_Transmit(' ');
		USART_Transmit('P');
		USART_Transmit('A');
		USART_Transmit('R');
		USART_Transmit('A');
		USART_Transmit(' ');
		USART_Transmit('C');
		USART_Transmit('O');
		USART_Transmit('R');
		USART_Transmit('R');
		USART_Transmit('E');
		USART_Transmit('G');
		USART_Transmit('I');
		USART_Transmit('R');
		USART_Transmit(':');
		USART_Transmit(enter);
		
		EnviarDireccion(a);
		j++;
	}
	else
	{
		
		a=' ';
		verificar();
		
	}
	return a;
}

void verificar(){
	
	USART_Transmit('E');
	USART_Transmit('R');
	USART_Transmit('R');
	USART_Transmit('O');
	USART_Transmit('R');
	USART_Transmit('!');
	USART_Transmit(' ');
	USART_Transmit('I');
	USART_Transmit('N');
	USART_Transmit('G');
	USART_Transmit('R');
	USART_Transmit('E');
	USART_Transmit('S');
	USART_Transmit('E');
	USART_Transmit(' ');
	USART_Transmit('U');
	USART_Transmit('N');
	USART_Transmit(' ');
	USART_Transmit('V');
	USART_Transmit('A');
	USART_Transmit('L');
	USART_Transmit('O');
	USART_Transmit('R');
	USART_Transmit(' ');
	USART_Transmit('C');
	USART_Transmit('O');
	USART_Transmit('R');
	USART_Transmit('R');
	USART_Transmit('E');
	USART_Transmit('C');
	USART_Transmit('T');
	USART_Transmit('O');
	USART_Transmit('!');
	USART_Transmit(enter);
}

char ContinuarCorregir (){
	
	c=UDR0;
	USART_Transmit(c);
	USART_Transmit(enter);
	
	if (c==48 || c==49)
	{
		
		if (c==48)
		{
			c=0;
			reiniciar();
			USART_Transmit('I');
			USART_Transmit('N');
			USART_Transmit('G');
			USART_Transmit('R');
			USART_Transmit('E');
			USART_Transmit('S');
			USART_Transmit('E');
			USART_Transmit(' ');
			USART_Transmit('N');
			USART_Transmit('U');
			USART_Transmit('M');
			USART_Transmit('E');
			USART_Transmit('R');
			USART_Transmit('O');
			USART_Transmit(' ');
			USART_Transmit('D');
			USART_Transmit('E');
			USART_Transmit(' ');
			USART_Transmit('P');
			USART_Transmit('A');
			USART_Transmit('S');
			USART_Transmit('O');
			USART_Transmit('S');
			USART_Transmit(' ');
			USART_Transmit('(');
			USART_Transmit('1');
			USART_Transmit('-');
			USART_Transmit('9');
			USART_Transmit(')');
			USART_Transmit(':');
			USART_Transmit(enter);
		}
		else
		{
			c=1;
			j++;
			
			
			USART_Transmit('S');
			USART_Transmit('I');
			USART_Transmit('G');
			USART_Transmit('U');
			USART_Transmit('I');
			USART_Transmit('E');
			USART_Transmit('N');
			USART_Transmit('T');
			USART_Transmit('E');
			USART_Transmit('S');
			USART_Transmit(' ');
			USART_Transmit('D');
			USART_Transmit('A');
			USART_Transmit('T');
			USART_Transmit('O');
			USART_Transmit('S');
			USART_Transmit(enter);
			USART_Transmit('I');
			USART_Transmit('N');
			USART_Transmit('G');
			USART_Transmit('R');
			USART_Transmit('E');
			USART_Transmit('S');
			USART_Transmit('E');
			USART_Transmit(' ');
			USART_Transmit('N');
			USART_Transmit('U');
			USART_Transmit('M');
			USART_Transmit('E');
			USART_Transmit('R');
			USART_Transmit('O');
			USART_Transmit(' ');
			USART_Transmit('D');
			USART_Transmit('E');
			USART_Transmit(' ');
			USART_Transmit('P');
			USART_Transmit('A');
			USART_Transmit('S');
			USART_Transmit('O');
			USART_Transmit('S');
			USART_Transmit(' ');
			USART_Transmit('(');
			USART_Transmit('1');
			USART_Transmit('-');
			USART_Transmit('9');
			USART_Transmit(')');
			USART_Transmit(':');
			USART_Transmit(enter);
		}
		
	}
	else
	{
		
		verificar();
		
	}
	
	return c;
}

void reiniciar(){
	n=0;
	v=0;
	a=' ';
	PORTA=0x00;
	
	
	
	
	
	inicializarLCD();
	
	j=1;
}

void EnviarPasos(n){

	escribir_lcd('P');
	escribir_lcd('A');
	escribir_lcd('S');
	escribir_lcd('O');
	escribir_lcd('S');
	escribir_lcd(':');
	escribir_lcd(' ');
	
	escribir_lcd(n);

}

void EnviarVelocidad(v){
	
	escribir_lcd(' ');
	escribir_lcd(' ');
	escribir_lcd('V');
	escribir_lcd('E');
	escribir_lcd('L');
	escribir_lcd(':');
	escribir_lcd(' ');
	
	escribir_lcd(v);
	
	enter_lcd();

}

void EnviarDireccion(char a){

	
	escribir_lcd('D');
	escribir_lcd('I');
	escribir_lcd('R');
	escribir_lcd(':');
	escribir_lcd(' ');

	if (a=='i'||a=='I')
	{
		escribir_lcd('I');
		escribir_lcd('Z');
		escribir_lcd('Q');
		escribir_lcd('U');
		escribir_lcd('I');
		escribir_lcd('E');
		escribir_lcd('R');
		escribir_lcd('D');
		escribir_lcd('A');
		escribir_lcd('.');
		
	}
	else
	{
		escribir_lcd('D');
		escribir_lcd('E');
		escribir_lcd('R');
		escribir_lcd('E');
		escribir_lcd('C');
		escribir_lcd('H');
		escribir_lcd('A');
		escribir_lcd('.');
	}

}