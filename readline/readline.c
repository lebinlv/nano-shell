/**
 * @file readline.c
 * @author Liber (lvlebin@outlook.com)
 * @brief readline component of nano-shell
 * @version 1.0
 * @date 2020-03-21
 *
 * @copyright Copyright (c) Liber 2020
 *
 */

#include <string.h>

#include "readline.h"
#include "shell_io.h"
#include "history.h"
#include "key_seq.h"

#include "shell_config.h"


#define CTL_CH(ch) ((ch) - 'A' + 1)
#define U_SHELL_ALERT() shell_putc('\a')

// erase sequence, used to erase one character on screen.
static const char _erase_seq[] = "\b \b";

// console input buffer
static char _rl_line_buffer[CONFIG_SHELL_INPUT_BUFFSIZE + 1];

// if non-zero, readline return the current line immediately
static int _rl_done;

/**
 * The number of characters present in `_rl_line_buffer`.
 * 0 <= `_rl_end` <= `CONFIG_SHELL_INPUT_BUFFSIZE`
 * When `_rl_cursor` is at the end of the line, `_rl_cursor` and `_rl_end` are equal.
 * Note that the value of `_rl_line_buffer[_rl_end]` should be `\0` in any case.
 */
static int _rl_end;


#if CONFIG_SHELL_LINE_EDITING
  // The offset of the current cursor position in `_rl_line_buffer`
  // 0 <= `_rl_cursor` <= `_rl_end`
  static int _rl_cursor;
#endif /* CONFIG_SHELL_LINE_EDITING */


#if CONFIG_SHELL_KEY_SEQ_BIND
  /* uesed by @file{key_seq.c} to recognize key sequences */
  int _rl_key_seq_len;
#endif


void rl_endline(void)
{
#if CONFIG_SHELL_HIST_MIN_RECORD > 0
  if (*_rl_line_buffer) {
    rl_history_add(_rl_line_buffer);
  }
#endif /*CONFIG_SHELL_HIST_MIN_RECORD */

#if CONFIG_SHELL_LINE_EDITING
  _rl_cursor = 0;
#endif /* CONFIG_SHELL_LINE_EDITING */

  _rl_end = 0;
  _rl_done = 1;
  shell_puts("\r\n");
}

// add one character to the buffer
void rl_add_char(char ch)
{
  if (_rl_end < CONFIG_SHELL_INPUT_BUFFSIZE && ch >= ' ') {
#if CONFIG_SHELL_LINE_EDITING
    int len = _rl_end - _rl_cursor;

    _rl_end++;
    _rl_line_buffer[_rl_end] = '\0';

    memmove(&_rl_line_buffer[_rl_cursor + 1], &_rl_line_buffer[_rl_cursor], len);
    _rl_line_buffer[_rl_cursor] = ch;

    shell_puts(&_rl_line_buffer[_rl_cursor++]);
    while (len > 0) {
      shell_putc('\b');
      len--;
    }
#else
    shell_putc(ch);
    _rl_line_buffer[_rl_end++] = ch;
    _rl_line_buffer[_rl_end] = '\0';
#endif /* CONFIG_SHELL_LINE_EDITING */
  } else {
    U_SHELL_ALERT();
  }
}

