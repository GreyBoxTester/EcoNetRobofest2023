#pragma once
#include <cstdint>

enum class Command : uint8_t
{
	DriveForward = 0,
	DriveBackward,
	TurnLeft,
	TurnRight,
	Stop,
	GrabRubbish,
	PlaceRubbish,
	AbortCurrentTask,
	EmergencyStop,
	SetPower
};

struct Packet
{
	Command cmd;
	uint8_t data;
};
