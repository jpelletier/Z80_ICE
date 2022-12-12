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

void serial_gets(char *buffer)
{
int result;

	do
	{
		result = serial_getchar();

		switch(result)
		{
		//case '\r':
		case '\n':
			*buffer = 0;
			return;
		default:
			*buffer++ = result & 0xff;
		}
	} while (true);
}

char serial_getchar(void)
{
int result;

	do
	{
		result = Uart_read();
	} while (result == -1);

	return result;
}

char buffer[80];

int serial_printf(char *format, ...)
{
   va_list aptr;
   int ret;

   va_start(aptr, format);
   ret = vsprintf(buffer, format, aptr);
   va_end(aptr);

#ifdef USE_CODE_RING_BUFFER
   Uart_sendstring(buffer);
#endif
   return(ret);
}

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
	Uart_sendstring("\r\nPress any key to continue");

	serial_getchar();
	Uart_sendstring("\033[1K\r");
}

void AnsiClearDisplay(void)
{
	Uart_sendstring("\033[2J");
}

void AnsiSaveCursorPosition(void)
{
	Uart_sendstring("\033[s");
}

void AnsiRestoreCursorPosition(void)
{
	Uart_sendstring("\033[u");
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
		serial_printf("%04X",temp);

		c = toupper(serial_getchar());

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
		Uart_sendstring("\b\b\b\b");
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
		serial_printf("%02X",temp);

		c = toupper(serial_getchar());

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
		Uart_sendstring("\b\b");
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

	c = serial_getchar();
	if (Ascii2Hex(&c))
	{
		//put the first nibble (MSB) into bits 7-4
		b = c << 4;

		c = serial_getchar();
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
 * NMI INT     S Z - H - P N C  IM: 00  I: 00  R: 00  PC : 0000  SP: 0000
 * A : 00  F : 0 0 0 0 0 0 0 0  BC : 0000  DE : 0000  HL : 0000  IX: 0000
 * A': 00  F': 0 0 0 0 0 0 0 0  BC': 0000  DE': 0000  HL': 0000  IY: 0000
 */

void PrintRegs(void)
{
	if (NMI_Pending) serial_printf("\033[30;43mNMI\033[m "); else serial_printf("NMI ");
	if (Interrupt_Pending) serial_printf("\033[30;43mINT\033[m"); else serial_printf("INT");

	serial_printf("     S Z - H - P N C  IM: %02X  I: %02X  R: %02X  PC : %04X  SP: %04X\r\n",
		z80Ice.IM, z80Ice.I, z80Ice.R, z80Ice.PC, z80Ice.R1.wr.SP);

	serial_printf("A : %02X  F : %d %d %d %d %d %d %d %d  BC : %04X  DE : %04X  HL : %04X  IX: %04X\r\n",
		z80Ice.R1.br.A,
		(z80Ice.R1.br.F & 0x80)>>7,
		(z80Ice.R1.br.F & 0x40)>>6,
		(z80Ice.R1.br.F & 0x20)>>5,
		(z80Ice.R1.br.F & 0x10)>>4,
		(z80Ice.R1.br.F & 0x08)>>3,
		(z80Ice.R1.br.F & 0x04)>>2,
		(z80Ice.R1.br.F & 0x02)>>1,
		(z80Ice.R1.br.F & 0x01),
		z80Ice.R1.wr.BC, z80Ice.R1.wr.DE, z80Ice.R1.wr.HL, z80Ice.R1.wr.IX);

	serial_printf("A': %02X  F': %d %d %d %d %d %d %d %d  BC': %04X  DE': %04X  HL': %04X  IY: %04X\r\n\n",
		z80Ice.R2.br.A,
		(z80Ice.R2.br.F & 0x80)>>7,
		(z80Ice.R2.br.F & 0x40)>>6,
		(z80Ice.R2.br.F & 0x20)>>5,
		(z80Ice.R2.br.F & 0x10)>>4,
		(z80Ice.R2.br.F & 0x08)>>3,
		(z80Ice.R2.br.F & 0x04)>>2,
		(z80Ice.R2.br.F & 0x02)>>1,
		(z80Ice.R2.br.F & 0x01),
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
 * dump and decode are the strings returned by the libZ80 library
 * dump contains bytes in ASCII form clumped together
 * */
void PrintDebugBuffers(char *dump, char *decode)
{
	unsigned int dump_len, codes;
	//unsigned int dec_len, value;

	dump_len = strlen(dump)/2;	//nb of bytes
	//dec_len = strlen(decode);

	//serial_printf("Dump: %s, len: %d, Decode: %s, len: %d\r\n", dump, dump_len, decode, dec_len);
	//serial_printf("Scan dump: %d, codes: %0X\r\n", sscanf(dump,"%x",&codes), codes);
	//serial_printf("Scan decode: %d, value: %04X\r\n", sscanf(decode,"%x",&value), value);
	sscanf(dump,"%x",&codes);

	serial_printf("PC: %04X ", z80Ice.PC);

	// print code bytes separately
	do
	{
		serial_printf("%02X ", (codes >> ((dump_len-1) * 8)) & 0xff);
	} while (--dump_len);

	//TODO replace values with symbols
	serial_printf("\b\t%s\r\n", decode);
}
