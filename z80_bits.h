/*
 * z80_bits.h
 *
 *  Created on: 3 déc. 2022
 *      Author: jpellet
 */

#ifndef Z80_BITS_H_
#define Z80_BITS_H_

//nHALT		F9
//nBUSAK	F10
//nBUSRQ	F0
//nRESET	F1
//nNMI		F2
//nINT		F3
//nWAIT		F4
//CLK		F5

//nRFSH		C13
//nM1		C12
//nIORQ		C11
//nMREQ		C10
//nWR		C9
//nRD		C8

#define BIT_HALT	9
#define BIT_BUSAK	10
#define BIT_BUSRQ	0
#define BIT_RST		1
#define BIT_NMI		2
#define BIT_INT		3
#define BIT_WAIT	4
#define BIT_CLK		5

#define BIT_RFSH	13
#define BIT_M1		12
#define BIT_IORQ	11
#define BIT_MREQ	10
#define BIT_WR		9
#define BIT_RD		8

#define MASK_HALT	(1<<BIT_HALT)
#define MASK_BUSAK	(1<<BIT_BUSAK)
#define MASK_BUSRQ	(1<<BIT_BUSRQ)
#define MASK_RST	(1<<BIT_RST)
#define MASK_NMI	(1<<BIT_NMI)
#define MASK_INT	(1<<BIT_INT)
#define MASK_WAIT	(1<<BIT_WAIT)
#define MASK_CLK	(1<<BIT_CLK)

#define MASK_RFSH	(1<<BIT_RFSH)
#define MASK_M1		(1<<BIT_M1)
#define MASK_IORQ	(1<<BIT_IORQ)
#define MASK_MREQ	(1<<BIT_MREQ)
#define MASK_WR		(1<<BIT_WR)
#define MASK_RD		(1<<BIT_RD)

#endif /* Z80_BITS_H_ */
