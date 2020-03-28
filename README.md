# Nano-Shell

![welcome](doc/pic/nano_shell_welcome.png)

Nano-Shell is a light but powerful shell designed for embedded systems.

- with or without embedded operating system;
- `<main loop mode>` or `<react mode>`;
- highly configurable;
- command line editing, history record, multi-line input, hot key bind, etc...
- memory friendly: **NO** malloc and free;
- light (build with arm-none-eabi-gcc 7.3.1 20180622, -O3):
  |                                        | .text<sup>(1)</sup>  | .rodata | .bss<sup>(2)</sup> | .data |
  |:--------------------------------------:|:------:|:-------:|:-----:|:-----:|
  | main loop mode<br/>all configurations on  | 2.5KB  | 1.03KB  | 852B  | 8B    |
  | main loop mode<br/>all configurations off | 616B   | 600B    | 180B  | 0B    |
  | react mode<br/>all configurations on      | 2.52KB | 1.03KB  | 852B  | 8B    |
  | react mode<br/>all configurations off     | 608KB  | 600B    | 180B  | 0B    |

  > 1 include build in `help` command.<br/>
  > 2 include `input buffer`(default 128Bytes) and `hisroty record buffer`(defaut 5*(128+2)Bytes)


## Getting Start

### Configurations
TODO:

### Add Command

Commands are added to nano-shell by creating a new command structure.

This is done by first including `command/command.h`, then using the NANO_SHELL_ADD_CMD() macro
to fill in a shell_cmd_t struct.

``` c
NANO_SHELL_ADD_CMD(_name, _func, _brief, _help)
```
`_name`: name of the command. Note: **THIS IS NOT** a string.

`_func`: function pointer: (*cmd)(const shell_cmd_t *, int, int, char *const[]).

`_brief`: brief summaries of the command. This is a string.

`_help`: detailed help information of the command. This is a string.

Simple example:
```c
int _do_demo(const shell_cmd_t *pcmd, int argc, char *const argv[])
{
  for (int i=0; i<argc; i++) {
    shell_printf("  [DEMO] ARGV[%d]: %s\r\n", i, argv[i]);
  }
  return 0;
}

NANO_SHELL_ADD_CMD(demo,
                   _do_demo,
                   "a command demo",
                   "  It's detailed help information of demo command\r\n");
```
Run it:

![demo](doc/pic/command_demo.png)

![help](doc/pic/help_demo.png)

### Porting nano-shell to your project

#### 1. add nano-shell root path to your project include path.

#### 2. implement these functions([`@file shell_io.h`](/shell_io/shell_io.h)) in your project:

this file may help: [`/shell_io/shell_io.c`](/shell_io/shell_io.c).
```c
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
 * @brief printf() for nano-shell
 *
 */
extern int shell_printf(const char *format, ...) __attribute__((format(printf, 1, 2)));


/**
 * @brief: Get next character available from stream.
 *
 * @param ch: Return the character in `ch` if there was...
 * @return: Result is non-zero if there was a character, or 0 if there wasn't.
 *
 */
extern int shell_getc(char *ch);
```
> NOT USED in `<react mode>`.

Note:

If you run nano-shell in `<main loop mode>`, to avoid losing characters, you'd better use a low layer receive fifo.

Take uart for example, you can detect incoming data using interrupts and then store each received character in a first-in-first-out (FIFO) buffer:
```c
void your_uart_interrupt_handler(void)
{
  /* your uart receive code */
  char ch = uart_get_char();

  /* store character in fifo */
  fifo_push(ch);
}
```

then `shell_getc(char *ch)` may be:
```c
int shell_getc(char *ch)
{
  if (fifo_empty()) {  // if no character in fifo,
    return 0;          // return false
  }

  *ch = fifo_pop();  // fifo is not empty, get a character from fifo.
  return 1;          // return true
}
```
#### 3. then modify the configuration file: [`shell_config.h`](/shell_config.h)

#### 4. according to your system, you can:

- without os, main loop mode:
  ```c
  #include "nano_shell.h"

  int main(void)
  {
    /* system init code... */

    /* nano-shell infinite loop. */
    nano_shell_loop(NULL);
  }
  ```

- without os, react mode(non-block):
  for example:
  ```c
  void your_interrupt_handler (void) // such as uart receive interrupt handler
  {
    /* your uart receive code */
    char ch = uart_get_char();

    /* nano-shell isr interface */
    nano_shell_react(ch);
  }
  ```

  `nano_shell_react()` is non-blocked (unless there was an infinite loop in your command function), you can call it when get a new character.

  ~~It is recommended to disable some configurations in `shell_config.h` if it was called in interrupt.~~

- with os, take freertos for example:
  ```c
  #include "nano_shell.h"

  int main(void)
  {
    /* system init code... */

    /* create nano_shell task */
    TaskHandle_t shellTaskHandle = NULL;
    xTaskCreate(nano_shell_loop, "shellTask", <stack_size>, NULL,
                <task_priority>, &shellTaskHandle);

    /* start rtos task scheduler */
    vTaskStartScheduler();
  }
  ```

  When determining the stack size for nano-shell, you should consider the memory occupied by commands added in nano-shell.

#### 5. define nano_shell section in your linker script file:

add these 3 lines to your linker script file:
```ld
.nano_shell : {
  . = ALIGN(4);
  KEEP (*(SORT(.nano_shell*)))
  . = ALIGN(4);
 } >FLASH
```

#### 5. build, flash and try it.
