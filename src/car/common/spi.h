//
// Created by edward on 2021/11/17.
//

#ifndef ZERO_SPI_H
#define ZERO_SPI_H
#include <unistd.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <log.h>
namespace Navio{
    class SPI{
    public:
        SPI(){}

        static int transfer(
                const char *spidev,
                unsigned char *tx,
                unsigned char *rx,
                unsigned int length,
                unsigned int speed_hz = 1000000,
                unsigned char bits_per_word = 8,
                unsigned short delay_usecs = 0
                ){
            spi_ioc_transfer spiIocTransfer;
            memset(&spiIocTransfer, 0, sizeof(spi_ioc_transfer));
            spiIocTransfer.tx_buf = (unsigned long)tx;
            spiIocTransfer.rx_buf = (unsigned long)rx;
            spiIocTransfer.len = length;
            spiIocTransfer.speed_hz = speed_hz;
            spiIocTransfer.bits_per_word = bits_per_word;
            spiIocTransfer.delay_usecs = delay_usecs;

            int fd = open(spidev, O_RDWR);
            if(fd < 0){
                LOG("open %s error:%s\n", spidev, strerror(errno));
                return -1;
            }
            int status = ioctl(fd, SPI_IOC_MESSAGE(1),&spiIocTransfer);
            close(fd);
            return status;
        }
    };
}

#endif //ZERO_SPI_H
