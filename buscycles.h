/*
 * buscycles.h
 *
 *  Created on: 25 nov. 2022
 *      Author: jpellet
 */

#include "z80_ice.h"

#ifndef BUSCYCLES_H_
#define BUSCYCLES_H_

byte z80Fetch(size_t param, ushort address);
void z80memWr(size_t param, ushort address, byte data);
byte z80memRd(size_t param, ushort address);
void z80ioWr(size_t param, ushort address, byte io_data);
byte z80ioRd(size_t param, ushort address);
byte z80iack(size_t param);

#endif /* BUSCYCLES_H_ */
