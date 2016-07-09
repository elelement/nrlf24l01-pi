#include <cstdlib>
#include <signal.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <RF24/RF24.h>
#include "nRF24L01.h"

using namespace std;

#define RXTX_PLOAD_WIDTH 32
#define MAXLINEA 256

void escribir_mensaje(const uint8_t* buffer, uint8_t length, RF24& radio) ;
void leer_mensaje(uint8_t* buffer, uint8_t length, RF24& radio);
void set_up_radio(RF24& radio);

int main()
{
	RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ); // Medido CLK SPI = 2MHz

    // Configuramos la radio
	set_up_radio(radio);

	// Bucle principal
	string operacion;
	uint8_t buffer_tx[RXTX_PLOAD_WIDTH];
	uint8_t buffer_rx[RXTX_PLOAD_WIDTH];


	while(true)
	{
        memset(buffer_tx, 0, RXTX_PLOAD_WIDTH);
        memset(buffer_rx, 0, RXTX_PLOAD_WIDTH);

		cout << endl << "---- Introduzca un comando en hexadecimal y pulse la tecla \"intro\": ----" << endl << endl;
		// Introducimos la opción
		cin >> operacion;

		// read in the string
		int cont = 0;
        for (uint8_t i = 2; i < RXTX_PLOAD_WIDTH; i += 2) 
        {
            buffer_tx[cont++] = (uint8_t) strtoul(operacion.substr(i, 2).c_str(), NULL, 16);
            printf("enviado : %2X\n", buffer_tx[cont-1]);
        }

        escribir_mensaje(buffer_tx, RXTX_PLOAD_WIDTH, radio);

        cout << endl << endl;

        leer_mensaje(buffer_rx, RXTX_PLOAD_WIDTH, radio);

        cout << endl << endl;
            // display payload
        for (uint8_t j = 0; j < RXTX_PLOAD_WIDTH; j ++) 
        {
            printf("recibido(%i) : %2X\n", j, buffer_rx[j]);
        }

        cout << endl << endl << endl;


		cout << endl << "Pulse control-c para salir." << endl;
	}
}

void set_up_radio(RF24& radio)
{
    uint8_t RXTX_ADDR[5] = { 0x5A, 0xA5, 0xB3, 0xC4, 0xD5 };

    radio.begin();

    // Esto son configuraciones específicas que yo le he dado. Quitar o adaptar para fines propios.
    // These are specific settings I've used for the test. Remove or adapt them to your own purposes.

    radio.write_register(CONFIG, 0x7B);         //1 BYTE CRC, POWER UP, PRX
    radio.write_register(EN_AA, 0x00);          //Disable auto ack
    radio.write_register(EN_RXADDR, 0x01);      //Enable data pipe 0
    radio.write_register(SETUP_AW, 0x03);       //5 BYTE address
    radio.write_register(SETUP_RETR, 0x00);     //Retransmit disabled
    radio.write_register(RF_CH, 0x01);          //Randomly chosen RF channel -1
    radio.write_register(RF_SETUP, 0x07);       //1Mbps, 0dBm, con LNA
    radio.write_register(RX_PW_P0, 0x20);       //RX payload = 32 BYTES

    radio.write_register(RX_ADDR_P0, RXTX_ADDR, 5);
    radio.write_register(TX_ADDR, RXTX_ADDR, 5);

    // Abrimos el canal de lectura
    radio.openReadingPipe(0, RXTX_ADDR);

    // Imprimimos la configuración creada
    radio.printDetails();

    radio.flush_tx();
    radio.flush_rx();
}

void escribir_mensaje(const uint8_t* buffer, uint8_t length, RF24& radio)
{
    radio.flush_tx();
    radio.stopListening();
    radio.write(buffer, RXTX_PLOAD_WIDTH, true);
}

void leer_mensaje(uint8_t* buffer, uint8_t length, RF24& radio)
{
	radio.startListening();

    cout << "Esperando..." << endl;

    unsigned long started_waiting_at = millis();
    while ( !radio.ReadDataAvailable() && (millis() - started_waiting_at) <= 250);

    // Leer el mensaje
    uint8_t len = radio.getDynamicPayloadSize();
    radio.read( buffer, len);

    radio.flush_rx();
}

