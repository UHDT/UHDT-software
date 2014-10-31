#define Kp 1
#define Kd 1
#define Ki 1

extern float err_tot;
extern float err_prev;

float pid_roll(float target, float measured);
float pid_pitch(float target, float measured);
float pid_yaw(float target, float measured);
