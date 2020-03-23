/**
 * @file shell_io.h
 * @author Liber (lvlebin@outlook.com)
 * @brief I/O interface
 * @version 1.0
 * @date 2020-03-17
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#ifndef __NANO_SHELL_IO_H
#define __NANO_SHELL_IO_H


/*********************************************************************
   nano-shell uses these functions to get/send character from stream.
   You should implement these functions in your project.
 *********************************************************************/

/**
 * @brief send a chararcter...
 *
 */
extern void shell_putc(char ch);


/**
 * @brief send string...
 *
 */
extern void shell_puts(const char *str);


/**
 * @brief Get next character available from stream.
 *        not used in interrupt mode.
 *
 * @param ch Return the character in `ch` if there was...
 * @return Result is non-zero if there was a character, or 0 if there wasn't.
 */
extern int shell_getc(char *ch);


#endif /* __NANO_SHELL_IO_H */
