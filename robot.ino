//#define DEBUG

#include <DualMotor.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

SoftwareSerial BT(14, 15); //A0 y A1 // RX, TX

DualMotor robotmotor;

byte BluetoothData;
bool blue = false;

void setup()
{
	//~30kHz on pwm pins 3 & 11
	// - minimize noise
	// - minimize heat (saturation of motor --> L293=RIP)
	TCCR2A = (0<<WGM21) | (1<<WGM20) | (0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0);
	TCCR2B = (0<<WGM22) | (0<<CS22) | (0<<CS21) | (1<<CS20) | (0<<FOC2A) | (0<<FOC2B);
#ifdef DEBUG
	Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
#endif
	BT.begin(9600);
	stop();
}

void loop()
{
	if (BT.available() > 0)
	{
		bluecontrol();
	}
	else if (blue == false)
	{
		delay(50);
		unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
#ifdef DEBUG
		Serial.print("Ping: ");
		Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
		Serial.println("cm");
#endif
		unsigned int distance = uS / US_ROUNDTRIP_CM;
		if (distance > 10)
		{
			forward(255,255);
			return;
		}
		else if (distance < 10)
		{
			back(255, 255);
			delay(2000);
			left(255, 255);
			delay(2000);
		}
	}
}

