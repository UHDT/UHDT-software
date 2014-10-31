#include "Filter.h"

extern float Q_angle;
extern float Q_gyro;
extern float R_angle;

extern float bias;
extern float P[2][2];
extern float angle;

float kalman(float a_angle,float gyro_rate,float dt)
{
	//Calculate rate after compensating for drift
	float rate = gyro_rate - bias;
	//Calculate Angle from gyro rate
	angle += rate*dt;

	//Calculate Covariance Matrix
	P[0][0] += (dt*P[1][1]-P[1][0]-P[0][1]+Q_angle)*dt;
	P[0][1] -= P[1][1]*dt;
	P[1][0] -= P[1][1]*dt;
	P[1][1] += Q_gyro*dt;

	//Calculate Innovation Matrix
	float S = P[0][0] + R_angle;

	//Calculate Kalman Gains
	float K_0 = P[0][0]/S;
	float K_1 = P[1][0]/S;

	//Find difference in new estimated angle from previous angle
	float y = a_angle - angle;

	//Update angle and bias according to Kalman gains
	angle += K_0*y;
	bias += K_1*y;

	//Update new covariance matrix according to Kalman gains
	P[0][0] -= K_0*P[0][0];
	P[0][1] -= K_0*P[0][1];
	P[1][0] -= K_1*P[0][0];
	P[1][1] -= K_1*P[0][1];

	//Return the kalman filtered angle
	return angle;
}
