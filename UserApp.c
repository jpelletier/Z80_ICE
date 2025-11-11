/*---------------------------------------------------------------------------*
 * UserApp.c                                                                 *
 * Copyright (C) 2022  Jacques Pelletier                                     *
 *                                                                           *
 * This program is free software; you can redistribute it and *or            *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software Foundation,   *
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *---------------------------------------------------------------------------*/

#include "z80_ice.h"

Z80Context z80Ice;
bool Interrupt_Pending = false;
bool NMI_Pending = false;

#ifdef USE_CODE_TEST_BUFFER
byte Z80Mem[65536];
#endif //USE_CODE_TEST_BUFFER

void setup(void) {
#ifdef USE_CODE_RING_BUFFER
	Ringbuf_init();
#endif //USE_CODE_RING_BUFFER
	/*
	How to Use printf on STM32
	https://shawnhymel.com/1873/how-to-use-printf-on-stm32/
	*/

	// Init Z80 ICE
	z80Ice.memFetch = z80Fetch;
	z80Ice.memRead = z80memRd;
	z80Ice.memWrite = z80memWr;
	z80Ice.ioRead = PCXTioRd;
	z80Ice.ioWrite = PCXTioWr;
}

void loop(void) {
	mainMenu();
}

