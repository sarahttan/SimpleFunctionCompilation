#ifdef __cplusplus
extern "C" {
#endif

extern void init_spidev_control(uint32_t channel, uint32_t speed);
extern void spi_transfer(uint32_t channel, uint8_t tx[], uint8_t rx[], int len);
extern void exit_spidev_control();

#ifdef __cplusplus
}
#endif