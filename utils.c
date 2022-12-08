/*---------------------------------------------------------------------------*
 * utils.c                                                                   *
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

/* Mcurses
void App_putchar(uint8_t c)
{
    putchar(c);
}

char App_getchar(void)
{
    return getchar();
}
*/

// https://siongui.github.io/2013/01/09/c-remove-string-trailing-newline-carriage-return/
void removeStringTrailingNewline(char *str)
{
  if (str == NULL)
    return;
  int length = strlen(str);
  if (str[length-1] == '\n' || str[length-1] == '\r')
    str[length-1]  = '\0';
  if (str[length-2] == '\n' || str[length-2] == '\r')
    str[length-2]  = '\0';
}

void GetAnyKey(void)
{
	printf("\r\nPress any key to continue");

	getchar();
	printf("\033[1K\r");
}

void AnsiClearDisplay(void)
{
	printf("\033[2J");
}

void AnsiSaveCursorPosition(void)
{
	printf("\033[s");
}

void AnsiRestoreCursorPosition(void)
{
	printf("\033[u");
}

int getBit(int bit, int value)
{
	return (value >> bit) & 1;
}

byte revert_bits(byte in)
{
	byte temp = 0;

	temp |= (in & 0x80) >> 7;
	temp |= (in & 0x40) >> 5;
	temp |= (in & 0x20) >> 3;
	temp |= (in & 0x10) >> 1;
	temp |= (in & 0x08) << 1;
	temp |= (in & 0x04) << 3;
	temp |= (in & 0x02) << 5;
	temp |= (in & 0x01) << 7;

	return temp;
}

ushort revert_word(ushort in)
{
	byte h,l;

	h = revert_bits(in & 0xff);
	l = revert_bits(in >> 8);

	return (h << 8) | l;
}

char setaddress(ushort *address)
{
	char c;
	ushort temp;

	temp = *address;

	do
	{
		printf("%04X",temp);

		c = toupper(getchar());

		switch(c)
		{
			case '0' ... '9':
				temp = (temp << 4) | (c & 0x0f);
				break;
			case 'A' ... 'F':
				temp = (temp << 4) | (c - 'A' + 0x0A);
				break;
			case '+':
				temp++;
				break;
			case '-':
				temp--;
				break;
			case 'Z':
				temp = 0;
				break;
			case 'R':
				temp = revert_word(temp);
				break;
			case 0x08:
				temp >>= 4;
				break;
			case '~': //neg = !(--HL) = (!HL)++
				temp--;
				//fallthrough
			case '!':
				temp ^= 0xFFFF;
				break;
			case 'Q':
				return c;
			case 0x0d:
				*address = temp;
				return c;
			default:
				;
		}
		printf("\b\b\b\b");
	} while (true);

	return 0;
}

/* Edit byte value with some handy transformations
 * z:	reset to 00
 * +:	increment
 * -:	decrement
 * !:	invert
 * ~:	negate (value = -value)
 * R:	revert bits (0..7 -> 7..0)
 * BSP:	remove nibble (3F -> 03)
 */

char in_b_k(byte *data)
{
	char c;
	byte temp;

	temp = *data;

	do
	{
		printf("%02X",temp);

		c = toupper(getchar());

		switch(c)
		{
			case '0' ... '9':
				temp = (temp << 4) | (c & 0x0f);
				break;
			case 'A' ... 'F':
				temp = (temp << 4) | (c - 'A' + 0x0A);
				break;
			case '+':
				temp++;
				break;
			case '-':
				temp--;
				break;
			case 'Z':
				temp = 0;
				break;
			case 0x08:
				temp >>= 4;
				break;
			case 'R': // revert bits 0..7 -> 7..0
				temp = revert_bits(temp);
				break;
			case '~': //neg = !(--B) = (!B)++
				temp--;
				//fallthrough
			case '!':
				temp ^= 0xFF;
				break;
			default:
				*data = temp;
				return c;
		}
		printf("\b\b");
	} while (true);

	return 0;
}

char setdatabyte(byte *data)
{
	char c;

	do
	{
		c = in_b_k(data);
	} while (!((c == 'Q')||(c == 0x0d)));

	return c;
}

bool Ascii2Hex(char *c)
{
	bool result = false;

	//'0' to '9' or 'A' to 'F'
	switch(toupper(*c))
	{
		case '0' ... '9':
			*c = *c - '0';
			result = true;
			break;

		case 'A' ... 'F':
        	*c = *c - 'A' + 0x0a;
			result = true;
			break;
		default:
		;
	}

	return result;
}

bool SerialGetAsciiByte(byte *data)
{
char c, b;
bool result = false;

	c = getchar();
	if (Ascii2Hex(&c))
	{
		//put the first nibble (MSB) into bits 7-4
		b = c << 4;

		c = getchar();
		if (Ascii2Hex(&c))
		{
			//combine first nibble (MSB) with second nibble (LSB)
			b |= c;
			*data = b;
			result = true;
		}
	}
	return result;
}

