#include "MotorSpeedsStruct.h"

class WallFollow{
  private:
    int targetDist;
    int lastReading;
    uint16_t prevTime;
  
    int pinNum;
    int baseSpeed;
    
    int errorSum = 0;
    float Kp;
    float Kd;
    float Ki;
    float lastError = 0;
  public:
  WallFollow(){
    pinNum = WF_SensorPort;
    Kp = WF_Kp;
    Kd = WF_Kd;
    Ki = WF_Ki;
    targetDist = WF_Distance;
    this->baseSpeed = WF_BaseSpeed;
  }
  void Init(){
    pinMode(pinNum, INPUT); //INPUT for IR sensor
  }
  void setTarget(int distCM){
    targetDist = distCM;
  }
  MotorSpeeds calcSpeeds(){
    //Conversion function from Analog Val to distance from LAB1
    float distanceCm = pow((analogRead(pinNum)/2206.0),(-1/0.738));
    float error = targetDist - distanceCm;

    errorSum += error;

    uint16_t now = millis();
    uint16_t deltaTime = now-prevTime;
    prevTime = now;
    
    //Scale down deltaTime so D value is not crazy high
    float dir = (error - lastError) / (0.01* deltaTime);

    lastError = error;
    float steering = Kp*error + Kd*dir + Ki*errorSum;

    MotorSpeeds output;
    output.left = baseSpeed - steering;
    output.right = baseSpeed + steering;

    return output;
  }
};

