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

void stop(bool i)
{
	robotmotor.M1stop();
	robotmotor.M2stop();
	if (i){
		beep(1);
	}
}

void toggle(bool i)
{
	if (i){
		if (automove){
			automove = false;
			digitalWrite(LED_BUILTIN, LOW);
			stop(1);
		}
		else{
			automove = true;
			beep(0);
		}
	}
	else if (!i)
	{
		if (follow){
			follow = false;
			digitalWrite(LED_BUILTIN, LOW);
			stop(1);
		}
		else{
			follow = true;
			beep(0);
		}
	}
}

void BlueControl()
{
	byte bluedata;
	bluedata = Serial.read();
	switch (bluedata)
	{
	case '8'://Forward
		forward(speed, speed - OFFSET_SPEED);
		Serial.println();
		break;

	case '4'://left
		left(speed, speed);
		Serial.println();
		break;

	case '2'://bacK
		back(speed, speed - OFFSET_SPEED);
		Serial.println();
		break;

	case '6'://right
		right(speed, speed);
		Serial.println();
		break;

	case '5'://stop
		stop(1);
		Serial.println();
		break;

	case '0'://switch auto mode
		toggle(1);
		Serial.println();
		break;

	case '9':
		Serial.print(F("ROBOT VERSION: ")); Serial.println(RVER);
		break;

	case '1':
		toggle(0);
		Serial.println();
		break;

	case '3':
		pingDebug();
		break;

#ifdef SERVOS
	case '7':
		if (pos >= 10){
			myservo.write(pos -= 10);
			Serial.println();
			break;
		}
		Serial.println(F("servo limit reached"));
		break;
	case '9':
		if (pos <= 170){
			myservo.write(pos += 10);
			Serial.println();
			break;
		}
		Serial.println(F("servo limit reached"));
		break;
	case '1':
		if (pos >= 10){
			myservo2.write(pos -= 10);
			Serial.println();
			break;
		}
		Serial.println(F("servo2 limit reached"));
		break;
	case '3':
		if (pos <= 170){
			myservo2.write(pos += 10);
			Serial.println();
			break;
		}
		Serial.println(F("servo2 limit reached"));
		break;
#endif
	default: return;
	}
}

void SonarControl()
{
	unsigned int distance_L = sonar_L.convert_cm(sonar_L.ping_median()); // Send ping, get distance in cm, 0 if > MAX_DISTANCE.
	unsigned int distance_R = sonar_R.convert_cm(sonar_R.ping_median());
	//unsigned int distance_F = sonar_F.convert_cm(sonar_F.ping_median());
#ifdef DEBUG_M
	/*Serial.print(F("Ping_F: "));
	Serial.print(distance_F);*/
	Serial.println(F("cm"));
	Serial.print(F("Ping_L: "));
	Serial.print(distance_L);
	Serial.println(F("cm"));
	Serial.print(F("Ping_R: "));
	Serial.print(distance_R);
	Serial.println(F("cm"));
#endif
	/*if (distance_F <= OBSTACLE)
	{
		stop();
		while (sonar_F.convert_cm(sonar_F.ping_median()) <= OBSTACLE)
		{
			back(speed, speed);
		}
		return;
	}*/

	if (distance_L <= OBSTACLE && distance_L > STUCK && distance_R >= OBSTACLE)
	{
		if (firstrun)
		{
			firstrun = false;
			stop(1);
			return;
		}
		tone(TONE_PIN, 700, 200);
		digitalWrite(LED_BUILTIN, HIGH);
		while (sonar_L.convert_cm(sonar_L.ping_median()) < OBSTACLE)
		{
			right(speed, speed);
		}
		return;
	}
	if (distance_R <= OBSTACLE && distance_R > STUCK && distance_L >= OBSTACLE)
	{
		if (firstrun)
		{
			firstrun = false;
			stop(1);
			return;
		}
		tone(TONE_PIN, 500, 200);
		digitalWrite(LED_BUILTIN, HIGH);
		while (sonar_R.convert_cm(sonar_R.ping_median()) < OBSTACLE)
		{
			left(speed, speed);
		}
		return;
	}
	if (distance_L <= STUCK && distance_L != 0 || distance_R <= STUCK && distance_R != 0)
	{
		back(speed, speed - OFFSET_SPEED);
		delay(700);
		return;
	}
	digitalWrite(LED_BUILTIN, LOW);
	forward(speed, speed - OFFSET_SPEED);
}

void Follow()
{
	unsigned int distance_L = sonar_L.convert_cm(sonar_L.ping_median()); // Send ping, get distance in cm, 0 if > MAX_DISTANCE.
	unsigned int distance_R = sonar_R.convert_cm(sonar_R.ping_median());
#ifdef DEBUG_M
	/*Serial.print(F("Ping_F: "));
	Serial.print(distance_F);*/
	Serial.println(F("cm"));
	Serial.print(F("Ping_L: "));
	Serial.print(distance_L);
	Serial.println(F("cm"));
	Serial.print(F("Ping_R: "));
	Serial.print(distance_R);
	Serial.println(F("cm"));
#endif
	if (distance_L > 18 && distance_R > 18 || distance_L < 5 && distance_R < 5) //if both sensors read more than 18 cm or less than 5 cm, brake both motors
	{
		stop(0);
	}
	if (distance_L <= 18 && distance_R <= 18 && (distance_L >= 5 && distance_R >= 5)) //if both sensors read less than 18 cm, go forward
	{
		forward(speed, speed - OFFSET_SPEED);
	}
	if (distance_L < 18 & distance_R > 18) //if only something is in front of the left ultrasonic sensor, turn left
	{
		left(speed, speed);
	}
	if (distance_L > 18 & distance_R < 18) //if only something is in front of the right ultrasonic sensor, turn right
	{
		right(speed, speed);
	}
}

void beep(bool i)
{
	if (i)
	{
		tone(TONE_PIN, 700, 200);
		delay(200);
		tone(TONE_PIN, 1000, 200);
	}
	else
	{
		tone(TONE_PIN, 1000, 200);
		delay(200);
		tone(TONE_PIN, 700, 200);
	}
}

void pingDebug()
{
	delay(50);
	unsigned int d = sonar_L.convert_cm(sonar_L.ping_median()); // Send ping, get distance in cm, 0 if > MAX_DISTANCE.
	unsigned int d2 = sonar_R.convert_cm(sonar_R.ping_median());
	Serial.println();
	Serial.print(F("Ping_L: "));
	Serial.print(d);
	Serial.println(F("cm"));
	Serial.print(F("Ping_R: "));
	Serial.print(d2);
	Serial.println(F("cm"));
}
