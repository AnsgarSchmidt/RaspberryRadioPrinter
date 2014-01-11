#ifndef SPI_H_
#define SPI_H_

int  init_spi(void);
void close_spi(void);
void loadWS2803(uint8_t led[nLEDs]);
void clearWS2803(void);

#endif
