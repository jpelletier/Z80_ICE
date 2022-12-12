/*---------------------------------------------------------------------------*
 * regedit.c                                                                 *
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
/*
NMI INT     S Z - H - P N C  IM: 00  I: 00  R: 00  PC : 0000  SP: 0000
A : 00  F : 0 0 0 0 0 0 0 0  BC : 0000  DE : 0000  HL : 0000  IX: 0000
A': 00  F': 0 0 0 0 0 0 0 0  BC': 0000  DE': 0000  HL': 0000  IY: 0000
*/
#include "z80_ice.h"

void flag_ed(void)
{
bool quit = false;

	do
	{
		serial_printf("\033[10;13H\033[44m%d %d %d %d %d %d %d %d",
			(z80Ice.R1.br.F & 0x80)>>7,
			(z80Ice.R1.br.F & 0x40)>>6,
			(z80Ice.R1.br.F & 0x20)>>5,
			(z80Ice.R1.br.F & 0x10)>>4,
			(z80Ice.R1.br.F & 0x08)>>3,
			(z80Ice.R1.br.F & 0x04)>>2,
			(z80Ice.R1.br.F & 0x02)>>1,
			(z80Ice.R1.br.F & 0x01));

		switch(toupper(serial_getchar()))
		{
			case 'S':
				z80Ice.R1.br.F ^= 0x80;
				break;
			case 'Z':
				z80Ice.R1.br.F ^= 0x40;
				break;
			case 'H':
				z80Ice.R1.br.F ^= 0x10;
				break;
			case 'P':
				z80Ice.R1.br.F ^= 0x04;
				break;
			case 'N':
				z80Ice.R1.br.F ^= 0x02;
				break;
			case 'C':
				z80Ice.R1.br.F ^= 0x01;
				break;
			case 'Q':
				quit = true;
				break;
			default:
				;
		}
	} while(!quit);

	serial_printf("\033[10;13H\033[m%d %d %d %d %d %d %d %d",
		(z80Ice.R1.br.F & 0x80)>>7,
		(z80Ice.R1.br.F & 0x40)>>6,
		(z80Ice.R1.br.F & 0x20)>>5,
		(z80Ice.R1.br.F & 0x10)>>4,
		(z80Ice.R1.br.F & 0x08)>>3,
		(z80Ice.R1.br.F & 0x04)>>2,
		(z80Ice.R1.br.F & 0x02)>>1,
		(z80Ice.R1.br.F & 0x01));
}

void alt_flag_ed(void)
{
	bool quit = false;

	do
	{
		serial_printf("\033[11;13H\033[44m%d %d %d %d %d %d %d %d",
			(z80Ice.R2.br.F & 0x80)>>7,
			(z80Ice.R2.br.F & 0x40)>>6,
			(z80Ice.R2.br.F & 0x20)>>5,
			(z80Ice.R2.br.F & 0x10)>>4,
			(z80Ice.R2.br.F & 0x08)>>3,
			(z80Ice.R2.br.F & 0x04)>>2,
			(z80Ice.R2.br.F & 0x02)>>1,
			(z80Ice.R2.br.F & 0x01));

		switch(toupper(serial_getchar()))
		{
			case 'S':
				z80Ice.R2.br.F ^= 0x80;
				break;
			case 'Z':
				z80Ice.R2.br.F ^= 0x40;
				break;
			case 'H':
				z80Ice.R2.br.F ^= 0x10;
				break;
			case 'P':
				z80Ice.R2.br.F ^= 0x04;
				break;
			case 'N':
				z80Ice.R2.br.F ^= 0x02;
				break;
			case 'C':
				z80Ice.R2.br.F ^= 0x01;
				break;
			case 'Q':
				quit = true;
				break;
			default:
				;
		}
	} while(!quit);

	serial_printf("\033[11;13H\033[m%d %d %d %d %d %d %d %d",
		(z80Ice.R2.br.F & 0x80)>>7,
		(z80Ice.R2.br.F & 0x40)>>6,
		(z80Ice.R2.br.F & 0x20)>>5,
		(z80Ice.R2.br.F & 0x10)>>4,
		(z80Ice.R2.br.F & 0x08)>>3,
		(z80Ice.R2.br.F & 0x04)>>2,
		(z80Ice.R2.br.F & 0x02)>>1,
		(z80Ice.R2.br.F & 0x01));
}

