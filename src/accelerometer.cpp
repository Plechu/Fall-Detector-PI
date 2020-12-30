#include <accelerometer.h>

void Vector::update(float x, float y, float z){
    X = x;
    Y = y;
    Z = z;
}  
void Vector::makeVersor(){
    float module = sqrt(X*X + Y*Y + Z*Z);
    X = X/module;
    Y = Y/module;
    Z = Z/module;
}

bool Accelerometer::getStatus(){
    bool status = true;
    accel.begin();
    delay(100);
    status &= accel.writeRate(ADXL345_RATE_100HZ);
    delay(100);
    status &= accel.writeRange(ADXL345_RANGE_16G);
    delay(100);
    if(accel.start()){
        delay(100);
        accel.update();
        reference.update(accel.getX(), accel.getY(), accel.getZ());
        if(isnan(reference.X) || isnan(reference.Y) || isnan(reference.Z)) return false;
        reference.makeVersor();
        return status;
    }
    return false;
}

bool Accelerometer::setActualVector(){  
    if(accel.update()){
        actual.update(accel.getX(), accel.getY(), accel.getZ());
        return true;
    }
    return false; 
}

bool Accelerometer::isFall(){ 
    if(sqrt(actual.X*actual.X + actual.Y*actual.Y + actual.Z*actual.Z) > impactTh){
        delay(2000);
        setActualVector();
        actual.makeVersor();
        return (57.2957795 * acos(reference.X*actual.X + reference.Y*actual.Y + reference.Z*actual.Z)) > angleTh;
    }
    return false;
}