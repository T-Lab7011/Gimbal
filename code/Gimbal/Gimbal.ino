#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <Servo.h>

#define INTERRUPT_PIN 2

const int SERVO_PIN_9 = 9;
const int SERVO_PIN_10 = 10;
const int SERVO_CENTER = 100;

MPU6050 mpu;
Servo servo9;
Servo servo10;

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
float ypr[3];

volatile bool mpuInterrupt = false;

void dmpDataReady() {
  mpuInterrupt = true;
}

void setup() {
  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(38400);
  while (!Serial)
    ;

  servo9.attach(SERVO_PIN_10);
  servo10.attach(SERVO_PIN_9);
  servo9.write(SERVO_CENTER);
  servo10.write(SERVO_CENTER);

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  devStatus = mpu.dmpInitialize();

  // Adjust these offsets for your own MPU6050 if the output is unstable.

  mpu.setXAccelOffset(3243);
  mpu.setYAccelOffset(5673);
  mpu.setZAccelOffset(9927);

  mpu.setXGyroOffset(67);
  mpu.setYGyroOffset(-13);
  mpu.setZGyroOffset(-1);


  if (devStatus == 0) {
    mpu.setDMPEnabled(true);

    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
}

void loop() {
  if (!dmpReady) return;

  if (readMPU6050()) {
    int servo9Angle = angleFromTilt(ypr[2]);
    int servo10Angle = angleFromTilt(ypr[1]);

    servo9.write(servo9Angle);
    servo10.write(servo10Angle-8);

    Serial.print(ypr[1]);
    Serial.print(",");
    Serial.print(ypr[2]);
    Serial.print(" -> ");
    Serial.print(servo9Angle);
    Serial.print(",");
    Serial.println(servo10Angle);
  }
}

int angleFromTilt(float tilt) {
  return constrain((int)(SERVO_CENTER - tilt), 0, 180);
}

bool readMPU6050() {
  while (!mpuInterrupt && fifoCount < packetSize) {
    if (mpuInterrupt && fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
    mpu.resetFIFO();
    fifoCount = mpu.getFIFOCount();
    Serial.println(F("FIFO overflow!"));
    return false;
  }

  if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    ypr[0] = 0;
    ypr[1] = ypr[1] * 180 / M_PI;
    ypr[2] = ypr[2] * 180 / M_PI;
    return true;
  }

  return false;
}
