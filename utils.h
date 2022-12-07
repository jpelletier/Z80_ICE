/*
 * utils.h
 *
 *  Created on: 25 nov. 2022
 *      Author: jpellet
 */

#ifndef UTILS_H_
#define UTILS_H_

void removeStringTrailingNewline(char *str);
void GetAnyKey(void);

void AnsiClearDisplay(void);
void AnsiSaveCursorPosition(void);
void AnsiRestoreCursorPosition(void);

int getBit(int bit, int value);
byte revert_bits(byte in);
ushort revert_word(ushort in);

char in_b_k(byte *data);
char setdatabyte(byte *data);
char setaddress(ushort *address);

bool Ascii2Hex(char *c);
bool SerialGetAsciiByte(byte *data);
bool SerialGetAsciiWord(ushort *addr);

void mem_dump(ushort address);
void PrintRegs(void);

bool is_return_inst(char *ascii_inst);

#endif /* UTILS_H_ */
