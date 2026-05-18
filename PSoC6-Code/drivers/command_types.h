#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    type_bool,
    type_int,
    type_float,
    type_string,
    type_char,
    type_char_list,
    type_int_list,
    type_float_list,
    type_string_list,
    type_bool_list
} cli_arg_type_t;

/**
 * @brief Flag for a command
 * - long_flag: long flag for the command (required - MUST NOT be NULL)
 * - short_flag: short flag for the command (optional - can be NULL)
 * - summary: summary of the flag (required - MUST NOT be NULL)
 * - value: pointer to the value of the flag (read-only)
 * - value_len: length of the value if it's a list (read-only)
 */
typedef struct {
    const char *long_flag;
    const char *short_flag;
    const char *summary;
    void *value;
    int  *value_len;
    const cli_arg_type_t type;
    bool enabled;
} cli_flag_t;

/**
 * @brief Command for the CLI
 * - name: name of the command (required - MUST NOT be NULL)
 * - handler: handler for the command (required - MUST NOT be NULL)
 *   - takes parameters: parent command (cli_command_t*), arguments (char**), arguments length (int)
 * - init: init function for the command (optional - can be NULL)
 *   - returns: bool
 * - description: description of the command (required - MUST NOT be NULL)
 * - summary: summary of the command (required - MUST NOT be NULL)
 * - flags: flags for the command (optional - can be NULL)
 * - flags_len: length of the flags array (required - MUST be 0 if flags is NULL)
 */

typedef struct cli_command {
    const char *name;
    void (*handler)(struct cli_command*, char**, int);
    bool (*init)(void);
    const char *description;
    const char *summary;
    cli_flag_t *flags;
    const uint8_t flags_len;
    bool enabled;
} cli_command_t;

#endif