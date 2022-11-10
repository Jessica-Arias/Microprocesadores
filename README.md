# Parcial 3 - Microprocesadores.

1. Se desea controlar el acceso a un parqueadero e indicar a los usuarios la capacidad disponible de lugares de
estacionamiento, figura 1. La capacidad del parqueadero es de 99, y se dispone de un sensor que genera un pulso cuando
ingresa un vehículo y otro sensor que genera un pulso cuando sale un vehículo del parqueadero:
 - Desarrolle el software para esta aplicación en lenguaje C para un microcontrolador ATMEGA2560. Considere un
cristal de 16 MHz.
 - La cantidad de cupos disponibles se debe visualizar cada 600ms por medio de 2 display de 7 segmentos de cátodo
común.
Suponga que para la visualización de los datos en los display dispone de una función que recibe un dato uint8_t entre
0 y 9, y retorna un uint8_t con el equivalente en código 7 segmentos: uint8_t conv_7seg (uint8_t dato) .
 - El parqueadero abre a las 8:00 a. m. y cierra a las 10:00 p. m., por lo tanto ha de desarrollar una aplicación de reloj
que encienda un led verde entre las 8:00 a.m. y las 10:00 p. m., y fuera de ese horario lo mantenga apagado.
 - Utilice interrupciones para la detección de los eventos de ingreso y salida de vehículos, y para el manejo de las
bases de tiempo que requiera para la solución.

