# Nano-Shell

Nano-Shell is a light but powerful shell designed for embedded systems.

- with or without embedded operating system;
- main loop mode or interrupt mode;
- flash friendly: less than 2.5KB(build with arm-none-eabi-gcc 7.3.1 20180622, -O3);
- memory friendly: no malloc and free;
- highly configurable;
- command line editing, history record, auto complete, hot key bind, etc...


## 简介
TODO:

## 功能
TODO:

## 配置说明
TODO:

## 移植

1. first implement these functions(@file [`shell_io.h`](/shell_io.h)) in your project:
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
    * @brief Get next character available from stream.
    *
    * @param ch Return the character in `ch` if there was...
    * @return Result is non-zero if there was a character, or 0 if there wasn't.
    */
   extern int shell_getc(char *ch);
   ```
2. then modify the configuration file [`shell_config.h`](/shell_config.h)
3. without os, main loop mode:
   ```c
   while (1) {
       nano_shell_loop(NULL); // nano-shell interface
   }
   ```
   without os, interrupt mode:
   ```c
   void your_interrupt_handler (void) // for example, uart receive interrupt handler
   {
       char ch = uart_get_char(); // your function to get a char

       nano_shell_run_in_isr(ch); // nano-shell isr interface
   }
   ```
   with os, take freertos for example:
   ```c
   void main()
   {
       TaskHandle_t shellTaskHandle = NULL;
       xTaskCreate(nano_shell_loop, "shellTask", 256, NULL,
                   priorityCMSIS2RTOS(osPriorityIdle), &shellTaskHandle);
   }
   ```
4. build, flash and try it.
