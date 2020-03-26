/**
 * @file shell_io.c
 * @author Liber (lvlebin@outlook.com)
 * @brief Example implementation of some functions in file "shell_io.h".
 * @version 1.0
 * @date 2020-03-24
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#include <stdio.h>
#include <stdarg.h>

#include "shell_io.h"
#include "shell_config.h"

/**
 * @brief This function should do the actual transmission of the character.
 *        It can also be implemented by macro definition, for example:
 *        ```
 *        #define low_level_write_char(ch) \
 *          do {                           \
 *            uart_send_char(ch);          \
 *          } while(0)
 *        ```
 *
 * @param ch the character to be transmitted.
 */
extern void low_level_write_char(char ch);


#if defined(__GNUC__)
#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */
#endif /* __GNUC__ */


/****************************************************************/

#if !(CONFIG_SHELL_PRINTF_BUFFER_SIZE > 0)
#error "the value of CONFIG_SHELL_PRINTF_BUFFER_SIZE must be greater than 0!"
#endif


__weak int shell_printf(const char *format, ...)
{
  static char shell_printf_buffer[CONFIG_SHELL_PRINTF_BUFFER_SIZE];

  int length = 0;

  va_list ap;
  va_start(ap, format);

  length = vsnprintf(shell_printf_buffer, CONFIG_SHELL_PRINTF_BUFFER_SIZE, format, ap);

  va_end(ap);

  for (int i = 0; i < length; i++) {
    low_level_write_char(shell_printf_buffer[i]);
  }

  return length;
}


__weak void shell_puts(const char *str)
{
  while (*str) {
    low_level_write_char(*str);
    str++;
  }
}


__weak void shell_putc(char ch)
{
  low_level_write_char(ch);
}
