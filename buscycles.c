/*---------------------------------------------------------------------------*
 * buscycles.c                                                               *
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

//Code is tweaked to fit timings measured on a real Z80 at 4 MHz.

/* This implement the 6 types of bus transactions of the Z80.
 *
 * - Fetch followed by refresh
 * - Memory read/write
 * - IO read/write
 * - Interrupt acknowledge
 *
 * */

void wait_half_cycle(void)
{
	asm("NOP");
	asm("NOP");
}

void wait_1_cycle(void)
{
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
}

#ifdef USE_CODE_TEST_BUFFER
byte z80Fetch(size_t param, ushort address)
{
	return Z80Mem[address];
}

byte z80memRd(size_t param, ushort address)
{
	return Z80Mem[address];
}

void z80memWr(size_t param, ushort address, byte data)
{
	Z80Mem[address] = data;
}

byte z80ioRd(size_t param, ushort address)
{
byte value;

	printf("Reading IO address %02X: ", address);

	setdatabyte(&value);

	return value;
}

void z80ioWr(size_t param, ushort address, byte io_data)
{
	printf("Writing IO address %02X: %02X\r\n", address, io_data);
}

byte z80iack(size_t param)
{
	return 0; //FIXME
}

#else
byte z80Fetch(size_t param, ushort address)
{
	byte op_code;

	//set data direction in
	GPIOC->MODER = 0x55550000;

	//M1 = 0
	GPIOC->ODR = 0b1110111100000000;

	wait_half_cycle();

	//MRQ & RD = 0
	GPIOC->ODR = 0b1110101000000000;
	GPIOD->ODR = address;

	wait_1_cycle();

	//Check wait input
	if ((GPIOF->IDR & GPIO_PIN_4) == 0)
	{
		wait_1_cycle();
	}

	op_code = GPIOC->IDR;

	asm("NOP");

	//INACTIVE
	GPIOC->ODR = 0xFF00;
	asm("NOP");
	asm("NOP");
	asm("NOP");

	//RFSH = 0
	GPIOC->ODR = 0b1101111100000000;

	wait_half_cycle();

	//MRQ = 0
	GPIOC->ODR = 0b1100101100000000;

	wait_1_cycle();

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	asm("NOP");
	asm("NOP");

	return op_code;
}

void z80memWr(size_t param, ushort address, byte data)
{
	GPIOD->ODR = address;
	GPIOC->ODR = data;

	//MRQ = 0
	GPIOC->ODR = data | 0b1111101100000000;

	//set data direction out
	GPIOC->MODER = 0x55555555;

	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	//MRQ,WR = 0
	GPIOC->ODR = data | 0b1111100100000000;

	//Check wait input
	if ((GPIOF->IDR & GPIO_PIN_4) == 0)
	{
		wait_1_cycle();
	}

	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	asm("NOP");
	asm("NOP");

	//set data direction in
	GPIOC->MODER = 0x55550000;
}

byte z80memRd(size_t param, ushort address)
{
	byte data_bus;

	GPIOD->ODR = address;

	//MRQ = 0, RD = 0
	GPIOC->ODR = 0b1111101000000000;

	//set data direction in
	GPIOC->MODER = 0x55550000;

	wait_1_cycle();
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	//Check wait input
	if ((GPIOF->IDR & GPIO_PIN_4) == 0)
	{
		wait_1_cycle();
	}

	data_bus = GPIOC->IDR;

	asm("NOP");
	asm("NOP");

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	return data_bus;
}

void z80ioWr(size_t param, ushort address, byte io_data)
{
	GPIOD->ODR = address;

	//IORQ = 0, WR = 0
	GPIOC->ODR = io_data | 0b1111010100000000;

	//set data direction out
	GPIOC->MODER = 0x55555555;

	wait_1_cycle();

	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	//Check wait input
	if ((GPIOF->IDR & GPIO_PIN_4) == 0)
	{
		wait_1_cycle();
	}

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	//set data direction in
	GPIOC->MODER = 0x55550000;
}

byte z80ioRd(size_t param, ushort address)
{
byte io_data;

	GPIOD->ODR = address;

	//set data direction in
	GPIOC->MODER = 0x55550000;

	//IORQ = 0, RD = 0
	GPIOC->ODR = 0b1111011000000000;

	wait_1_cycle();

	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	//Check wait input
	if ((GPIOF->IDR & GPIO_PIN_4) == 0)
	{
		wait_1_cycle();
	}

	io_data = GPIOC->IDR;

	asm("NOP");
	asm("NOP");

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	return io_data;
}

byte z80iack(size_t param)
{
byte irq_data;

	//set data direction in
	GPIOC->MODER = 0x55550000;

	//M1 = 0
	GPIOC->ODR = 0b1110111100000000;

	wait_1_cycle();

	//M1 = 0, IORQ = 0
	GPIOC->ODR = 0b1110011100000000;

	//Check wait input
	if ((GPIOF->IDR & GPIO_PIN_4) == 0)
	{
		wait_1_cycle();
	}

	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");

	irq_data = GPIOC->IDR;

	asm("NOP");
	asm("NOP");

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	return irq_data;
}
#endif

