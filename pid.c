#include <pid.h>
#include <stdio.h>

float pid_roll(float target, float measured)
{
	float err;
	float dt = .085;
	float output;
	float err_diff;

	err = target - measured;
	err_diff = err - err_prev;
	err_tot = err_tot + err;

	output = Kp * err; //+ (Ki * err_tot * dt) + (Kd * err_diff /dt);

	err_prev = err;
}

float pid_pitch(float target, float measured);
float pid_yaw(float target, float measured);
