/**
 * @file command.h
 * @author Liber (lvlebin@outlook.com)
 * @brief
 * @version 1.0
 * @date 2020-03-23
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#ifndef __NANO_SHELL_COMMAND_H
#define __NANO_SHELL_COMMAND_H

#include "shell_linker.h"

#include "shell_config.h"

// shell command structure
typedef struct _shell_cmd_s {
  const char *name; // command name

  /**
   * @brief this is the implementation function of the command.
   *
   * @param pCmdt: pointer of the structure.
   * @param argc: the count of arguments.
   * @param argv: argument vector.
   * @return 0 if succeed, else non-zero. (return value is not used in ver1.0)
   *
   * @note the command name is the first argument, argv[0], so argc is always at least 1.
   */
  int (*cmd)(const struct _shell_cmd_s *pCmdt, int argc, char *const argv[]);

#if CONFIG_SHELL_CMD_BRIEF_USAGE
  const char *brief_usage; // brief usage of the command.
#endif

#if CONFIG_SHELL_CMD_LONG_HELP
  const char *help;  // detailed help information of the command.
#endif
} shell_cmd_t;


// shell function structure
typedef struct {
  const char *name;  // function name

  const int param_n; // number of parameters

  int (*func)(); // function pointr.

#if CONFIG_SHELL_FUNC_BRIEF_USAGE
  const char *brief;  // brief summaries of the command.
#endif
} shell_func_t;


#if CONFIG_SHELL_CMD_BRIEF_USAGE
  #define _CMD_BRIEF(x) x,
#else
  #define _CMD_BRIEF(x)
#endif /* CONFIG_SHELL_CMD_BRIEF_USAGE */

#if CONFIG_SHELL_CMD_LONG_HELP
  #define _CMD_HELP(x) x,
#else
  #define _CMD_HELP(x)
#endif /* CONFIG_SHELL_CMD_LONG_HELP */

#if CONFIG_SHELL_FUNC_BRIEF_USAGE
  #define _FUNC_BRIEF(x) x,
#else
  #define _FUNC_BRIEF(x)
#endif /* CONFIG_SHELL_FUNC_BRIEF_USAGE */



#define _shell_cmd_complete(_name, _func, _brief, _help) \
  { #_name, _func, _CMD_BRIEF(_brief) _CMD_HELP(_help) }


#define _shell_func_complete(_name, _nparam, _func, _brief) \
  { #_name, _nparam, _func, _FUNC_BRIEF(_brief) }


/**
 * @brief add a command to nano-shell
 *
 * @_name: name of the command. Note: THIS IS NOT a string.
 * @_func: function pointer: (*cmd)(const shell_cmd_t *, int, int, char *const[]).
 * @_brief: brief summaries of the command. This is a string.
 * @_help: detailed help information of the command. This is a string.
 */
#define NANO_SHELL_ADD_CMD(_name, _func, _brief, _help) \
  _shell_entry_declare(shell_cmd_t, _name) = _shell_cmd_complete(_name, _func, _brief, _help)


/**
 * @brief add a function to nano-shell.
 *
 * @_name: name of the function. Note: THIS IS NOT a string.
 * @_nparam: param num of the function.
 * @_func: pointer of the function.
 * @_brief: brief summaries of the function. This is a string.
 */
#define NANO_SHELL_ADD_FUNC(_name, _nparam, _func, _brief) \
  _shell_entry_declare(shell_func_t, _name) = _shell_func_complete(_name, _nparam, _func, _brief)


/**
 * @brief
 *
 * @param cmd_name
 * @return const shell_cmd_t*
 */
const shell_cmd_t *shell_find_cmd(const char *cmd_name);


/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int shell_run_cmd(int argc, char *const argv[]);


#endif /* __NANO_SHELL_COMMAND_H */