// Rubout the character behind `_rl_cursor`(Backspace).
void rl_rubout(void)
{
#if CONFIG_SHELL_LINE_EDITING
  if (_rl_cursor > 0) {
    int len = _rl_end - (--_rl_cursor);
    _rl_end--;

    memmove(&_rl_line_buffer[_rl_cursor], &_rl_line_buffer[_rl_cursor + 1], len);
    shell_putc('\b');
    shell_puts(&_rl_line_buffer[_rl_cursor]);
    shell_putc(' ');
    do {
      shell_putc('\b');
      len--;
    } while (len > 0);
#else
  if (_rl_end > 0) {
    _rl_end--;
    _rl_line_buffer[_rl_end] = '\0';
    shell_puts(_erase_seq);
#endif /* CONFIG_SHELL_LINE_EDITING */
  } else {
    U_SHELL_ALERT();
  }
}


#if CONFIG_SHELL_HIST_MIN_RECORD > 0
void rl_process_history(const char *history)
{
  if (history) {
#if CONFIG_SHELL_LINE_EDITING
    shell_puts(&_rl_line_buffer[_rl_cursor]);  // move cursor to the end on screen.
#endif

    while (_rl_end > 0) {       // erase all on the screen.
      shell_puts(_erase_seq);
      _rl_end--;
    }

    _rl_end = strlen(history);  // update _rl_end.

#if CONFIG_SHELL_LINE_EDITING
    _rl_cursor = _rl_end;       // update _rl_cursor if LINE_EDITING is enabled.
#endif

    memcpy(_rl_line_buffer, history, _rl_end + 1);
    shell_puts(_rl_line_buffer);  // display new text and move cursor to the end on screen.
  } else {
    U_SHELL_ALERT();
  }
}

void rl_get_pre_history(void)
{
  rl_process_history(rl_history_prev());
}

void rl_get_next_history(void)
{
  rl_process_history(rl_history_next());
}
#endif /* CONFIG_SHELL_HIST_MIN_RECORD > 0 */


#if CONFIG_SHELL_LINE_EDITING
// Delete the character under the cursor (Delete).
void rl_delete(void)
{
  if (_rl_cursor < _rl_end) {
    int len = _rl_end - _rl_cursor;
    _rl_end--;

    memmove(&_rl_line_buffer[_rl_cursor], &_rl_line_buffer[_rl_cursor + 1], len);
    shell_puts(&_rl_line_buffer[_rl_cursor]);
    shell_putc(' ');
    do {
      shell_putc('\b');
      len--;
    } while (len > 0);
  }
}

// Move curosr to the beginning of line.
void rl_line_home(void)
{
  while (_rl_cursor) {
    shell_putc('\b');
    _rl_cursor--;
  }
}

// Move cursor to the end of line.
void rl_line_end(void)
{
  shell_puts(_rl_line_buffer + _rl_cursor);
  _rl_cursor = _rl_end;
}

// Move forward (left).
void rl_forward_cursor(void)
{
  if (_rl_cursor > 0) {
    shell_putc('\b');
    _rl_cursor--;
  } else {
    U_SHELL_ALERT();
  }
}

// Move backward (right).
void rl_backward_cursor(void)
{
  if (_rl_cursor < _rl_end) {
    shell_putc(_rl_line_buffer[_rl_cursor]);
    _rl_cursor++;
  } else {
    U_SHELL_ALERT();
  }
}

// Erase from beginning of line to cursor.
void rl_erase_all_backward(void)
{
  if (_rl_cursor > 0) {
    int len = _rl_end - _rl_cursor + 1;

    shell_puts(&_rl_line_buffer[_rl_cursor]);  // move cursor to the end on screen.
    while (_rl_end > 0) {                       // erase all on the screen
      shell_puts(_erase_seq);
      _rl_end--;
    }

    memmove(_rl_line_buffer, &_rl_line_buffer[_rl_cursor], len--);  // new text.
    shell_puts(_rl_line_buffer);  // display new text and move cursor to the end on screen.

    _rl_cursor = 0;
    _rl_end = len;
    while (len > 0) {  // move cursor to the begin on the screen.
      shell_putc('\b');
      len--;
    }
  }
}

// Erase from cursor to end of line.
void rl_erase_all_forward(void)
{
  shell_puts(&_rl_line_buffer[_rl_cursor]);  // move cursor to the end on screen.
  while (_rl_end > _rl_cursor) {
    shell_puts(_erase_seq);  // erase all right to _rl_cursor, and move screen cursor to _rl_cursor.
    _rl_end--;
  }
  _rl_line_buffer[_rl_end] = '\0';
}
#endif /* CONFIG_SHELL_LINE_EDITING */


void rl_dispatch(char ch)
{
#if CONFIG_SHELL_KEY_SEQ_BIND
  if (_rl_key_seq_len) {
    rl_dispatch_seq(ch);
    return;
  }
#endif /* CONFIG_SHELL_KEY_SEQ_BIND */

  switch (ch) {
    case '\r':  // CTL_CH('M')
    case '\n':  // CTL_CH('J')
      rl_endline();
      break;

    case CTL_CH('C'):
      shell_puts("^C\r\n");
      *_rl_line_buffer = '\0';
      rl_endline();
      break;

    case 255:
    case 127:
    case 8:  // backspace, CTL_CH('H')
      rl_rubout();
      break;

#if CONFIG_SHELL_KEY_SEQ_BIND
    case '\033':  // ESC(\033)
      if (_rl_key_seq_len == 0) {
        rl_dispatch_seq(ch);
      }
      break;
#endif /* CONFIG_SHELL_KEY_SEQ_BIND */


#if CONFIG_SHELL_LINE_EDITING
    case CTL_CH('A'):  // HOME
      rl_line_home();
      break;
    case CTL_CH('E'):  // END
      rl_line_end();
      break;

    case CTL_CH('B'):  // <-- (left arrow)
      rl_forward_cursor();
      break;
    case CTL_CH('F'):  // --> (right arrow)
      rl_backward_cursor();
      break;


    case CTL_CH('K'):  // Delete all characters on the right side.
      rl_erase_all_forward();
      break;
    case CTL_CH('U'):  // Delete all characters one the left side.
      rl_erase_all_backward();
      break;

    case CTL_CH('D'):  // DELETE
      rl_delete();
      break;

    case CTL_CH('X'):
    case CTL_CH('O'):
      break;
#endif /* CONFIG_SHELL_LINE_EDITING */


#if CONFIG_SHELL_HIST_MIN_RECORD > 0
    case CTL_CH('P'):  // up arrow
      rl_get_pre_history();
      break;
    case CTL_CH('N'):  // down arrow
      rl_get_next_history();
      break;
#endif /* CONFIG_SHELL_HIST_MIN_RECORD > 0 */

    default:  // add current character to the buffer
      rl_add_char(ch);
      break;
  }
}


char *readline(const char *promot)
{
  char input;

  if (promot) {
    shell_puts(promot);
  }

  _rl_done = 0;
  *_rl_line_buffer = '\0';
  while (_rl_done == 0) {
    while (!shell_getc(&input)) {
    }

    rl_dispatch(input);
  }

  return _rl_line_buffer;
}