void reg_ed(void)
{
	ushort reg_pair;
	byte a,b;

	Uart_sendstring("\033[15;1HRegisters editor\r\n\n");

	Uart_sendstring("\033[37;42mI\033[m: I,  \033[37;42mP\033[m: PC,     \033[37;42mS\033[m: SP,  \033[37;42mQ\033[m: quit\r\n");
	Uart_sendstring("\033[37;42ma\033[m: A,  \033[37;42mf\033[m: flags,  \033[37;42mb\033[m: BC,  \033[37;42md\033[m: DE,  \033[37;42mh\033[m: HL,  \033[37;42mX\033[m: IX\r\n");
	Uart_sendstring("\033[37;42mA\033[m: A', \033[37;42mF\033[m: flags', \033[37;42mB\033[m: BC', \033[37;42mD\033[m: DE', \033[37;42mH\033[m: HL', \033[37;42mY\033[m: IY\r\n");

	do
	{
		switch(serial_getchar())
		{
		case 'a':
			Uart_sendstring("\033[10;5H\033[44m");
			b = z80Ice.R1.br.A;
			a = in_b_k(&b);
			if (a == 0x0d) z80Ice.R1.br.A = b;
			serial_printf("\b\b\033[m%02X",b);
			break;

		case 'A':
			Uart_sendstring("\033[11;5H\033[44m");
			b = z80Ice.R2.br.A;
			a = in_b_k(&b);
			if (a == 0x0d) z80Ice.R2.br.A = b;
			serial_printf("\b\b\033[m%02X",b);
			break;

		case 'b':
			Uart_sendstring("\033[10;35H\033[44m");
			reg_pair = z80Ice.R1.wr.BC;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R1.wr.BC = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'B':
			Uart_sendstring("\033[11;35H\033[44m");
			reg_pair = z80Ice.R2.wr.BC;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R2.wr.BC = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'd':
			Uart_sendstring("\033[10;46H\033[44m");
			reg_pair = z80Ice.R1.wr.DE;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R1.wr.DE = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'D':
			Uart_sendstring("\033[11;46H\033[44m");
			reg_pair = z80Ice.R2.wr.DE;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R2.wr.DE = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'f':
			flag_ed();
			break;
		case 'F':
			alt_flag_ed();
			break;

		case 'h':
			Uart_sendstring("\033[10;57H\033[44m");
			reg_pair = z80Ice.R1.wr.HL;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R1.wr.HL = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'H':
			Uart_sendstring("\033[11;57H\033[44m");
			reg_pair = z80Ice.R2.wr.HL;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R2.wr.HL = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'I':
			Uart_sendstring("\033[9;41H\033[44m");
			b = z80Ice.I;
			a = in_b_k(&b);
			if (a == 0x0d) z80Ice.I = b;
			serial_printf("\b\b\033[m%02X",b);
			break;

		case 'X':
			Uart_sendstring("\033[10;67H\033[44m");
			reg_pair = z80Ice.R1.wr.IX;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R1.wr.IX = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'Y':
			Uart_sendstring("\033[11;67H\033[44m");
			reg_pair = z80Ice.R1.wr.IY;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R1.wr.IY = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'S':
			Uart_sendstring("\033[9;67H\033[44m");
			reg_pair = z80Ice.R1.wr.SP;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.R1.wr.SP = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'P':
			Uart_sendstring("\033[9;57H\033[44m");
			reg_pair = z80Ice.PC;
			a = setaddress(&reg_pair);
			if (a == 0x0d) z80Ice.PC = reg_pair;
			serial_printf("\b\b\b\b\033[m%04X",reg_pair);
			break;

		case 'Q':
			return;
		}
	} while (true);
}
