;
; Arias_Gallego_Laboratorio1_v2.asm
;
; Author : jessi
;

;
; Arias_Gallego_Laboratorio1.asm
;
; Author : jessi
;


;====================================================================
; DEFINICIONES INICIALES
;====================================================================     
	.DEF TEMP=R16
	.DEF TEMP0=R17
	.DEF S=R18						; variable segundos pasados en hex.
									; Se trabaja display 7 segmentos catodo comun
	.DEF K=R19						; Crear constante 1

	.DEF P=R20						; Variable numero pulsos

;====================================================================
; VECTORES DE INTERRUPCION
;====================================================================     
	.ORG 0x0000
	JMP INICIO

	.ORG 0x0002
	JMP INT_EXTERNA

	.ORG 0x0028
	JMP INT_TEMP
;====================================================================
; CONFIGURACIÓN DE VARIABLES Y UBICACIONES DE MEMORIA EXTERNA PARA GUARDAR DATOS.
;====================================================================
	INICIO:
			LDI S, 0x00			; Segundos inicio 0.
			LDI P, 0x00			; Pulsos inicio 0.
			LDI TEMP0, 0x3F		; 0 en 7segmentos catodo comun
			LDI K, 0x01			; Constante K=1

			LDI R27, 0x80		; Registro X=0x8000, direccion display segundos.
			LDI R26, 0X00
			LDI R29, 0X81		; Registro Y=0x8100, direccion display numero de pulsos.
			LDI R28, 0X00

			LDI TEMP, 0x80	; Habilitar memoria externa en pines PortA
			STS XMCRA,TEMP

			LDI TEMP, 1<<XMM2|1<<XMM1|0<<XMM0
			STS XMCRB, TEMP

			ST X, TEMP0		; Direccion display segundos 0x8000, direccion display pulsos 0x8100, direccion pulsador 0x8200
			ST Y, TEMP0		; Definir segundos y pulsos inicial en 0, en 7segmentos y guardarlos en memoria externa, conectado a display
			
			LDI TEMP, 0XFF
			OUT PORTD, TEMP
			
;====================================================================
; CONFIGURACIÓN DE PERIFÉRICOS: TIMER 1
;====================================================================
			LDI TEMP, 0xC2			; Iniciar Temporizador en C2F7, para desbordar 1s
			LDI TEMP0, 0xF7
			STS TCNT1H, TEMP
			STS TCNT1L, TEMP0

			LDI TEMP, 1<<CS12|0<<CS11|1<<CS10	; Divisor de frecuecnia de 1024
			STS TCCR1B, TEMP

;====================================================================
; HABILITAR INTERRUPCIONES INT0, TIMER 1 Y GLOBALES
;====================================================================	
			LDI TEMP, 1<<TOIE1		; Habilitar interrupcion timmer, con desbordamiento 1s.
			STS TIMSK1, TEMP

			LDI TEMP, 1<<INT0		; Habilitar interrupcion externa
			OUT EIMSK, TEMP

			LDI TEMP, 0x02			; Interrupción por flanco de bajada
			STS EICRA, TEMP

			SEI
;====================================================================
; BUCLE INFINITO: LOOP
;====================================================================	
			BUCLE:		NOP
						JMP BUCLE
;====================================================================
; Subrutina de Atención a la Interrupción del Timer 1
;====================================================================	
		INT_TEMP:		LDI TEMP, 0xC2			; Volver a iniciar Temporizador en C2F7, para desbordar 1s
						LDI TEMP0, 0xF7
						STS TCNT1H, TEMP
						STS TCNT1L, TEMP0

						MOV TEMP, S
						CALL AUMENTAR
						MOV S, TEMP
						CALL HEX_7SEG

						ST X, TEMP0			; Display cambia cada segundo.
						
						RETI
;====================================================================
; Subrutina de Atención a la Interrupción Externa 0
;====================================================================	
		INT_EXTERNA:	MOV TEMP, P
						CALL AUMENTAR
						MOV P,TEMP
						CALL HEX_7SEG
						

						ST Y, TEMP0			; Display cambia cada pulso.

						RETI
;====================================================================
; Subrutinas aumentar y transformar a codigo 7 segmentos
;====================================================================	
		AUMENTAR:
						ADD TEMP, K
						CPI TEMP, 0x0A
						BRLO FIN_AUM
						LDI TEMP, 0x00

		FIN_AUM:		RET

		HEX_7SEG:		
						CERO:		CPI TEMP,0x00
									BRNE UNO
									LDI TEMP0, 0x3F
									JMP FIN_HEX_7SEG

						UNO:		CPI TEMP,0x01
									BRNE DOS
									LDI TEMP0, 0x06
									JMP FIN_HEX_7SEG

						DOS:		CPI TEMP,0x02
									BRNE TRES
									LDI TEMP0, 0x5B
									JMP FIN_HEX_7SEG

						TRES:		CPI TEMP,0x03
									BRNE CUATRO
									LDI TEMP0, 0x4F
									JMP FIN_HEX_7SEG

						CUATRO:		CPI TEMP,0x04
									BRNE CINCO
									LDI TEMP0, 0x66
									JMP FIN_HEX_7SEG

						CINCO:		CPI TEMP,0x05
									BRNE SEIS
									LDI TEMP0, 0x6D
									JMP FIN_HEX_7SEG

						SEIS:		CPI TEMP,0x06
									BRNE SIETE
									LDI TEMP0, 0x7D
									JMP FIN_HEX_7SEG

						SIETE:		CPI TEMP,0x07
									BRNE OCHO
									LDI TEMP0, 0x07
									JMP FIN_HEX_7SEG

						OCHO:		CPI TEMP,0x08
									BRNE NUEVE
									LDI TEMP0, 0x7F
									JMP FIN_HEX_7SEG

						NUEVE:		LDI TEMP0, 0x6F
									JMP FIN_HEX_7SEG
		
		FIN_HEX_7SEG:	RET


;======================================================================
; FIN DEL PROGRAMA
;======================================================================

