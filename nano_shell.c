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

#include "shell_config.h"

#define isblank(c) ((c) == ' ' || (c) == '\t')

int nano_shell_parse_line(char *input, char *argv[], const int maxArgc)
{
  char current;
  int nargc = 0;

  while (nargc < maxArgc) {
    while (isblank(*input)) {
      input++;
    }
    if (*input == '\0') {  // end of input
      argv[nargc] = NULL;
      break;
    }

    current = *input;
    // single quotes ('') and double quotes ("")
    if (current == '\'' || current == '"') {
      argv[nargc] = ++input;
      while (*input != current) {
        input++;
      }
    } else {  // normal character
      argv[nargc] = input++;
      while (*input && !isblank(*input)) {
        input++;
      }
    }
    nargc++;
    if (*input != '\0') {
      *input++ = '\0'; /* terminate current arg */
    }
  }

  return nargc;
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
      shell_run_cmd(argc, argv);
    }
  }
}


void nano_shell_react_to_input(char ch)
{
  static char *argv[CONFIG_SHELL_MAX_ARGC + 1];
  int argc;

  char *line = readline_react(ch);

  if (line) {
    if (*line) {
      argc = nano_shell_parse_line(line, argv, CONFIG_SHELL_MAX_ARGC + 1);

      if (argc > CONFIG_SHELL_MAX_ARGC) {
        argc--;
        shell_printf("** WARNING: too many args (max: %d)! ", CONFIG_SHELL_MAX_ARGC);
        shell_printf("arguments after \"%s\" will be ignored. **\r\n", argv[argc - 1]);
      }
      shell_run_cmd(argc, argv);
    }

    shell_puts(CONFIG_SHELL_PROMPT);
  }
}
