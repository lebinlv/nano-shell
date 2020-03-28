/**
 * @file nano_shell.c
 * @author Liber (lvlebin@outlook.com)
 * @brief nano-shell: a light but powerful shell designed for embedded systems.
 * @version 1.0
 * @date 2020-03-27
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#include <string.h>
#include "nano_shell.h"
#include "shell_io/shell_io.h"
#include "command/command.h"
#include "readline/readline.h"
#include "parse/text_parse.h"

#include "shell_config.h"


/**
 * @brief
 *
 * @param argc: MUST be larger than 0
 * @param argv:
 * @return int
 */
int nano_shell_run_cmd(int argc, char *const argv[])
{
  const shell_cmd_t *pCmdt = shell_find_cmd(argv[0]);

  if (pCmdt) {
    return pCmdt->cmd(pCmdt, argc, argv);
  }
  shell_printf("  %s: command not found.\r\n", argv[0]);

  return -1;
}


#if (CONFIG_SHELL_MAX_ARGC < 1)
#error "CONFIG_SHELL_MAX_ARGC must be no less than 1."
#endif

void nano_shell_loop(void *argument)
{
  static char *argv[CONFIG_SHELL_MAX_ARGC + 1];
  char *line;
  int argc;

  for (;;) {
    line = readline(CONFIG_SHELL_PROMPT);

    if (*line) {
      argc = nano_shell_parse_line(line, argv, CONFIG_SHELL_MAX_ARGC + 1);

      // shell_printf("[DEBUG] argc: %d\r\n", argc);
      // for (int i = 0; i < argc; i++) {
      //   shell_printf("[DEBUG] ARGV[%d]: %s\r\n", i, argv[i]);
      // }

      if (argc > CONFIG_SHELL_MAX_ARGC) {
        argc--;
        shell_printf("** WARNING: too many args (max: %d)! ", CONFIG_SHELL_MAX_ARGC);
        shell_printf("arguments after \"%s\" will be ignored. **\r\n", argv[argc - 1]);
      }

    if (argc > 0) {
      nano_shell_run_cmd(argc, argv);
    }
  }
}


void nano_shell_react_to_input(char ch)
{
  static char *argv[CONFIG_SHELL_MAX_ARGC + 1];
  int argc;

  char *line = readline_react(ch);

  if (line) {
    /**
     * in react mode, use if (* line) to avoid unnecessary process
     * to improve speed.
     */
    if (*line) {
      argc = nano_shell_parse_line(line, argv, CONFIG_SHELL_MAX_ARGC + 1);

      if (argc > CONFIG_SHELL_MAX_ARGC) {
        argc--;
        shell_printf("** WARNING: too many args (max: %d)! ", CONFIG_SHELL_MAX_ARGC);
        shell_printf("arguments after \"%s\" will be ignored. **\r\n", argv[argc - 1]);
      }

      if (argc > 0) {
        nano_shell_run_cmd(argc, argv);
      }
    }

    shell_puts(CONFIG_SHELL_PROMPT);
  }
}
