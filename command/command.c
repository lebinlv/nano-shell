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


const shell_cmd_t *shell_find_cmd(const char *cmd_name)
{
  if (cmd_name == NULL) {
    return NULL;
  }

  const shell_cmd_t *tmp = _shell_entry_start(shell_cmd_t);
  unsigned int count = _shell_entry_count(shell_cmd_t);

  while (count) {
    if (strcmp(cmd_name, tmp->name) == 0) {
      return tmp;
    }
    count--;
    tmp++;
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
