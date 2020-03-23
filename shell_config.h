/**
 * @file shell_config.h
 * @author Liber (lvlebin@outlook.com)
 * @brief nano-shell configurations.
 * @version 1.0
 * @date 2020-03-18
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#ifndef __NANO_SHELL_CONFIG_H
#define __NANO_SHELL_CONFIG_H

/******************************* readline configuration ****************************/

/* command line input buffer size(byte) */
#define CONFIG_SHELL_INPUT_BUFFSIZE (127U)

/* set 1 to enable command line edit */
#define CONFIG_SHELL_LINE_EDITING 1

/* ESC Control Sequence detect, such as Home, Delete, Arrow, etc. */
#define CONFIG_SHELL_KEY_SEQ_BIND 1

/* set 1 to enable Backslash('\') for line continuation */
#define CONFIG_SHELL_MULTI_LINE 1


/**
 * set 0 to disable history record.
 *
 * nano-shell will take `CONFIG_SHELL_HIST_MIN_RECORD*(2+CONFIG_SHELL_INPUT_BUFFSIZE)` bytes to
 *   record **at least** `CONFIG_SHELL_HIST_MIN_RECORD` histroys.
 * the maximum number of history records depends on the average length of the input.
 */
#define CONFIG_SHELL_HIST_MIN_RECORD (5U)


/******************************* command configuration ****************************/

#define CONFIG_SHELL_FUNC_BRIEF 1

#define CONFIG_SHELL_CMD_BRIEF  1

#define CONFIG_SHELL_CMD_LONG_HELP 1

#endif /* __NANO_SHELL_CONFIG_H */
