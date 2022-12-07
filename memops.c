/*---------------------------------------------------------------------------*
 * memops.c                                                                  *
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

void fillMemory(void)
{
	char c;
	ushort start, end;
	ushort i;
	byte data_byte;

	printf("Fill start: ");

	c = setaddress(&start);

	if (c != 'Q')
	{
		printf(" to end: ");
		c = setaddress(&end);
		if (c != 'Q')
		{
			printf(" value: ");
			c = setdatabyte(&data_byte);
			if (c != 'Q')
			{
				for (i = start; i <= end; i++)
				{
#ifdef USE_CODE_TEST_BUFFER
					Z80Mem[i] = data_byte;
#else
					z80memWr(0,i,data_byte);
#endif
				}
			}
		}
	}
}

void moveMemory(void)
{
	char c;
	ushort start = 0, end = 0, dest = 0;
	ushort i, length;
	byte data_byte;

	printf("Move start: ");

	c = setaddress(&start);

	if (c != 'Q')
	{
		printf(" to end: ");
		c = setaddress(&end);
		if (c != 'Q')
		{
			printf(" dest: ");
			c = setaddress(&dest);
			if (c != 'Q')
			{
				//Check for move
				if (start != dest)
				{
					length = end - start + 1;

					//Check direction to support overlap
					if (dest > start)
					{
						dest += (length -1);

						// From end to start
						i = length;
						do
						{
							putchar('-');
#ifdef USE_CODE_TEST_BUFFER
							data_byte = Z80Mem[end--];
							Z80Mem[dest--] = data_byte;
#else
							data_byte = z80memRd(0,end--);
							z80memWr(0,dest--,data_byte);
#endif
						} while (--i);
					}
					else
					{
						// From start to end
						i = length;
						do
						{
							putchar('+');
#ifdef USE_CODE_TEST_BUFFER
							data_byte = Z80Mem[start++];
							Z80Mem[start++] = data_byte;
#else
							data_byte = z80memRd(0,start++);
							z80memWr(0,dest++,data_byte);
#endif
						} while (--i);
					}
					printf("Move done!\r\n");
				}
				else
				{
					printf("No move: source = dest\r\n");
				}
			}
			else
			{
				printf("Cancelled move\r\n");
			}
		}
	}
	GetAnyKey();
}

void loadIntelHex(void)
{
	char line_input[128];
    char Data_Str[128];

	char *str;

	unsigned int nb_record;
	ushort BytesTotal;
	int result;

	unsigned int address;
	unsigned int nb_bytes, record_type;
	unsigned int data_byte, checksum;

	bool checksumBad = false;

	printf("\r\nWaiting for Hex file transfer\r\n");

	nb_record = 0;
	BytesTotal = 0;

	do
    {
		putchar('\021');	//XON
    	gets(line_input);
    	puts(line_input);
    	putchar('\023');	//XOFF

    	nb_record++;

        /* Remove carriage return/line feed at the end of line. */
    	removeStringTrailingNewline(line_input);

    	result = sscanf(line_input, ":%02X%04X%02X%s", &nb_bytes, &address, &record_type, Data_Str);
    	if (result != 4)
    	{
			printf("\r\nLine %d: data record, address: %04X, nb bytes: %d\r\n", nb_record, address, nb_bytes);
			printf("Data_Str: %s\r\n", Data_Str);
			printf("166: Error in line %d of hex file\n", nb_record);
    	}

    	checksum = nb_bytes + (address >> 8) + (address & 0xff) + record_type;
		str = Data_Str;

		switch(record_type)
		{
			case 0x00:
				printf("\r\nLine %d: data record, address: %04X, nb bytes: %d\r\n", nb_record, address, nb_bytes);
				// data records
				do
				{
					result = sscanf(str, "%02X", &data_byte);
					if (result != 1) printf("179: Error in line %d of hex file\r\n", nb_record);

					//store each byte
#ifdef USE_CODE_TEST_BUFFER
					Z80Mem[address++] = data_byte;
#else
					z80memWr(0,address++,data_byte);
#endif
					BytesTotal++;
					str += 2;

					//calculate checksum
					checksum += data_byte;
				} while (--nb_bytes);

				printf("nb_bytes: %d, calculated checksum: %02X\r\n", nb_bytes, checksum & 0xFF);

				//get checksum
				result = sscanf(str, "%02X", &data_byte);
				if (result != 1) printf("196: Error %d in line %d of hex file\n", result, nb_record);

				//verify checksum
				checksum += data_byte;

				if ((checksum & 0xFF) == 0) putchar('.');
				else
				{
					printf("203: Checksum error in line %d: should be %02X\n",
							nb_record, (256 - checksum) & 0xFF);

					checksumBad = true;
					putchar('*');
				}
				if ((nb_record % 32) == 0) printf("\r\n");
				break;

			case 0x01:
				printf("\r\nLine %d: last record, address: %04X, nb bytes: %d\r\n", nb_record, address, nb_bytes);
				//last record
				//get checksum
				sscanf(str, "%02X", &data_byte);

				//verify checksum
				checksum += data_byte;

				printf("nb_bytes: %d, calculated checksum: %02X\r\n", nb_bytes, checksum & 0xFF);

				checksumBad = ((checksum & 0xFF) != 0);

				//hex_end
				if (checksumBad)
				{
					printf("\r\nChecksum bad\r\n");
				}
				else
				{
					printf("\r\nTransfer completed, number of bytes received: %d\r\n",BytesTotal);
				}
				return;

			default:
				;
		}
    } while (true);
}








