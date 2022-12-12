/*
 * z80_ice.h
 *
 *  Created on: 3 déc. 2022
 *      Author: jpellet
 */

#ifndef Z80_ICE_H_
#define Z80_ICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

//Conditional defines
#include "config.h"

//libz80
//https://github.com/ggambetta/libz80
#include "z80.h"

//https://www.youtube.com/watch?v=wj427hpP81s
//https://github.com/controllerstech/stm32-uart-ring-buffer
#include "UartRingbuffer.h"

#include "buscycles.h"
#include "z80_bits.h"
#include "main.h"
#include "retarget.h"
#include "UserApp.h"
#include "binedit.h"
#include "memops.h"
#include "menus.h"
#include "utils.h"
#include "regedit.h"

//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

extern bool Interrupt_Pending;
extern bool NMI_Pending;

extern Z80Context z80Ice;

#ifdef USE_CODE_TEST_BUFFER
extern byte Z80Mem[65536];
#endif //USE_CODE_TEST_BUFFER

#endif /* Z80_ICE_H_ */
