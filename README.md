# nrlf24l01-pi

Example showing how to use a NRLF2401+ radio module from a Raspberry Pi. I used it for testing purposes in a simple domotic system I'm building:

## Launch

Compile it first using:

```bash
make clean && make
```

Then execute it by simply typing (change the name in the makefile if you want):

```bash
sudo ./domomain
```

The program expects inputs in hexadecimal format up to 32 bytes. For example:

```
0x510332100313140000000
```

## Basics

Declare your RF24 object using "set_up_radio()":

```cpp
void set_up_radio(RF24& radio);
```

Example:
```cpp
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ); 
set_up_radio(radio);
```

Send commands using "escribir_mensaje()":

```cpp
void escribir_mensaje(const uint8_t* buffer, uint8_t length, RF24& radio) ;
```

Example:

```cpp
uint8_t buffer_tx[RXTX_PLOAD_WIDTH];
int cont = 0;
for (uint8_t i = 2; i < 32; i += 2) 
{
    buffer_tx[cont++] = (uint8_t) strtoul(operacion.substr(i, 2).c_str(), NULL, 16);
    printf("%2X ", buffer_tx[cont-1]);
}
escribir_mensaje(buffer_tx, 32, radio);
```

And read responses (if any) using "leer_mensaje()".
```cpp
void leer_mensaje(uint8_t* buffer, uint8_t length, RF24& radio);
```

Example:

```cpp
uint8_t buffer_rx[32];
leer_mensaje(buffer_rx, 32, radio);
for (uint8_t j = 0; j < 32; j ++) 
{
    printf("%2X ", buffer_rx[j]);
}
```
