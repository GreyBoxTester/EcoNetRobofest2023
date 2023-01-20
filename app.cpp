#include "app.h"
#include "Robot.h"
#include "Protocol.h"
#include <atomic>

static Packet currentPacket;
static Robot robot;
//atomic
static bool executionDone = true;

void emergencyButtonHandler(intptr_t unused)
{
	ter_tsk(MAIN_TASK);
	ter_tsk(EXECUTION_TASK);
	ev3::Brick::setButtonHandler(ev3::BrickButton::Down, nullptr, 0);
	robot.emergencyStop();
}

void executionTask(intptr_t unused)
{
	switch (currentPacket.cmd)
	{
	case Command::DriveForward: robot.driveForward(); break;
	case Command::DriveBackward: robot.driveBackward(); break;
	case Command::TurnLeft: robot.turnLeft(); break;
	case Command::TurnRight: robot.turnRight(); break;
	case Command::Stop: robot.stop(); break;
	case Command::GrabRubbish: robot.grabAndIdentifyRubbish(); break;
	case Command::PlaceRubbish: robot.placeRubbish(); break;
	case Command::SetPower: robot.setDrivePower(currentPacket.data); break;
	}
	executionDone = true;
}

void mainTask(intptr_t unused)
{
	ev3::Brick::setButtonHandler(ev3::BrickButton::Down, emergencyButtonHandler, 0);
	ev3::Speaker::playTone(ev3::Note::A4, 50);
	ev3::BluetoothSlave bt;
	while (!bt.connected()) { ev3::Time::delay(10); }
	ev3::Speaker::playTone(ev3::Note::A4, 50);
	ev3::Console::write("connected");

	while (bt.connected())
	{
		Packet p;
		bt.readBytes(&p, sizeof(Packet));
		ev3::Console::write(">%d %d", p.cmd, p.data);
		if (p.cmd == Command::EmergencyStop) 
		{ 
			ter_tsk(EXECUTION_TASK);
			ev3::Brick::setButtonHandler(ev3::BrickButton::Down, nullptr, 0);
			robot.emergencyStop();
			break; 
		}

		ev3::Time::delay(10);

		if (!executionDone) { continue; }
		executionDone = false;
		currentPacket = p;
		act_tsk(EXECUTION_TASK);
	}

	ev3::Console::write("disconnected");

	/*Robot robot;

	ev3::Speaker::playTone(ev3::Note::A4, 50);
	ev3::BluetoothSlave bt;
	while (!bt.connected()) { ev3::Time::delay(10); }
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

	ev3::Console::write("disconnected");*/

	/*ev3::SoundFile plasticSound("ev3rt/res/plastic.wav");
	ev3::SoundFile metalSound("ev3rt/res/metal.wav");
	ev3::SoundFile paperSound("ev3rt/res/paper.wav");

	Robot robot;

	while (true)
	{
		bool enterState, backState;
		do 
		{
			enterState = ev3::Brick::isButtonPressed(ev3::BrickButton::Enter);
			backState = ev3::Brick::isButtonPressed(ev3::BrickButton::Back);
			ev3::Time::delay(100); 
		} while (!enterState && !backState);

		if (backState) { break; }

		RubbishType rubbish = robot.grabAndIdentifyRubbish();

		ev3::Time::delay(100);
		switch (rubbish)
		{
		case RubbishType::Bottle: ev3::Speaker::playSoundFile(plasticSound); break;
		case RubbishType::Can: ev3::Speaker::playSoundFile(metalSound); break;
		case RubbishType::Paper: ev3::Speaker::playSoundFile(paperSound); break;
		}
		ev3::Time::delay(100);

		robot.placeRubbish();
		robot.driveBack();
	}*/
}
