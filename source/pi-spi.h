//2025-06-12

//	c++ wrappers
#ifdef __cplusplus
extern "C" {
#endif

#ifndef PiSPI_H
#define PiSPI_H
int spiOpen(unsigned controller, unsigned channel, unsigned speed, unsigned mode);
int spiDataRW2(int channel, unsigned char *tx_data, unsigned char *rx_data, int len);
int spiDataRW (int channel, unsigned char *data, int len);
#endif /* PiSPI_H */

#ifdef __cplusplus
}
#endif