bool SerialGetAsciiWord(ushort *addr)
{
	byte low, high;
	bool result = false;

	if (SerialGetAsciiByte(&high))
	{
		if (SerialGetAsciiByte(&low))
		{
			*addr = (high << 8) | low;
			result = true;
		}
	}
	return result;
}

/*
 *             S Z - H - P N C                                   SP: 0000
 * A : 00  F : 0 0 0 0 0 0 0 0  BC : 0000  DE : 0000  HL : 0000  IX: 0000
 * A': 00  F': 0 0 0 0 0 0 0 0  BC': 0000  DE': 0000  HL': 0000  IY: 0000
 *
 *
 *
 */

void PrintRegs(void)
{
/*
	printf("AF: %04X\tAF': %04X\r\n",z80Ice.R1.wr.AF, z80Ice.R2.wr.AF);
	printf("BC: %04X\tBC': %04X\r\n",z80Ice.R1.wr.BC, z80Ice.R2.wr.BC);
	printf("DE: %04X\tDE': %04X\r\n",z80Ice.R1.wr.DE, z80Ice.R2.wr.DE);
	printf("HL: %04X\tHL': %04X\r\n",z80Ice.R1.wr.HL, z80Ice.R2.wr.HL);
	printf("IX: %04X\tIY : %04X\r\n",z80Ice.R1.wr.IX, z80Ice.R2.wr.IY);
	printf("SP: %04X\r\n",z80Ice.R1.wr.SP);
*/
	printf("            S Z - H - P N C                                   SP: %04X\r\n",z80Ice.R1.wr.SP);
	printf("A : %02X  F : %d %d %d %d %d %d %d %d  BC : %04X  DE : %04X  HL : %04X  IX: %04X\r\n",
		z80Ice.R1.wr.AF>>8,
		(z80Ice.R1.wr.AF & 0x80)>>7,
		(z80Ice.R1.wr.AF & 0x40)>>6,
		(z80Ice.R1.wr.AF & 0x20)>>5,
		(z80Ice.R1.wr.AF & 0x10)>>4,
		(z80Ice.R1.wr.AF & 0x08)>>3,
		(z80Ice.R1.wr.AF & 0x04)>>2,
		(z80Ice.R1.wr.AF & 0x02)>>1,
		(z80Ice.R1.wr.AF & 0x01),
		z80Ice.R1.wr.BC, z80Ice.R1.wr.DE, z80Ice.R1.wr.HL, z80Ice.R1.wr.IX);

	printf("A': %02X  F': %d %d %d %d %d %d %d %d  BC': %04X  DE': %04X  HL': %04X  IY: %04X\r\n\n",
		z80Ice.R2.wr.AF>>8,
		(z80Ice.R2.wr.AF & 0x80)>>7,
		(z80Ice.R2.wr.AF & 0x40)>>6,
		(z80Ice.R2.wr.AF & 0x20)>>5,
		(z80Ice.R2.wr.AF & 0x10)>>4,
		(z80Ice.R2.wr.AF & 0x08)>>3,
		(z80Ice.R2.wr.AF & 0x04)>>2,
		(z80Ice.R2.wr.AF & 0x02)>>1,
		(z80Ice.R2.wr.AF & 0x01),
		z80Ice.R2.wr.BC, z80Ice.R2.wr.DE, z80Ice.R2.wr.HL, z80Ice.R2.wr.IY);
}

bool is_return_inst(char *ascii_inst)
{
bool result = false;
unsigned int hex_inst;

	if (sscanf(ascii_inst,"%02X", &hex_inst) == 1)
	{
		if (hex_inst == 0xC9) result = true;
	}

	return result;
}

/*
 * dump contains bytes in ASCII form clumped together
 * */
void PrintDebugBuffers(char *dump, char *decode)
{
	unsigned int dump_len, codes;
	//unsigned int dec_len, value;

	dump_len = strlen(dump)/2;	//nb of bytes
	//dec_len = strlen(decode);

	//printf("Dump: %s, len: %d, Decode: %s, len: %d\r\n", dump, dump_len, decode, dec_len);
	//printf("Scan dump: %d, codes: %0X\r\n", sscanf(dump,"%x",&codes), codes);
	//printf("Scan decode: %d, value: %04X\r\n", sscanf(decode,"%x",&value), value);
	sscanf(dump,"%x",&codes);

	printf("PC: %04X ", z80Ice.PC);

	// print code bytes separately
	do
	{
		printf("%02X ", (codes >> ((dump_len-1) * 8)) & 0xff);
	} while (--dump_len);

	//TODO replace values with symbols
	printf("\b\t%s\r\n", decode);
}
