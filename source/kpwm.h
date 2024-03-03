//2024-02-25

// kpwm.h
// Kernel PWM
int kpwm_export(int chan);
int kpwm_unexport(unsigned chan);
unsigned kpwm_get_period(unsigned chan);
void kpwm_enable(unsigned chan, unsigned enable);
void kpwm_start(unsigned chan, unsigned period, unsigned pulse_width);
void kpwm_start_f(unsigned chan, float frequency, float duty_cycle);
void kpwm_set_pulse_width(unsigned chan, unsigned pulse_width);
void kpwm_set_duty_cycle(unsigned chan, float duty_cycle);
