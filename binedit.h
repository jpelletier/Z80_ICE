/*
 * binedit.h
 *
 *  Created on: Nov 26, 2022
 *      Author: jpellet
 */

#ifndef BINEDIT_H_
#define BINEDIT_H_

void move_cursor(ushort hl);
void restore_color(ushort address);
ushort move_left(ushort hl);
ushort move_right(ushort hl);
ushort move_up(ushort hl);
ushort move_down(ushort hl);
void mem_dump(ushort address);
void update_dump(ushort hl);
void binary_ed(ushort address);

#endif /* BINEDIT_H_ */
