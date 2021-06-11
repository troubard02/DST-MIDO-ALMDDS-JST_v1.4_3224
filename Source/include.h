#ifndef __INCLUDE_H
#define __INCLUDE_H


#define F_CPU 8000000


#include <stdio.h>
#include <iom168pa.h>
#include <string.h>
#include <ina90.h>
#include <inavr.h>
#include <math.h>
#include <stdint.h>
#include <ina90.h>
#include <stdlib.h>

#include "ps-33d.h"


#include "main.h"
#include "module_initialize.h"
#include "fnd_display.h"
#include "usart.h"

#include "switch.h"
#include "adc.h"
#include "motor.h"
#include "alarm.h"

#include "system_mode.h"

/*
#include "twi_master_driver.h"


#include "ps-33d.h"
#include "switch.h"
#include "fnd_display.h"
#include "system_mode.h"
#include "adc.h"
#include "motor.h"
#include "alarm.h"
*/


#include "type_conversion_util.h"
#include "avr_gpio_driver.h"
#include "avr_twi_master_driver.h"


#endif

