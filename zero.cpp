/**********************************************************
    > File Name: zero.cpp
    > Author:Edward
    > Mail:yuanx_LE@126.com
    > Created Time: 2021年11月16日 星期二 10时15分36秒
 **********************************************************/
#include <iostream>
#include <memory>
#include <rgbled.h>
#include <log.h>
#include <unistd.h>
#include <mpu9250.h>



int main(int argc, char *argv[]){
	std::unique_ptr<Navio::RGBLed> rgbLed(std::move(new Navio::RGBLed()));
    if(!rgbLed->initialize()){
        LOG("rgb led init failed\n");
        return -1;
    }

    std::unique_ptr<Navio::MPU9250> mpu9250(std::move(new Navio::MPU9250{}));
    if(!mpu9250->probe()){
        LOG("mpu9250 not enabled\n");
        return -1;
    }
    if(!mpu9250->initialize()){
        LOG("initialize mpu failed\n");
        return -1;
    }

    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    while (true){
        rgbLed->setColor(Navio::Black);

        mpu9250->update();
        mpu9250->read_accelerometer(&ax, &ay, &az);
        mpu9250->read_gyroscope(&gx, &gy, &gz);
        mpu9250->read_magnetometer(&mx, &my, &mz);
        usleep(500 * 1000);
        rgbLed->setColor(Navio::Blue);

        LOG("ACC: %+7.3f %+7.3f %+7.3f ", ax, ay, az);
        LOG("GYR: %+8.3f %+8.3f %+8.3f ", gx, gy, gz);
        LOG("MAG: %+7.3f %+7.3f %+7.3f\n", mx, my, mz);

    }


	return 0;
}
