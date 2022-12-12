/*
 * config.h
 *
 *  Created on: 3 déc. 2022
 *      Author: jpellet
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// implement an on chip memory instead of the target memory
// for testing purposes
//#define USE_CODE_TEST_BUFFER

// to improve performances for loading Intel hex files
//#define USE_CODE_SERIAL_INPUT_BUFFER
#define USE_CODE_RING_BUFFER

//Allow to debug the Intel hex download code
//#define USE_CODE_DEBUG_INTEL_HEX

#endif /* CONFIG_H_ */
