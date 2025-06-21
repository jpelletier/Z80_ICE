/*---------------------------------------------------------------------------*
 * menus.c                                                               *
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

void mainMenu(void)
{
	char c;

	//Turns on software flow control
	Uart_sendstring("\021\033[2JZ80 ICE Main menu v2\r\n\n");
	Uart_sendstring(
			"A: Manual mode\r\n"
			"B: Bus transactions\r\n"
			"C: Bus repeated transactions\r\n"
			"D: Memory operations\r\n"
			"E: Z80 debugger\r\n"
			);
	c = toupper(serial_getchar());
	switch(c)
	{
		case 'A':
			ManualModeMenu();
			break;
		case 'B':
			BusTransactionsMenu();
			break;
		case 'C':
			BusRepeatTransactionsMenu();
			break;
		case 'D':
			MemoryOperationsMenu();
			break;
		case 'E':
			Z80DebuggerMenu();
			break;
		default:
			;
	}
}

void ManualModeMenu(void)
{
	char c;

	//INACTIVE
	GPIOC->ODR = 0xFF00;

	AnsiSaveCursorPosition();

	do
	{
		Uart_sendstring("\033[2JManual mode\r\n\n");
		Uart_sendstring("M1 MREQ IORQ RD WR RFSH BUSACK HALT\tD7 D6 D5 D4 D3 D2 D1 D0\r\n");
		serial_printf(" %1d  %1d    %1d    %1d  %1d  %1d    %1d      %1d\t %1d  %1d  %1d  %1d  %1d  %1d  %1d  %1d\r\n\n",
				getBit(BIT_M1,GPIOC->ODR),
				getBit(BIT_MREQ,GPIOC->ODR),
				getBit(BIT_IORQ,GPIOC->ODR),
				getBit(BIT_RD,GPIOC->ODR),
				getBit(BIT_WR,GPIOC->ODR),
				getBit(BIT_RFSH,GPIOC->ODR),
				getBit(BIT_BUSAK,GPIOF->ODR),
				getBit(BIT_HALT,GPIOF->ODR),

				getBit(7,GPIOC->ODR),
				getBit(6,GPIOC->ODR),
				getBit(5,GPIOC->ODR),
				getBit(4,GPIOC->ODR),
				getBit(3,GPIOC->ODR),
				getBit(2,GPIOC->ODR),
				getBit(1,GPIOC->ODR),
				getBit(0,GPIOC->ODR));

		Uart_sendstring("RST INT NMI WAIT BUSRQ\t\tD7 D6 D5 D4 D3 D2 D1 D0\r\n");
		serial_printf(" %1d   %1d   %1d   %1d    %1d\t\t%1d  %1d  %1d  %1d  %1d  %1d  %1d  %1d\r\n\n",
				getBit(BIT_RST,GPIOF->IDR),
				getBit(BIT_INT,GPIOF->IDR),
				getBit(BIT_NMI,GPIOF->IDR),
				getBit(BIT_WAIT,GPIOF->IDR),
				getBit(BIT_BUSRQ,GPIOF->IDR),

				getBit(7,GPIOC->IDR),
				getBit(6,GPIOC->IDR),
				getBit(5,GPIOC->IDR),
				getBit(4,GPIOC->IDR),
				getBit(3,GPIOC->IDR),
				getBit(2,GPIOC->IDR),
				getBit(1,GPIOC->IDR),
				getBit(0,GPIOC->IDR));

		Uart_sendstring("A15 A14 A13 A12 A11 A10 A9 A8 A7 A6 A5 A4 A3 A2 A1 A0\r\n");
		serial_printf("  %1d   %1d   %1d   %1d   %1d   %1d  %1d  %1d  %1d  %1d  %1d  %1d  %1d  %1d  %1d  %1d\r\n\n",
				getBit(15,GPIOD->ODR),
				getBit(14,GPIOD->ODR),
				getBit(13,GPIOD->ODR),
				getBit(12,GPIOD->ODR),
				getBit(11,GPIOD->ODR),
				getBit(10,GPIOD->ODR),
				getBit( 9,GPIOD->ODR),
				getBit( 8,GPIOD->ODR),
				getBit( 7,GPIOD->ODR),
				getBit( 6,GPIOD->ODR),
				getBit( 5,GPIOD->ODR),
				getBit( 4,GPIOD->ODR),
				getBit( 3,GPIOD->ODR),
				getBit( 2,GPIOD->ODR),
				getBit( 1,GPIOD->ODR),
				getBit( 0,GPIOD->ODR));

		Uart_sendstring(
				"Toggle bits:\r\n"
				"A: M1       I: A15   Q: A7   7: D7\r\n"
				"B: MREQ     J: A14   R: A6   6: D6\r\n"
				"C: IORQ     K: A13   S: A5   5: D5\r\n"
				"D: RD       L: A12   T: A4   4: D4\r\n"
				"E: WR       M: A11   U: A3   3: D3\r\n"
				"F: RFSH     N: A10   V: A2   2: D2\r\n"
				"G: BUSACK   O: A9    W: A1   1: D1\r\n"
				"H: HALT     P: A8    X: A0   0: D0\r\n\n"
				"Z: return to previous menu\r\n"
				);

			c = toupper(serial_getchar());
			switch(c)
			{
			case '7':
				GPIOC->ODR ^= 0x80;
				break;
			case '6':
				GPIOC->ODR ^= 0x40;
				break;
			case '5':
				GPIOC->ODR ^= 0x20;
				break;
			case '4':
				GPIOC->ODR ^= 0x10;
				break;
			case '3':
				GPIOC->ODR ^= 0x08;
				break;
			case '2':
				GPIOC->ODR ^= 0x04;
				break;
			case '1':
				GPIOC->ODR ^= 0x02;
				break;
			case '0':
				GPIOC->ODR ^= 0x01;
				break;
			case 'A':
				GPIOC->ODR ^= MASK_M1;
				break;
			case 'B':
				GPIOC->ODR ^= MASK_MREQ;
				break;
			case 'C':
				GPIOC->ODR ^= MASK_IORQ;
				break;
			case 'D':
				//set data direction in
				GPIOC->MODER = 0x55550000;
				GPIOC->ODR ^= MASK_RD;
				break;
			case 'E':
				//set data direction out
				GPIOC->MODER = 0x55555555;
				GPIOC->ODR ^= MASK_WR;
				break;
			case 'F':
				GPIOC->ODR ^= MASK_RFSH;
				break;
			case 'G':
				GPIOF->ODR ^= MASK_BUSAK;
				break;
			case 'H':
				GPIOF->ODR ^= MASK_HALT;
				break;

			case 'I':
				GPIOD->ODR ^= 0x8000;
				break;
			case 'J':
				GPIOD->ODR ^= 0x4000;
				break;
			case 'K':
				GPIOD->ODR ^= 0x2000;
				break;
			case 'L':
				GPIOD->ODR ^= 0x1000;
				break;
			case 'M':
				GPIOD->ODR ^= 0x0800;
				break;
			case 'N':
				GPIOD->ODR ^= 0x0400;
				break;
			case 'O':
				GPIOD->ODR ^= 0x0200;
				break;
			case 'P':
				GPIOD->ODR ^= 0x0100;
				break;
			case 'Q':
				GPIOD->ODR ^= 0x0080;
				break;
			case 'R':
				GPIOD->ODR ^= 0x0040;
				break;
			case 'S':
				GPIOD->ODR ^= 0x0020;
				break;
			case 'T':
				GPIOD->ODR ^= 0x0010;
				break;
			case 'U':
				GPIOD->ODR ^= 0x0008;
				break;
			case 'V':
				GPIOD->ODR ^= 0x0004;
				break;
			case 'W':
				GPIOD->ODR ^= 0x0002;
				break;
			case 'X':
				GPIOD->ODR ^= 0x0001;
				break;
			case 'Z':
				return;
			default:
				;
		}

		AnsiRestoreCursorPosition();

	} while (true);
}

void BusTransactionsMenu(void)
{
	char c;
	ushort address;
	byte data_byte;

	AnsiSaveCursorPosition();

	do
	{
		Uart_sendstring("\033[2JBus transactions\r\n\n");
		Uart_sendstring(
				"A: set address\r\n"
				"B: set data\r\n"
				"C: do 1 fetch cycle\r\n"
				"D: do 1 memory read cycle\r\n"
				"E: do 1 memory write cycle\r\n"
				"F: do 1 IO port read cycle\r\n"
				"G: do 1 IO port write cycle\r\n"
				"H: do 1 INTA cycle\r\n"
				"Q: quit\r\n\n"
				);
		c = toupper(serial_getchar());
		switch(c)
		{
			case 'A':
				setaddress(&address);
				break;
			case 'B':
				setdatabyte(&data_byte);
				break;
			case 'C':
				serial_printf("Fetch at %04X: %02X\r\n", address, z80Fetch(0,address));
				break;
			case 'D':
				serial_printf("Mem read at %04X: %02X\r\n", address, z80memRd(0,address));
				break;
			case 'E':
				serial_printf("Mem write at %04X: %02X\r\n", address, data_byte);
				z80memWr(0,address, data_byte);
				break;
			case 'F':
				serial_printf("IO read at %04X: %02X\r\n", address, z80ioRd(0,address));
				break;
			case 'G':
				serial_printf("IO write at %04X: %02X\r\n", address, data_byte);
				z80ioWr(0,address, data_byte);
				break;
			case 'H':
				serial_printf("Int ack: %02X\r\n", z80iack(0));
				break;
			case 'Q':
				return;
		}

		AnsiRestoreCursorPosition();

	} while (true);
}

/* hardware diag with scope */
void BusRepeatTransactionsMenu(void)
{
	char c;
	ushort address;
	byte data_byte;

	AnsiSaveCursorPosition();

	do
	{
		Uart_sendstring("\033[2JBus transaction loops\r\n\n");
		Uart_sendstring(
				"A: set address\r\n"
				"B: set data\r\n"
				"C: fetch cycles\r\n"
				"D: memory read cycles\r\n"
				"E: memory write cycles\r\n"
				"F: IO port read cycles\r\n"
				"G: IO port write cycles\r\n"
				"H: INTA cycles\r\n\n"
				"Q: quit\r\n\n"
				);
		c = toupper(serial_getchar());
		switch(c)
		{
			case 'A':
				setaddress(&address);
				break;
			case 'B':
				setdatabyte(&data_byte);
				break;
			case 'C':
				AnsiSaveCursorPosition();
				do
				{
					serial_printf("Fetch at %04X: %02X\r\n", address, z80Fetch(0,address));
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				break;
			case 'D':
				AnsiSaveCursorPosition();
				do
				{
					serial_printf("Memory read at %04X: %02X\r\n", address, z80memRd(0,address));
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				break;
			case 'E':
				AnsiSaveCursorPosition();
				do
				{
					serial_printf("Memory write at %04X: %02X\r\n", address, data_byte);
					z80memWr(0,address, data_byte);
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				break;
			case 'F':
				AnsiSaveCursorPosition();
				do
				{
					serial_printf("IO read at %04X: %02X\r\n", address, z80ioRd(0,address));
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				break;
			case 'G':
				AnsiSaveCursorPosition();
				do
				{
					serial_printf("IO write at %04X: %02X\r\n", address, data_byte);
					z80ioWr(0,address, data_byte);
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				break;
			case 'H':
				AnsiSaveCursorPosition();
				do
				{
					serial_printf("Int ack: %02X\r\n", z80iack(0));
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				break;
			case 'Q':
				return;
			default:
				;
		}

		AnsiRestoreCursorPosition();

	} while (true);
}

void MemoryOperationsMenu(void)
{
	char c;
	byte data_byte;
	byte port;

	AnsiSaveCursorPosition();

	do
	{
		Uart_sendstring("\033[2JMemory operations\r\n\n");
		Uart_sendstring(
				"E: edit memory\r\n"
				"L: load Intel hex file\r\n"
				"F: fill memory\r\n"
				"M: move memory\r\n"
				"I: input\r\n"
				"O: output\r\n"
				"Q: quit\r\n\n"
				);
		c = toupper(serial_getchar());
		switch(c)
		{
			case 'E':
				binary_ed(0);
				break;
			case 'L':
				loadIntelHex();
				GetAnyKey();
				break;
			case 'F':
				fillMemory();
				break;
			case 'M':
				moveMemory();
				break;
			case 'I':
				Uart_sendstring("Input port: ");
				c = setdatabyte(&port);
				if (c != 'Q')
				{
					data_byte = z80ioRd(0, port);
					serial_printf(" %02X\r\n",data_byte);
					GetAnyKey();
				}
				break;
			case 'O':
				Uart_sendstring("Output port: ");
				c = setdatabyte(&port);
				if (c != 'Q')
				{
					Uart_write(' ');
					//putchar(' ');
					c = setdatabyte(&data_byte);
					if (c != 'Q')
					{
						z80ioWr(0,port, data_byte);
						Uart_sendstring("\r\n");
					}
				}
				break;
			case 'Q':
				return;
		}

		AnsiRestoreCursorPosition();

	} while (true);
}

/*
NMI INT RUN S Z - H - P N C  IM: 02  I: FF  R: 3B  PC : C35A  SP: FDF6
A : 02  F : 0 0 0 1 0 0 0 0  BC : 0468  DE : FFAC  HL : 000B  IX: 0000
A': 00  F': 0 0 0 0 0 0 0 0  BC': 0000  DE': 0000  HL': 0000  IY: 0000
*/
void Z80DebuggerMenu(void)
{
	char c;
	ushort address;

	char dump_buffer[80];
	char decode_buffer[80];

	AnsiSaveCursorPosition();

	do
	{
		Uart_sendstring("\033[2JZ80 debugger\r\n\n");
		Uart_sendstring(
				"A: reset\tC: run until return\tI: step in Int Pending\r\n"
				"B: step \tD: run until address\tN: step in NMI Pending\r\n"
				"E: edit regs\tF: run until int\r\n"
				"R: run\r\n"
				"Q: quit\r\n\n"
				);

		PrintRegs();

		/* Decode the next instruction to be executed. */
		Z80Debug(&z80Ice, dump_buffer, decode_buffer);

		PrintDebugBuffers(dump_buffer, decode_buffer);

		c = toupper(serial_getchar());

		switch(c)
		{
			case 'A':
				Z80RESET(&z80Ice);
				break;

			case ' ':
			case 'B':
				Z80Execute(&z80Ice);
				break;

			case 'C':
				if (is_return_inst(dump_buffer)) break;

				// Run until return
				serial_printf("\033[9;9H\033[37;42mRUN\033[m");

				do
				{
					Z80Execute(&z80Ice);
					if (z80Ice.halted)
					{
						GPIOF->ODR &= !MASK_HALT; /* FIXME doesn't work */
						serial_printf("\033[9;9H\033[37;41mHLT\033[m");
					}

					/* Decode the next instruction to be executed. */
					Z80Debug(&z80Ice, dump_buffer, decode_buffer);
				} while (!is_return_inst(dump_buffer));
				break;

			case 'D':
				// Run until address
				serial_printf("\033[9;9H\033[37;42mRUN\033[m");

				//Get address
				Uart_sendstring("\033[7;17HAddress: \033[44m");
				address = 0;
				c = setaddress(&address);
				serial_printf("\033[7;17H\033[m\033[0K");

				if (c != 0x0d) break;

				while (z80Ice.PC != address)
				{
					Z80Execute(&z80Ice);
					if (z80Ice.halted)
					{
						serial_printf("\033[9;9H\033[37;41mHLT\033[m");
						break;
					}
				}
				break;

			case 'E':
				//Edit registers
				reg_ed();
				break;

			case 'F':
				// Run until interrupt
				serial_printf("\033[9;9H\033[37;42mRUN\033[m");

				do
				{
					Z80Execute(&z80Ice);
					if (z80Ice.halted) serial_printf("\033[9;9H\033[37;41mHLT\033[m");
				} while (!Interrupt_Pending);
				break;

			case 'I':
				Interrupt_Pending = false;
				Z80INT(&z80Ice, z80iack(0));
				Z80Execute(&z80Ice);
				break;
			case 'N':
				NMI_Pending = false;
				Z80NMI(&z80Ice);
				Z80Execute(&z80Ice);
				break;

			case 'R':
				// Run until keypressed
				serial_printf("\033[9;9H\033[37;42mRUN\033[m");

				do
				{
					if (NMI_Pending)
					{
						NMI_Pending = false;
						Z80NMI(&z80Ice);
					}

					if (Interrupt_Pending)
					{
						Interrupt_Pending = false;
						Z80INT(&z80Ice, z80iack(0));
					}

					Z80Execute(&z80Ice);
					if (z80Ice.halted)
					{
						serial_printf("\033[9;9H\033[37;41mHLT\033[m");
						break;
					}
				} while (!IsDataAvailable());
				break;

			case 'Q':
				return;
		}

		AnsiRestoreCursorPosition();

	} while (true);
}

