# arduino-turret
Arduino Code for a simple nerf turret

Librarys
* RF24 by TMRh20
* LiquidCrystal I2C


Usecase der Lybarys
* RF24 by TMRh20 -- Funken
* LiquidCrystal I2C -- Display

RF24 Funkmodul
`` ┌───────────────┐
 │ NRF24L01      │
 │               │
 │  ┌100µF┐      │
 │ GND   VCC     │
 │ CE    CSN     │
 │ SCK   MOSI    │
 │ MISO  IRQ     │
 └───────────────┘``