void forward(byte speed, byte speed2)
{
	robotmotor.M1move(speed, 1);
	robotmotor.M2move(speed2, 1);
}

void back(byte speed, byte speed2)
{
	robotmotor.M1move(speed, 0);
	robotmotor.M2move(speed2, 0);
}

void left(byte speed, byte speed2)
{
	robotmotor.M1move(speed, 0);
	robotmotor.M2move(speed2, 1);
}

void right(byte speed, byte speed2)
{
	robotmotor.M1move(speed, 1);
	robotmotor.M2move(speed2, 0);
}

void stop()
{
	robotmotor.M1stop();
	robotmotor.M2stop();
}

void toggle()
{
	if (automove){
		automove = false;
		stop();
	}
	else{
		automove = true;
	}

}

void BlueControl()
{
	byte bluedata;
	bluedata = BT.read();
	switch (bluedata)
	{
	case '8'://Forward
		forward(OFFSET_SPEED, MAX_SPEED);
		BT.println();
		break;

	case '4'://left
		left(MAX_SPEED, MAX_SPEED);
		BT.println();
		break;

	case '2'://bacK
		back(OFFSET_SPEED, MAX_SPEED);
		BT.println();
		break;

	case '6'://right
		right(MAX_SPEED, MAX_SPEED);
		BT.println();
		break;

	case '5'://stop
		stop();
		BT.println();
		break;

	case '0'://switch auto mode
		toggle();
		BT.println();
		break;
#ifdef SERVOS
	case '7':
		if (pos >= 10){
			myservo.write(pos -= 10);
			BT.println();
			break;
		}
		BT.println(F("servo limit reached"));
		break;
	case '9':
		if (pos <= 170){
			myservo.write(pos += 10);
			BT.println();
			break;
		}
		BT.println(F("servo limit reached"));
		break;
	case '1':
		if (pos >= 10){
			myservo2.write(pos -= 10);
			BT.println();
			break;
		}
		BT.println(F("servo2 limit reached"));
		break;
	case '3':
		if (pos <= 170){
			myservo2.write(pos += 10);
			BT.println();
			break;
		}
		BT.println(F("servo2 limit reached"));
		break;
#endif
	default: return;
	}
}

void SonarControl()
{
	delay(100);
	unsigned int distance = sonar.ping_cm(); // Send ping, get distance in cm, 0 if > MAX_DISTANCE.
	//unsigned int distance2 = sonar2.ping_cm();
#ifdef DEBUG
	Serial.print(F("Ping: "));
	Serial.print(distance);
	Serial.println(F("cm"));
	/*Serial.print(F("Ping2: "));
	Serial.print(distance2);
	Serial.println(F("cm"));*/
#endif
	if (distance < OBSTACLE && distance >= STUCK)
	{
		left(MAX_SPEED, MAX_SPEED);
	}
	else if (distance < STUCK && distance != 0 )//&& (distance2 > OBSTACLE || distance2 == 0))
	{
		digitalWrite(LED_BUILTIN, HIGH);
		back(OFFSET_SPEED, MAX_SPEED);
		delay(1000);
		right(0, MAX_SPEED);
	}
	/*else if (distance2 < OBSTACLE && distance2 >= STUCK)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		left(MAX_SPEED, 0);
	}
	else if (distance2 < STUCK && distance2 != 0)
	{
		right(0, MAX_SPEED);
	}*/
	else {
		digitalWrite(LED_BUILTIN, LOW);
		forward(OFFSET_SPEED, MAX_SPEED);
	}
}