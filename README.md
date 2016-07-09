# nrlf24l01-pi
Example to communicate two NRLF2401+ modules from a Raspberry Pi. For test purpose only.

## Basics
Declare your RF24 object and set it up:
```cpp
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ); 
// Configuramos la radio
set_up_radio(radio);
```

Send commands using "escribir_mensaje()":

```cpp
escribir_mensaje(buffer_tx, RXTX_PLOAD_WIDTH, radio);
```

And read responses (if any) using "leer_mensaje()".
```cpp
leer_mensaje(buffer_rx, RXTX_PLOAD_WIDTH, radio);
```
