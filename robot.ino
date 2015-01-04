//#define DEBUG
#define SERVOS
#include <DualMotor.h>
#include <SoftwareSerial.h>
#include <NewPing.h>
#ifdef SERVOS
#include <Servo.h>
#endif
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400cm.
//#define TRIGGER_PIN2  16  // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN2     17  // Arduino pin tied to echo pin on the ultrasonic sensor.

//MOTORS && CONTROL STUFF
#define MAX_SPEED	180
#define OFFSET_SPEED MAX_SPEED - 25
#define OBSTACLE 20
#define STUCK OBSTACLE - 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
//NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

SoftwareSerial BT(14, 15); //A0 y A1 // RX, TX

DualMotor robotmotor;

#ifdef SERVOS
Servo myservo;
Servo myservo2;
byte pos = 90;
byte pos2 = 90;
bool servos = false;
#endif
bool automove = false;


void setup()
{
	//~30Hz on pwm pins 3 & 11
	// - minimize noise
	// - minimize heat (saturation of motor --> L293=RIP)
	TCCR2B = TCCR2B & (B11111000 | B00000111);
	pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG
	Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
#endif
	BT.begin(9600);
#ifdef SERVOS
	myservo.attach(9);
	myservo2.attach(10);
	myservo.write(pos);
	myservo2.write(pos2);
#endif
	stop();
}

void loop()
{
	if (BT.available() > 0)
	{
		BlueControl();
	}
	else if (automove == true)
	{
		SonarControl();
	}
}

