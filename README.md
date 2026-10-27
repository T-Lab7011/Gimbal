# Gimbal

Arduino program and 3D models for the Gimbal project. The program uses an MPU6050 sensor and two servos to control the gimbal.

## Video

[Watch on YouTube Shorts](https://www.youtube.com/shorts/ZjIYJ557E4c)

## Repository Contents

- [`code/Gimbal/Gimbal.ino`](code/Gimbal/Gimbal.ino) — Arduino program
- [`stl/`](stl/) — STL files for 3D printing
- [`cad/gimbal.step`](cad/gimbal.step) — original STEP CAD model

## Arduino Program

Open [`code/Gimbal/Gimbal.ino`](code/Gimbal/Gimbal.ino) in the Arduino IDE. Keep the sketch inside the `Gimbal` folder.

The sketch requires the **I2Cdev**, **MPU6050** (with `MPU6050_6Axis_MotionApps20.h`), and **Servo** libraries. It also uses the board's built-in `Wire` library.

Current settings in the program:

| Setting | Value |
| --- | --- |
| MPU6050 interrupt pin | 2 |
| Servo signal pins | 9 and 10 |
| Servo center | 100 degrees |
| Serial baud rate | 38400 |

MPU6050 calibration offsets and servo adjustments are set in the sketch. Check them against your hardware before use.

## STL Files

- [`part_1.stl`](stl/part_1.stl)
- [`part_2.stl`](stl/part_2.stl)
- [`part_3.stl`](stl/part_3.stl)

