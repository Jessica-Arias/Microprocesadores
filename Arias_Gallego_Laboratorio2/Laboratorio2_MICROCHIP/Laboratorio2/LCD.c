
char caracter;
void escribir_lcd(char caracter);
void inicializarLCD();
void enter_lcd();

void inicializarLCD()
{
	
	asm("LDI R16,0xFF")	;	//DDRB = 0xFF;(0x04)			//Puerto B salida
	asm("OUT 0x04,R16");
	
	asm("LDI R16,0x00");		//PORTB = 0x00;	(0x05)		//puerto B en 0
	asm("OUT 0x05, R16");
	
	asm("LDI R16,0xFF");	//DDRC = 0xFF;	(0x07)		//Puerto C salida
	asm("OUT 0x07,R16");
	
	asm("LDI R16,0x00");     //PORTC = 0x00;(0x08)			//puerto C en 0
	asm("OUT 0x08, R16");
	
	asm("LDI R16,0xFF");	//DDRD = 0xFF;	(0x0A)		//Puerto D es salida		
	asm("OUT 0x0A,R16");
	
	
	asm("LDI R16,0x00");    //PORTD = 0x00;	(0x0B)		//Se inicia en 0
	asm("OUT 0x0B, R16");
	
	
	asm("LDI R16,0x01");	//PORTB = 0x01;			//Clear display
	asm("OUT 0x05, R16");
	pulso();
	
	asm("LDI R16,0b1111");		//PORTB = 0b1111;			//Activacion display, cursor en on, y blinking activado
	asm("OUT 0x05, R16");
	pulso();
	
	asm("LDI R16,0b111000");		//PORTB = 0b111000;		//Display configurado a 2 linea, 8 bits , matriz 5x7
	asm("OUT 0x05, R16");
	pulso();
	
}

void pulso()
{
	asm("LDI R16,0x01");		//PORTC = 0X01;            //Se activa el enable (bit0 PORTC)
	asm("OUT 0x08, R16");
	_delay_ms(50);
	asm("LDI R16,0x00");		//PORTC = 0X00;            //Se baja el enable
	asm("OUT 0x08, R16");
	_delay_ms(50);
}



void escribir_lcd(char caracter)
{
	asm("LDI R16,0x01");		//PORTD = 0X01;				//Se activa pin conectado a RS PARA ACTIVAR MODO VISUALIZACION
	asm("OUT 0x0B, R16");
	PORTB = caracter;			//Se configura puerto B con el valor del caracter
	pulso();
	_delay_ms(50);
	asm("LDI R16,0x00");		//PORTD = 0X00;
	asm("OUT 0x0B, R16");
}

void enter_lcd()
{
	asm("LDI R16,0xc0");		//PORTB=0xC0;		//POSICION 1 FILA 2
	asm("OUT 0x05, R16");
	pulso();
}
	
	
