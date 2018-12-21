/*
 * This is a modified version of the DualMC33926MotorShield.h library ported by Chris Bove for use with the individual 
 * motor controllers on the MotorShield after soldering on headers on the individual D2 and SF pins
 * 
 * Author: Sean O'Neil
*/

#ifndef SingleMC33926MotorController_h
#define SingleMC33926MotorController_h

#include "mbed.h"

class SingleMC33926MotorController
{

public:
	// CONSTRUCTOR - 
	SingleMC33926MotorController(PinName DIR, PinName PWM, PinName FB, PinName nD2, PinName nSF);

	// DESTRUCTOR
	~SingleMC33926MotorController();

	// Public Methods
	void enable(); // enables the motor controller
	void disable(); // disables the motor controller
	void setSpeedCoast(float speed); // PWMs the enable pin for a Coast-drive mode
	void setSpeedBrake(float speed); // PWMs the PWM pin for a brake drive mode
	float getCurrent(); // return the current in Amps
	bool hasFault(); // detect if there is a fault with the motor controller

private: 
	// Private methods
	void setSpeed(float speed, bool coast);

	// Private members
	PinName _DIR;
    PinName _PWM;
    PinName _FB;

	PinName _nD2; // enable line
	PinName _nSF; // fault input line. 

    DigitalOut * _DirOut;
    PwmOut * _nD2Out;
    PwmOut * _PwmOut;
    AnalogIn * _FbAnalogIn;
    DigitalIn * _nSfDigitalIn; // TODO make this InterruptIn
};

#endif