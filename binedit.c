/*---------------------------------------------------------------------------*
 * binedit.c                                                                 *
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

void move_cursor(ushort hl)
{
byte row = ((hl & 0xff) >> 4) + 3;
byte column = 3 * (hl & 0x0f) + 8; //6;

	printf("\033[%d;%dH",row,column);
}

void restore_color(ushort address)
{
#ifdef USE_CODE_TEST_BUFFER
	printf("\b\b\033[m%02X",Z80Mem[address]);
#else
	printf("\b\b\033[m%02X",z80memRd(0,address));
#endif
}

ushort move_left(ushort hl)
{
	restore_color(hl);

	//roll decrement
	hl = ((hl - 1) & 0x0f) | (hl & 0xfff0);
    move_cursor(hl);

    return hl;
}

ushort move_right(ushort hl)
{
	restore_color(hl);

	//roll increment
	hl = ((hl + 1) & 0x0f) | (hl & 0xfff0);
    move_cursor(hl);

    return hl;
}

ushort move_up(ushort hl)
{
	restore_color(hl);

	//roll decrement
	hl = ((hl - 0x10) & 0xff) | (hl & 0xff00);
    move_cursor(hl);

    return hl;
}

ushort move_down(ushort hl)
{
	restore_color(hl);

	//roll increment
	hl = ((hl + 0x10) & 0xff) | (hl & 0xff00);
    move_cursor(hl);

    return hl;
}

void mem_dump(ushort address)
{
	byte a,b;
	ushort hl;

	hl = address & 0xFF00;

	printf("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");

    do
    {
		printf("%04X ",hl);

		b = 16;
		do
		{
#ifdef USE_CODE_TEST_BUFFER
			printf("%02X ",Z80Mem[hl++]);
#else
			printf("%02X ",z80memRd(0,hl++));
#endif

		} while (--b);

		hl -= 16;

		b = 16;
		do
		{
#ifdef USE_CODE_TEST_BUFFER
			a = Z80Mem[hl++];
#else
			a = z80memRd(0,hl++);
#endif
			if ((a < 0x20) || (a > 0x7f)) a = '.';
			putchar(a);
		} while (--b);

		printf("\r\n");

    } while (hl & 0x00f0);
}

void update_dump(ushort hl)
{
	//home
	printf("\033[m\033[2;1H");

	//redraw
	mem_dump(hl & 0xff00);
	move_cursor(hl);
}

//Byte edit
//+      increment
//-      decrement
//bsp    remove previous hex
//Z      00
//enter  accept byte and exit byte edit
//Q      scrap byte and exit byte edit
//
//c-A    left
//c-D    right
//c-W    up
//c-X    down
//H      L = 00
//P      previous 256-byte block
//N      next 256-byte block
//X      exit binary editor

void binary_ed(ushort address)
{
	ushort hl = address;
	byte a,b;

	restore_color(hl);

	printf("\033[2JBinary editor\r\n");
    mem_dump(address);

    printf("\r\nLeft: \033[37;42mctl-A\033[m  right: \033[37;42mctl-D\033[m  down: \033[37;42mctl-X\033[m up: \033[37;42mctl-W\033[m\r\n");
    printf("High: \033[37;42mH\033[m  low: \033[37;42mL\033[m  Relative jump: \033[37;42mJ\033[m  Absolute jump: \033[37;42mG\033[m\r\n");
    printf("Previous block: \033[37;42mP\033[m  next block: \033[37;42mN\033[m  store value: \033[37;42menter\033[m  exit editor: \033[37;42mX\033[m\r\n");

    //goto row 3 column 6
    printf("\033[3;6H");

    do
    {
        printf("\033[44m");

#ifdef USE_CODE_TEST_BUFFER
    	b = Z80Mem[hl];
#else
        b = z80memRd(0,hl);
#endif
        //byte edit
        a = in_b_k(&b);

        switch(a)
        {
        	//c-A arrow left
        	case 0x01:
        		hl = move_left(hl);
				break;

        	//c-D arrow right
        	case 0x04:
        		hl = move_right(hl);
				break;

        	//c-X arrow down
        	case 0x18:
        		hl = move_down(hl);
				break;

        	//c-W arrow up
        	case 0x17:
        		hl = move_up(hl);
				break;

        	// absolute jump
        	case 'G':
#ifdef USE_CODE_TEST_BUFFER
        		hl = b | (Z80Mem[hl+1] << 8);
#else
        		hl = b | (z80memRd(0,hl+1) << 8);
#endif
				update_dump(hl);
				break;

            // high -> H = b
        	case 'H':
				hl = b << 8;
				update_dump(hl);
				break;

			case 'L':
            	restore_color(hl);
            	hl = (hl & 0xff00) | b;
				move_cursor(hl);
				break;

            //relative jump
            case 'J':
            	restore_color(hl);

        		hl = hl + b + 1;
            	if (b > 0x7f) hl -= 0x100;

            	update_dump(hl);
            	break;

            //previous 256-byte block
            case 'P':
            	hl -= 256;
				//keep color
				update_dump(hl);
            	break;

            //next 256-byte block
            case 'N':
            	hl += 256;
				//keep color
				update_dump(hl);
            	break;

            case 'X':
            	//exit editor
            	restore_color(hl);
            	AnsiClearDisplay();
            	return;

            case 0x0d:
#ifdef USE_CODE_TEST_BUFFER
            	Z80Mem[hl] = b;
#else
            	z80memWr(0,hl,b);
#endif
            	break;

            default:
            	;
		}

        putchar('\b');
    	putchar('\b');

    } while (true);
}

