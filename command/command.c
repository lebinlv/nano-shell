/**
 * @file command.c
 * @author Liber (lvlebin@outlook.com)
 * @brief
 * @version 1.0
 * @date 2020-03-24
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#include <string.h>

#include "shell_io/shell_io.h"
#include "command.h"

/****************************** build in command: help ******************************/
void shell_print_all_cmd(void)
{
  const shell_cmd_t *tmp = _shell_entry_start(shell_cmd_t);
  const shell_cmd_t *end = _shell_entry_end(shell_cmd_t);

  for(; tmp < end; tmp++) {
#if CONFIG_SHELL_CMD_BRIEF
    shell_printf("  %s: %s\r\n", tmp->name, tmp->brief);
#else
    shell_printf("  %s\r\n", tmp->name)
#endif
  }
}

void shell_cmd_help(const char *cmd_name)
{
#if CONFIG_SHELL_CMD_LONG_HELP
  const shell_cmd_t *tmp = shell_find_cmd(cmd_name);

  if (tmp) {
    shell_printf("%s: %s\r\n", cmd_name, tmp->brief);

    // use shell_putc instead of shell_printf
    char *str = tmp->help;
    while (*str) {
      shell_putc(*str);
      str++;
    }
  } else {
    shell_printf("  %s: command not found.\r\n");
  }
#endif /* CONFIG_SHELL_CMD_LONG_HELP */
}

int shell_cmd_help(const shell_cmd_t *pcmd, int argc, char *const argv[])
{
  if(argc == 1) {
    shell_puts("nano-shell, version 1.0.0\r\n"
#if CONFIG_SHELL_CMD_LONG_HELP
              "Type `help name' to find out more about the function `name'.\r\n"
#endif
              "\r\n");
    shell_print_all_cmd();
  }
#if CONFIG_SHELL_CMD_LONG_HELP
  else {
    for(int i=1; i<argc; i++) {
      shell_cmd_help(argv[i]);
    }
  }
#endif /* CONFIG_SHELL_CMD_LONG_HELP */
}

NANO_SHELL_ADD_CMD(help, shell_cmd_help, "help brief\r\n", "help long help\r\n");
/****************************** build in command: help ******************************/


const shell_cmd_t *shell_find_cmd(const char *cmd_name)
{
  if (cmd_name == NULL) {
    return NULL;
  }

  const shell_cmd_t *tmp = _shell_entry_start(shell_cmd_t);
  const shell_cmd_t *end = _shell_entry_end(shell_cmd_t);
  unsigned int len = strlen(cmd_name);

  for (; tmp < end; tmp++) {
    if (memcmp(cmd_name, tmp->name, len) == 0) {
      return tmp;
    }
  }
  return NULL;
}


int shell_run_cmd(int argc, char *const argv[])
{
  const shell_cmd_t *pCmdt = shell_find_cmd(argv[0]);

  if (pCmdt) {
    return pCmdt->cmd(pCmdt, argc, argv);
  }

  shell_printf("  %s: command not found.\r\n", argv[0]);
  return -1;
}
