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
	if (blue){
		blue = false;
	}
	else{
		blue = true;
		stop();
	}

}

void bluecontrol()
{
	BluetoothData = BT.read();
	switch (BluetoothData)
	{
	case '8'://Forward
		forward(255, 255);
		break;

	case '4'://left
		left(255, 255);
		break;

	case '2'://bacK
		back(255, 255);
		break;

	case '6'://right
		right(255, 255);
		break;

	case '5'://stop
		stop();
		break;

	case '0'://switch auto mode
		toggle();
		break;
	}
}
