//
// Created by edward on 2021/11/17.
//

#ifndef ZERO_INTERNALSENSOR_H
#define ZERO_INTERNALSENSOR_H

namespace Navio{
    class InternalSensor{
    public:
        virtual bool initialize() = 0;
        virtual bool probe() = 0;
        virtual void update() = 0;

        float read_temperature(){
            return temperature_;
        }
        void read_accelerometer(float *ax, float *ay, float *az){
            *ax = ax_; *ay = ay_; *az = az_;
        }
        void read_gyroscope(float *gx, float *gy, float *gz){
            *gx = gx_; *gy = gy_; *gz = gz_;
        }
        void read_magnetometer(float *mx, float *my, float *mz){
            *mx = mx_; *my = my_; *mz = mz_;
        }

    protected:
        float temperature_;
        float ax_, ay_, az_;
        float gx_, gy_, gz_;
        float mx_, my_, mz_;
    };
}

#endif //ZERO_INTERNALSENSOR_H
