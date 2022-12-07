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
#include <ctype.h>
#include <string.h>
#include <math.h>

//libz80
#include "z80.h"

#include "config.h"

#include "buscycles.h"
#include "z80_bits.h"
#include "main.h"
#include "retarget.h"
#include "UserApp.h"
#include "binedit.h"
#include "memops.h"
#include "menus.h"
#include "utils.h"

//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

extern Z80Context z80Ice;

#ifdef USE_CODE_TEST_BUFFER
extern byte Z80Mem[65536];
#endif //USE_CODE_TEST_BUFFER

#endif /* Z80_ICE_H_ */
