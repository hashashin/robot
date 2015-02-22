/*
PIN DEFINITIONS
---------------
pwmPinM1 = D3
sensePinM1 = D5
sense2PinM1 = D6
pwmPinM2 = D11
sensePinM2 = D7
sense2PinM2 = D8
PING_PIN = D2
ECHO_PIN = D9
PING_PIN2 = D4
ECHO_PIN2  = D10
TONE_PIN = D12
PING_PIN_F = A2 (D16)
A0 & A1(D14, D15) = RX, TX bluetooth
LED_PIN = A3 (D17)
*/

//#define DEBUG_M
//#define SERVOS
//#define I2C_T

#define RVER 1

#include <DualMotor.h>
//#include <SoftwareSerial.h>
#include <NewPing.h>
//#include <SoftTimer.h>
//#include <Heartbeat.h>

#ifdef I2C_T
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#define OUTPUT_READABLE_ACCELGYRO
#endif

#ifdef SERVOS
//#include <Servo.h>
#endif

//#define PING_PIN_F 16
#define PING_PIN_L  2  // Arduino pin tied to trigger ping on the ultrasonic sensor.
#define ECHO_PIN_L 9
#define PING_PIN_R  4  // Arduino pin tied to trigger ping on the ultrasonic sensor.
#define ECHO_PIN_R 10
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400cm.

//#define LED_PIN 17
#define TONE_PIN 12

//MOTORS && CONTROL STUFF
#define MAX_SPEED 115
#define OFFSET_SPEED 5
#define OBSTACLE 30
#define STUCK 10

byte speed = MAX_SPEED;

//NewPing sonar_F(PING_PIN_F, PING_PIN_F, MAX_DISTANCE);
NewPing sonar_L(PING_PIN_L, ECHO_PIN_L, MAX_DISTANCE);
NewPing sonar_R(PING_PIN_R, ECHO_PIN_R, MAX_DISTANCE);

//SoftwareSerial BT(14, 15); //A0 y A1 // RX, TX

DualMotor robotmotor;

//Task task1(100, control);
//Heartbeat heartbeat(LED_PIN);

#ifdef SERVOS
Servo myservo;
Servo myservo2;
byte pos = 90;
byte pos2 = 90;
bool servos = false;
#endif

#ifdef I2C_T
MPU6050 mpu;
#endif

bool automove = false;
bool firstrun = true;
bool follow = false;

void setup()
{
	//~30Hz on pwm pins 3 & 11
	// - minimize noise
	// - minimize heat (saturation of motor --> L293=RIP)
	TCCR2B = TCCR2B & B11111000 | B00000111;
	pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG_M
	//Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
#endif
	Serial.begin(115200);
#ifdef SERVOS
	myservo.attach(9);
	myservo2.attach(10);
	myservo.write(pos);
	myservo2.write(pos2);
#endif
#ifdef I2C_T
	mpu.initialize();
	mpu.testConnection();
#endif
	stop(1);
	//SoftTimer.add(&task1);
	beep(1);
}

void loop()
{
	if (Serial.available() > 0)
	{
		BlueControl();
	}
	else if (automove)
	{
		SonarControl();
	}
	else if (follow && !automove)
	{
		Follow();
	}
}
