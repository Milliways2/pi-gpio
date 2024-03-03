#! /usr/bin/env python3
"""
This code provides user space access to the PWM hardware.
    The Pi has 2 independent hardware PWM channels.
    The same  PWM channel is available on multiple pins but the output is identical
    Channel 0 can be accessed on GPIO 12, 18, 40, 52
    Channel 1 can be accessed on GPIO 13, 19, 41, 45, 53
    
The kernel PWM service needs to be started before use.

To set up PWM on GPIO 18/19 
    dtoverlay=pwm-2chan in config.txt 
    sudo dtoverlay pwm-2chan on command line

To set up PWM on GPIO 12/13
    dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
    sudo dtoverlay pwm-2chan  pin=12 func=4 pin2=13 func2=4

There is a service pwm to set up a single channel.
"""

# 2024-02-28

import sys, os, time
PWM_SYSFS = "/sys/class/pwm/pwmchip0"

def kpwm_export(chan) :
    """
    Export PWM channel
    chan: 0 or 1
    NOTE a short delay is needed following kpwm_export
    """
    if not os.path.isdir(PWM_SYSFS) :
        print("Overlay not loaded")
        exit()
    if os.path.isdir(f"{PWM_SYSFS}/pwm{chan}") :
        return  # already exported
    with open(f"{PWM_SYSFS}/export", "w") as f:
        f.write(f"{chan}")

def kpwm_unexport(chan) :
    """
    Unexport PWM channel
    chan: 0 or 1
    """
    with open(f"{PWM_SYSFS}/unexport", "w") as f:
        f.write(f"{chan}")

def kpwm_get_period(chan) :
    """
    chan: 0 or 1
    """
    with open(f"{PWM_SYSFS}/pwm{chan}/period", "r") as f:
        return int(f.read())

def kpwm_enable(chan, enable) :
    """
    Enable/disable PWM on chan
    chan: 0 or 1
    enable: 0 or 1
    """
    with open(f"{PWM_SYSFS}/pwm{chan}/enable", "w") as f:
        f.write(f"{enable}")

def kpwm_set_pulse_width(chan, pulse_width) :
    """
    chan: 0 or 1
    pulse_width: The period of the PWM signal in nanoseconds
    """
    with open(f"{PWM_SYSFS}/pwm{chan}/duty_cycle", "w") as f:
        f.write(f"{pulse_width}")

def kpwm_set_duty_cycle(chan, duty_cycle) :
    """
    chan: 0 or 1
    duty_cycle: 0.0 - 1.0
    """
    pulse_width = int(kpwm_get_period(chan) * duty_cycle)
    with open(f"{PWM_SYSFS}/pwm{chan}/duty_cycle", "w") as f:
        f.write(f"{pulse_width}")

def kpwm_start(chan, period, pulse_width) :
    """
    Enable the PWM on chan, setting period and pulse_width
    chan: 0 or 1
    period: The period of the PWM signal in nanoseconds
    pulse_width: The period of the PWM signal in nanoseconds
    """
    with open(f"{PWM_SYSFS}/pwm{chan}/period", "w") as f:
        f.write(f"{period}")
    with open(f"{PWM_SYSFS}/pwm{chan}/duty_cycle", "w") as f:
        f.write(f"{pulse_width}")
    with open(f"{PWM_SYSFS}/pwm{chan}/enable", "w") as f:
        f.write("1")

def kpwm_start_f(chan, frequency, duty_cycle) :
    """
    Enable the PWM on chan, setting frequency and duty_cycle
    chan: 0 or 1
    frequency: The frequency of the PWM signal in Hz
        (used to calculate period to nearest 10nS)
    duty_cycle: 0.0 - 1.0
    """
    period = int(1000000000/frequency)
    pulse_width = int(period * duty_cycle)
    return kpwm_start(chan, period, pulse_width)


