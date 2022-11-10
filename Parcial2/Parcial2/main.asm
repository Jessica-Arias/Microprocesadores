;
; Parcial2.asm
;
; Created: 22/04/2021 8:38:39
; Author : jessi
;



;====================================================================
; DEFINICIONES INICIALES
;====================================================================     
	.DEF TEMP=R16
	.DEF TEMP0=R17

	.DEF T=R18						; Variable tiempo transcurrido (ms)
	.DEF S=R19						; variable milisegundos pasados con señal alta.
	.DEF A=R20						; varible A=1, si señal arriba,

	.DEF K=R21						; Crear constante 1
;====================================================================
; VECTORES DE INTERRUPCION
;====================================================================     
	.ORG 0x0000
	JMP INICIO

	.ORG 0x0008
	JMP INT_EXTERNA

	.ORG 0x0028
	JMP INT_TEMP
;====================================================================
; CONFIGURACIÓN DE VARIABLES Y PERIFÉRICOS: PUERTOS A, B y D
;====================================================================
	INICIO:
			LDI S, 0x00				; Milisegundos inicio 0.
			LDI T, 0X00				; Tiempo inical 0ms
			LDI TEMP0, 0X00			; Iniciar puertos en 0.
			LDI K, 0x01				; Constante K=1
			LDI A, 0X00				; Señal inicia abajo, puerto D en 0.

			LDI TEMP, 0xFF			; Definir puertos A de salida
			OUT DDRA, TEMP

			OUT DDRD, TEMP

			OUT PORTD, TEMP0		; Iniciar D en 0
			OUT PORTA, TEMP0		
			
;====================================================================
; CONFIGURACIÓN DE PERIFÉRICOS: TIMER 1
;====================================================================
			LDI TEMP, 0xFC			; Iniciar Temporizador en FC18 =64536, para desbordar 1ms
			LDI TEMP0, 0x18
			STS TCNT1H, TEMP
			STS TCNT1L, TEMP0

			LDI TEMP, 0B010			; Divisor de frecuecnia de 8
			STS TCCR1B, TEMP

;====================================================================
; HABILITAR INTERRUPCIONES INT_EXT, TIMER 1 Y GLOBALES
;====================================================================	
			LDI TEMP, 1<<TOIE1		; Habilitar interrupcion timmer, con desbordamiento 1s.
			STS TIMSK1, TEMP

			LDI TEMP, 1<<INT3		; Habilitar interrupcion externa
			OUT EIMSK, TEMP

			LDI TEMP, 0x01			; Interrupción por cualquier flanco
			STS EICRA, TEMP

			SEI
;====================================================================
; BUCLE INFINITO: TAREA FONDO
;====================================================================	
			BUCLE:		NOP
						JMP BUCLE
;====================================================================
; Subrutina de Atención a la Interrupción del Timer 1
;====================================================================	
		INT_TEMP:		LDI TEMP, 0xFC			; Volver a iniciar Temporizador en FC18, para desbordar 1ms
						LDI TEMP0, 0x18
						STS TCNT1H, TEMP
						STS TCNT1L, TEMP0

						ADD T, K				; aumentar tiempo pasado
						CPI A, 0X01				; Verficar si A esta en 1
						BRNE FIN_INT_TEMP		; Si A esta en 0, acabe interrupción
						CPI S, 0XC8				; Comparar si es menor a 200.
						BRPL MAYOR				; Si es mayor reiniciar s, sino
						ADD S, K				; Aumentar variable s (milisegundos arriba)
						

						OUT PORTA, S			; Se imprime el ultimo valor de s.
						RETI

		MAYOR:			LDI S, 0X00			
		FIN_INT_TEMP:	RETI
;====================================================================
; Subrutina de Atención a la Interrupción Externa 3
;====================================================================	
		INT_EXTERNA:	CPI A, 0X01			; Como se activa con cada flanco, e inicia en 0
						BREQ AUMENTAR		; se compara el valor y si es 0, pasa a ser 1
						SUB A, K			; Si es 1, pasa a ser 0

						RETI

		AUMENTAR:		ADD A, K					

						RETI

;======================================================================
; FIN DEL PROGRAMA
;======================================================================
