// All credit to Carmine Noviello for this code
// https://github.com/cnoviello/mastering-stm32/blob/master/nucleo-f030R8/system/src/retarget/retarget.c

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
//#include "../Inc/retarget.h"
#include <stdint.h>
#include <stdio.h>

#include "z80_ice.h"

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#ifdef USE_CODE_SERIAL_INPUT_BUFFER
#define UART2_RX_BUFFER_SIZE 4096

char Uart2_Rx_Buffer[UART2_RX_BUFFER_SIZE];
#endif

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart) {
  gHuart = huart;

  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);

  //allow for uploading Intel Hex files
#ifdef USE_CODE_SERIAL_INPUT_BUFFER
  setvbuf(stdin, Uart2_Rx_Buffer, _IOFBF, UART2_RX_BUFFER_SIZE);
#endif
}

int _write(int fd, char* ptr, int len) {
#ifdef USE_CODE_RING_BUFFER
  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
  {
	Uart_write(*ptr);
	return 1;
  }
#else //USE_CODE_RING_BUFFER
  HAL_StatusTypeDef hstatus;

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
  {
    hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return EIO;
  }
#endif //USE_CODE_RING_BUFFER
  errno = EBADF;
  return -1;
}

int _read(int fd, char* ptr, int len)
{
#ifdef USE_CODE_RING_BUFFER
  if (fd == STDIN_FILENO)
  {
	  if (IsDataAvailable() > 0)
	  {
		  *ptr = Uart_read();
		  return 1;
	  } else return EIO;
  }
#else //USE_CODE_RING_BUFFER
  HAL_StatusTypeDef hstatus;

  if (fd == STDIN_FILENO) {
    hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return 1;
    else
      return EIO;
  }
#endif //USE_CODE_RING_BUFFER

  errno = EBADF;
  return -1;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)
