//
// Created by edward on 2021/11/16.
//

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fstream>
#include "log.h"
#include "gpio.h"

/* Raspberry Pi GPIO memory */
#define BCM2835_PERI_BASE   0x20000000
#define BCM2837_PERI_BASE   0x3F000000
#define BCM2711_PERI_BASE   0xFE000000
#define GPIO_BASE(address)  (address + 0x200000)
#define PAGE_SIZE           (4*1024)
#define BLOCK_SIZE          (4*1024)

/* GPIO setup. Always use INP_GPIO(x) before OUT_GPIO(x) or SET_GPIO_ALT(x,y) */
#define GPIO_MODE_IN(g)     *(gpio_+((g)/10)) &= ~(7<<(((g)%10)*3))
#define GPIO_MODE_OUT(g)    *(gpio_+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_MODE_ALT(g,a)  *(gpio_+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET_HIGH       *(gpio_+7)  // sets   bits which are 1
#define GPIO_SET_LOW        *(gpio_+10) // clears bits which are 1
#define GPIO_GET(g)         (*(gpio_+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define SYSFS_RPI_MODEL_FILE_PATH "/sys/firmware/devicetree/base/model"

using namespace Navio;

Pin::Pin(uint8_t pin):
pin_(pin),
gpio_(nullptr),
mode_(GPIOModeInput) {

}

Pin::~Pin() {
    if(!deinit_()){
        LOG("deinitialization failed\n");
    }
}

bool Pin::init() {
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(mem_fd == -1){
        LOG("/dev/mem cannot be opened:%s\n", strerror(errno));
        return false;
    }
    int rpi_version = 0;
    try{
        rpi_version = getRaspberryPiVersion();
    }catch (std::runtime_error& e){
        LOG("err:%s\n", e.what());
        return false;
    }
    uintptr_t address = 0;
    switch (rpi_version) {
        case 1:
            address = GPIO_BASE(BCM2835_PERI_BASE);
            break;
        case 2:
        case 3:
            address = GPIO_BASE(BCM2837_PERI_BASE);
            break;
        case 4:
            address = GPIO_BASE(BCM2711_PERI_BASE);
            break;
        default:
            LOG("Unexpected version:%d\n", rpi_version);
            return false;
    }
    void *gpio_map = mmap(
            nullptr,
            BLOCK_SIZE,
            PROT_READ|PROT_WRITE,
            MAP_SHARED,
            mem_fd,
            address
            );
    if(gpio_map == MAP_FAILED){
        LOG("cannot mmap memory");
        return false;
    }
    /*No need to keep mem_fd open after mmap*/
    if(close(mem_fd) < 0){
        LOG("cannot close mem_fd:%s\n", strerror(errno));
        return false;
    }
    gpio_ = reinterpret_cast<volatile uint32_t*>(gpio_map);
    return true;
}

int Pin::getRaspberryPiVersion() const {
    std::string content;
    std::ifstream f(SYSFS_RPI_MODEL_FILE_PATH);
    if(!f){
        LOG("could not open %s\n", SYSFS_RPI_MODEL_FILE_PATH);
        return 0;
    }
    while (f >> content){
        try{
            return std::stoi(content);
        }catch (std::invalid_argument){

        }
    }
    LOG("Could not read RPi version!\n");
    return 0;
}

bool Pin::deinit_() {
    if(munmap(const_cast<uint32_t*>(gpio_), BLOCK_SIZE) < 0){
        LOG("unmap failed\n");
        return false;
    }
    return false;
}
void Pin::setMode(GPIOMode mode) {
    if(mode == GPIOModeInput){
        GPIO_MODE_IN(pin_);
    }else{
        GPIO_MODE_IN(pin_);
        GPIO_MODE_OUT(pin_);
    }
    mode_ = mode;
}

uint8_t Pin::read() const {
    uint32_t value = GPIO_GET(pin_);
    return value ? 1 : 0;
}

void Pin::write(uint8_t value) {
    if(mode_ != GPIOModeOutput){
        LOG("no effect because mode is not set\n");
    }
    if (value == LOW){
        GPIO_SET_LOW = 1 << pin_;
    }else{
        GPIO_SET_HIGH = 1 << pin_;
    }
}

void Pin::toggle() {
    write(!read());
}