#include "app.h"
#include "Robot.h"

enum class Command : uint8_t
{
	DriveForward = 0,
	DriveBackward,
	TurnLeft,
	TurnRight,
	Stop,
	GrabRubbish,
	PlaceRubbish,
	EmergencyStop,
	SetPower
};

void main_task(intptr_t unused)
{
	Robot robot;

	ev3::Speaker::playTone(ev3::Note::A4, 50);
	ev3::BluetoothSlave bt;
	while (!bt.connected()) { ev3::Time::sleep(10); }
	ev3::Speaker::playTone(ev3::Note::A4, 50);
	ev3::Console::write("connected");

	while (bt.connected())
	{
		Command command = (Command)bt.readByte();
		ev3::Console::write(">%d", command);
		if (command == Command::EmergencyStop) { robot.emergencyStop(); break; }
		switch (command)
		{
		case Command::DriveForward: robot.driveForward(); break;
		case Command::DriveBackward: robot.driveBackward(); break;
		case Command::TurnLeft: robot.turnLeft(); break;
		case Command::TurnRight: robot.turnRight(); break;
		case Command::Stop: robot.stop(); break;
		case Command::GrabRubbish: robot.grabAndIdentifyRubbish(); break;
		case Command::PlaceRubbish: robot.placeRubbish(); break;
		case Command::SetPower: robot.setDrivePower(bt.readByte()); break;
		}
	}

	ev3::Console::write("disconnected");


	/*Robot robot;
	switch (robot.grabAndIdentifyRubbish())
	{
	case RubbishType::Bottle: ev3::Speaker::playSoundFile(ev3::SoundFile("ev3rt/res/plastic.wav")); break;
	case RubbishType::Can: ev3::Speaker::playSoundFile(ev3::SoundFile("ev3rt/res/metal.wav")); break;
	case RubbishType::Paper: ev3::Speaker::playSoundFile(ev3::SoundFile("ev3rt/res/paper.wav")); break;
	}*/
}
