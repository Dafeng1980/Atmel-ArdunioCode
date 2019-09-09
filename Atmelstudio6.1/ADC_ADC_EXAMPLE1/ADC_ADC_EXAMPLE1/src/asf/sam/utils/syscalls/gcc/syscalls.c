/**
 * \file
 *
 * \brief Syscalls for SAM (GCC).
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "board.h"
#include "uart.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#undef errno
extern int errno;
extern int _end;

extern caddr_t _sbrk(int incr);
extern int link(char *old, char *new);
extern int _close(int file);
extern int _fstat(int file, struct stat *st);
extern int _isatty(int file);
extern int _lseek(int file, int ptr, int dir);
extern int _read(int file, char *ptr, int len);
extern int _write(int file, char *ptr, int len);
extern void _exit(int status);
extern void _kill(int pid, int sig);
extern int _getpid(void);

extern caddr_t _sbrk(int incr)
{
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}
	prev_heap = heap;

	heap += incr;

	return (caddr_t) prev_heap;
}

extern int link(char *old, char *new)
{
	return -1;
}

extern int _close(int file)
{
	return -1;
}

extern int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;

	return 0;
}

extern int _isatty(int file)
{
	return 1;
}

extern int _lseek(int file, int ptr, int dir)
{
	return 0;
}

extern int _read(int file, char *ptr, int len)
{
	size_t nChars = 0;
	uint8_t c = 0;

	for (; len > 0; --len) {
		while (uart_read(CONSOLE_UART, &c));

		if (c == 0) {
			break;
		}
		*ptr++ = c;
		++nChars;
	}

	return nChars;
}

extern int _write(int file, char *ptr, int len)
{
	int iIndex;

	for (iIndex = 0; iIndex < len; iIndex++, ptr++) {
		while (!uart_is_tx_ready(CONSOLE_UART));
		uart_write(CONSOLE_UART, *ptr);
	}

	return iIndex;
}

extern void _exit(int status)
{
	printf("Exiting with status %d.\n", status);

	for (;;);
}

extern void _kill(int pid, int sig)
{
	return;
}

extern int _getpid(void)
{
	return -1;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
