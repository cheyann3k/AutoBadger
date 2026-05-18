#include "cli.h"

// #include "console.h"
// #include "spi.h"
// #include "eeprom.h"
// #include "lsm6dsm.h"

// Internal Global variables for CLI
char** cont_argv = NULL;
cyhal_timer_t cont_timer;
int cont_argc = 0;
uint32_t cont_del = 0;
const cyhal_timer_cfg_t cont_timer_cfg =
{
    .compare_value = 0,                  // Timer compare value, not used
    .period        = 1000000,            // Timer period set to a large enough value
                                         //   compared to event being measured
    .direction     = CYHAL_TIMER_DIR_UP, // Timer counts up
    .is_compare    = false,              // Don't use compare mode
    .is_continuous = false,              // Do not run timer indefinitely
    .value         = 0                   // Initial value of counter
};

bool using_ble = false;

cli_command_t* AVAILABLE_COMMANDS;
int AVAILABLE_COMMANDS_LEN;
 
    

/**
 * @brief Parse an argument.
 * 
 * The argument can be a boolean (true or false), int, float, string, char, or list.
 * 
 * @note This function assumes strings, chars, and lists are surrounded by "", '', [] respectively
 * 
 * @param arg the argument to parse
 * @param value_len the length of the value if it's a list
 * @return void* the parsed value as a pointer
 */
void* cli_args_parser(char* arg, int* value_len, cli_arg_type_t type) {
    switch (type) {
        case type_int: {
            if (!is_int(arg)) {
                printf("Invalid int argument\n");
                return NULL;
            }

            if (value_len != NULL) {
                *value_len = -1;
            }

            int* value = malloc(sizeof(int));
            *value = atoi(arg);
            return value;
        }
        case type_float: {
            if (!is_float(arg)) {
                printf("Invalid float argument\n");
                return NULL;
            }

            if (value_len != NULL) {
                *value_len = -1;
            }
            
            float* value = malloc(sizeof(float));
            *value = atof(arg);
            return value;
        }
        case type_string: {
            if (arg[0] != '"') {
                printf("Invalid string argument\n");
                return NULL;
            }
            
            if (value_len != NULL) {
                *value_len = -1;
            }

            char* value = malloc(*value_len - 1);
            strcpy(value, arg+1);
            value[strlen(value)-1] = '\0';
            return value;
        }
        case type_char: {
            if (arg[0] != '\'') {
                printf("Invalid char argument\n");
                return NULL;
            }

            if (value_len != NULL) {
                *value_len = -1;
            }

            char* value = malloc(sizeof(char));
            *value = arg[1];
            return value;
        }
        case type_bool: {
            for (int i = 0; i < strlen(arg); i++) {
                arg[i] = tolower(arg[i]);
            }

            if (value_len != NULL) {
                *value_len = -1;
            }

            if (strcmp(arg, "true") == 0) {
                bool* value = malloc(sizeof(bool));
                *value = true;
                return value;
            } else if (strcmp(arg, "false") == 0) {
                ;
                bool* value = malloc(sizeof(bool));
                *value = false;
                return value;
            } else {
                printf("Invalid bool argument\n");
                return NULL;
            }
        }
        case type_char_list: {
            char** list = cli_arg_splitter(arg, value_len, true);

            if (list == NULL) {
                free_string_list(list, *value_len);
                printf("Invalid char list argument - invalid list\n");
                return NULL;
            }

            // Remove surrounding quotes and Convert to char array (instead of char* array)
            char* value = malloc(*value_len * sizeof(char));

            for (int i = 0; i < *value_len; i++) {
                int len = strlen(list[i]);

                if (len != 3) {
                    free_string_list(list, *value_len);
                    free(value);
                    printf("Invalid char list argument - char in list is more/less than one char\n");
                    return NULL;
                } else if (list[i][0] != '"' || list[i][len-1] != '"') {
                    free_string_list(list, *value_len);
                    free(value);
                    printf("Invalid char list argument - not surrounded by '\n");
                    return NULL;
                }

                value[i] = list[i][1];
            }
            
            free_string_list(list, *value_len);
            return value;
        }
        case type_string_list: {
            char** list = cli_arg_splitter(arg, value_len, true);

            if (list == NULL) {
                free_string_list(list, *value_len);
                printf("Invalid string list argument - invalid list\n");
                return NULL;
            }

            // Remove surrounding quotes
            for (int i = 0; i < *value_len; i++) {
                int len = strlen(list[i]);
                if (list[i][0] != '"' || list[i][len-1] != '"') {
                    printf("Invalid string list argument - not surrounded by \"\n");
                    return NULL;
                }
                list[i][len-1] = '\0';
                strcpy(list[i], list[i]+1);
            }

            return list;
        }
        case type_int_list: {
            char** list = cli_arg_splitter(arg, value_len, true);

            if (list == NULL) {
                free_string_list(list, *value_len);
                printf("Invalid int list argument - invalid list\n");
                return NULL;
            }

            // Convert to int array (instead of int* array) and convert elements to ints
            int* value = malloc(*value_len * sizeof(int));

            for (int i = 0; i < *value_len; i++) {
                if (!is_int(list[i])) {
                    free_string_list(list, *value_len);
                    free(value);
                    printf("Invalid int list argument - invalid int in list\n");
                    return NULL;
                }
                value[i] = atoi(list[i]);
            }

            free_string_list(list, *value_len);
            return value;
        }
        case type_float_list: {
            char** list = cli_arg_splitter(arg, value_len, true);

            if (list == NULL) {
                free_string_list(list, *value_len);
                printf("Invalid int list argument - invalid list\n");
                return NULL;
            }

            // Convert to float array (instead of float* array) and convert elements to floats
            float* value = malloc(*value_len * sizeof(float));

            for (int i = 0; i < *value_len; i++) {
                if (!is_float(list[i])) {
                    free_string_list(list, *value_len);
                    free(value);
                    printf("Invalid float list argument - invalid float in list\n");
                    return NULL;
                }
                value[i] = atof(list[i]);
            }

            free_string_list(list, *value_len);
            return value;
        }
        case type_bool_list: {
            char** list = cli_arg_splitter(arg, value_len, true);

            if (list == NULL) {
                free_string_list(list, *value_len);
                printf("Invalid bool list argument - invalid list\n");
                return NULL;
            }

            // Convert to bool array (instead of bool* array) and convert elements to bools
            bool* value = malloc(*value_len * sizeof(bool));

            for (int i = 0; i < *value_len; i++) {
                for (int j = 0; j < strlen(list[i]); j++) {
                    list[i][j] = tolower(list[i][j]);
                }

                if (strcmp(list[i], "true") == 0) {
                    value[i] = true;
                } else if (strcmp(list[i], "false") == 0) {
                    value[i] = false;
                } else {
                    free_string_list(list, *value_len);
                    free(value);
                    printf("Invalid bool list argument - invalid bool in list\n");
                    return NULL;
                }
            }

            free_string_list(list, *value_len);
            return value;
        }
        default: {
            printf("Invalid argument type\n");
            return NULL;
        }
    }
}

void cli_flags_parser(cli_command_t* command, char** argv, int argc) {
    for (int i = 0; i < command->flags_len; i++) {
        if (!command->flags[i].enabled) continue;
        if (command->flags[i].value != NULL) {
            free(command->flags[i].value);
            command->flags[i].value = NULL;
        }

        bool has_short_flag = command->flags[i].short_flag != NULL;

        for (int a = 0; a < argc; a += 2) {
            if ((has_short_flag && strcmp(argv[a], command->flags[i].short_flag) == 0) || 
                strcmp(argv[a], command->flags[i].long_flag) == 0) {
                if (a+1 < argc) {
                    command->flags[i].value = cli_args_parser(argv[a+1], command->flags[i].value_len, command->flags[i].type);
                } 
                break;
            }
        }
    }
}

bool cli_command_handler(cli_command_t* parent, cli_command_t* commands, int commands_len, 
                         char** argv, int argc) {
    if (argc != 0) {
        if (strcmp(argv[0], "help") == 0) {
            cli_help(parent, commands, commands_len);
            return true;
        }
        for (int i = 0; i < commands_len; i++) {
            if (commands[i].enabled && strcmp(argv[0], commands[i].name) == 0) {
                // Updated argv and argc to not include the command name
                commands[i].handler(&commands[i], argv+1, argc-1);
                return true;
            }
        }
    }
    return false;
}

bool cli_help_handler(cli_command_t* command, char** argv, int argc) {
    return cli_command_handler(command, NULL,0, argv, argc);
}

void cli_help(cli_command_t* command, cli_command_t* subcommands, int subcommands_len) {
    bool is_command = command != NULL && command->enabled;
    bool has_subcommands = subcommands_len > 0;

    if (is_command) {
        printf("%s\n%s", command->name, command->description);

        if (command->flags_len > 0) {
            printf("\n\nAvailable flags:\n");
            for (int i = 0; i < command->flags_len; i++) {
                if (!command->flags[i].enabled) continue;
                printf("\t");
                if (command->flags[i].short_flag != NULL) {
                    printf("%s, ", command->flags[i].short_flag);
                }
                printf("%s - %s\n", command->flags[i].long_flag, command->flags[i].summary);
            }
        }
    }

    if (has_subcommands) {
        if (is_command) {
            printf("\n\nAvailable subcommands:\n");
        } else {
            printf("Available commands:\n");
        }
    
        for (int i = 0; i < subcommands_len; i++) {
            if (!subcommands[i].enabled) continue;
            printf("  %s - %s\n", subcommands[i].name, subcommands[i].summary);
        }
    }
}


char** cli_arg_splitter(char* args, int* split_len, bool for_list) {
    bool in_quotes = false;
    bool in_single_quotes = false;
    bool in_backslash = false;

    int single_qoutes_cnt = 0;
    int len = strlen(args);
    int elements = 0;

    char* new_args = malloc(len * sizeof(char));

    int new_args_index = 0;

    for (int i = 0; i < len; i++) {
        if (args[i] == '"' && !in_single_quotes && !in_backslash) {
            in_quotes = !in_quotes;
        } else if (args[i] == '\'' && !in_quotes) {
            if (in_single_quotes && single_qoutes_cnt == 0) {
                printf("Error parsing arguments - char is empty\n");
                return NULL;
            }
            in_single_quotes = !in_single_quotes;
            single_qoutes_cnt = 0;
        } 
        if (args[i] == '\\' && !in_backslash) {
            in_backslash = true;
        } else {
            in_backslash = false;
        }

        if (in_single_quotes) {
            if ((single_qoutes_cnt > 1 && !in_backslash) || single_qoutes_cnt > 2) {
                printf("Error parsing arguments - char is more than 1 char\n");
                return NULL;
            }
            single_qoutes_cnt++;
        }
        
        // printf("%c - In Quotes: %i\n", args[i], elements);

        if (((for_list && args[i] == ',') || (!for_list && args[i] == ' ')) && 
              !in_quotes && !in_single_quotes) {
            new_args[new_args_index] = '\0';
            new_args_index++;
            elements++;
        } else if (in_quotes || in_single_quotes || !for_list || args[i] != ' ') {
            new_args[new_args_index] = args[i];
            new_args_index++;
        }
    }

    new_args[new_args_index] = '\0';

    if (in_quotes || in_single_quotes) {
        printf("Error parsing arguments - missing closing quotes\n");
        return NULL;
    } else if (in_backslash) {
        printf("Error parsing arguments - invalid backslash\n");
        return NULL;
    }

    char** split = malloc((elements+1) * sizeof(char*));

    int split_index = 0;
    // int new_args_len = 0;

    for (int i = 0; i <= elements; i++) {
        // new_args_len = strlen(new_args + (split_index)*(sizeof(char)));
        // split[i] = malloc(new_args_len * sizeof(char));
        // strcpy(split[i], new_args + (split_index)*(sizeof(char)));
        split[i] = strdup(new_args + (split_index)*(sizeof(char)));
        split_index +=  strlen(split[i])+1;
    }
    free(new_args);
    *split_len = elements+1;
    return split;
}

bool cli_init(cli_command_t* COMMANDS, int COMMANDS_LEN, bool use_ble) {
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        return false;
    }

    // Always init conosle for debugging
    console_init();

    using_ble = use_ble;

    if (using_ble) {
        printf("BLE INIT\r\n");
        ble_cli_init();
        printf("BLE INITIALIZED\r\n");
    }

    printf("TIMER INIT\r\n");
    result = cyhal_timer_init(&cont_timer, NC, NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
        return false;
    } else {
        printf("TIMER CONFIGURE\r\n");
        result = cyhal_timer_configure(&cont_timer, &cont_timer_cfg);

        if (result != CY_RSLT_SUCCESS)
        {
            CY_ASSERT(0);
            return false;
        } else {
            printf("TIMER SET FREQUENCY\r\n");

            result = cyhal_timer_set_frequency(&cont_timer, CONT_TIMER_FREQ);

            if (result != CY_RSLT_SUCCESS)
            {
                printf("TIMER SET FREQUENCY FAILED with: %lu\r\n", result);
                CY_ASSERT(0);
                return false;
            }
        }
    }
    printf("TIMER INITIALIZED\r\n");

    printf("\x1b[2J\x1b[;H");

    printf("******************\n\r");
    printf("* Robo Badgers CLI *\n\r");
    printf("******************\n\r");

	__enable_irq();

    // int commands_len = sizeof(COMMANDS) / sizeof(cli_command_t);
    for (int i = 0; i < COMMANDS_LEN; i++) {
        if (COMMANDS[i].enabled && COMMANDS[i].init != NULL) {
            if (!COMMANDS[i].init()) {
                return false;
            }
        }
    }

    return true;
}

void cli_run(cli_command_t* COMMANDS, int COMMANDS_LEN) {
    
    printf("Ready for commands:\n\r");

    int argc = 0;

    AVAILABLE_COMMANDS = COMMANDS;
    AVAILABLE_COMMANDS_LEN = COMMANDS_LEN;

    while (true) {
        if (using_ble) {
            ble_cli_process();
        }

        if (check_for_input()) {
            // Clear continuous command
            stop_continuous();

            printf("\n");// printf("\nReceived: %s - len: %i\n", pcInputString, strlen(pcInputString));

            // Process the received data
            char ** argv = cli_arg_splitter(get_input(), &argc, false);

            // for (int i = 0; i < argc; i++) {
            //     printf("Arg %i: %s - len: %i\n", i, argv[i], strlen(argv[i]));
            // }
            // printf("Args len: %i\n", argc);

            printf("\n");

            clear_input();
            
            // If no arguments, continue
            if (argc == 0) {
                continue;
            }

            // int commands_len = sizeof(COMMANDS) / sizeof(cli_command_t);
            if (!cli_command_handler(NULL, COMMANDS, COMMANDS_LEN, argv, argc)) {
                printf("Invalid command\n");
            }

            free_string_list(argv, argc);

            printf("\n");
        } else if (cont_argc > 0) {
            // for (int i = 0; i < cont_argc; i++) {
            //     printf("Cont Arg %i: %s - len: %i\n", i, cont_argv[i], strlen(cont_argv[i]));
            // }
            // printf("Cont Args len: %i\n", cont_argc);
            // printf("Cont Del: %lu\n", cont_del);
            if (cyhal_timer_read(&cont_timer) / CONT_TIMER_MS_DIV >= cont_del) {
                cyhal_timer_reset(&cont_timer);

                printf("\n");
                cli_command_handler(NULL, COMMANDS, COMMANDS_LEN, cont_argv, cont_argc);
                printf("\n");
            }
        }
    }
}

bool check_for_input() {
    if (using_ble) {
        return ALERT_BLE_RX;
    } else {
        return ALERT_CONSOLE_RX;
    }
}

char* get_input() {
    if (using_ble) {
        return bleInputString;
    } else {
        return pcInputString;
    }
}

void clear_input() {
    if (using_ble) {
        // Clear the flag
        ALERT_BLE_RX = false;
        // Reset the input string
        memset(bleInputString, 0, DEBUG_MESSAGE_MAX_LEN);
        bleInputIndex = 0;
    } else {
        // Clear the flag
        ALERT_CONSOLE_RX = false;
        // Reset the input string
        memset(pcInputString, 0, DEBUG_MESSAGE_MAX_LEN);
        cInputIndex = 0;
    }
}

void cli_continuous_handler(cli_command_t* parent, char** argv, int argc) {
    // NOTE: if you run cont stop it will run the stop command twice not once
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc >= 2) {
            int* del = (int*)cli_args_parser(argv[0], NULL, type_int);
            if (del == NULL) {
                printf("Invalid cont (continous) command - delay is not an integer\n");
                free(del);
                return;
            } else if (*del < CONT_MIN_DELAY) {
                printf("Invalid cont (continous) command - delay (%i) is less than %i\n", *del, CONT_MIN_DELAY);
                free(del);
                return;
            }

            if (strcmp(argv[1], parent->name) == 0) {
                printf("Invalid cont (continous) command - cannot run cont command in cont command\n");
                free(del);
                return;
            }

            bool subcommand = cli_command_handler(parent, AVAILABLE_COMMANDS, AVAILABLE_COMMANDS_LEN, argv+1, argc-1);

            if (!subcommand) {
                printf("Invalid cont (continous) command - Must have subcommand\r\n");
                return;
            }

            // Clear the global continuous variables
            stop_continuous();

            cont_del = *del;
            free(del);

            cont_argc = argc-1;

            cont_argv = malloc(cont_argc * sizeof(char*));
            for (int i = 0; i < cont_argc; i++) {
                cont_argv[i] = malloc(strlen(argv[i+1]) * sizeof(char));
                strcpy(cont_argv[i], argv[i+1]);
            }

            cyhal_timer_start(&cont_timer);
        } else {
            printf("Invalid cont (continous) command - Too Few Argument(s) (expected 2)\n");
        }
    }
}

void cli_stop_handler(cli_command_t* parent, char** argv, int argc) {
    // cont = 4 letters
    if (!cli_help_handler(parent, argv, argc)) {
        if (argc == 0) {
            stop_continuous();
        } else {
            printf("Invalid stop command - Extra Argument(s) (expected 0)\n");
        }
    }
}

void stop_continuous() {
    cont_del = 0;
    cyhal_timer_stop(&cont_timer);
    cyhal_timer_reset(&cont_timer);
    if (cont_argv != NULL) {
        free_string_list(cont_argv, cont_argc);
        cont_argv = NULL;
        cont_argc = 0;
    }
}

bool is_int(char* arg) {
    int length = strlen(arg);
    bool hasSign = arg[0] == '-' || arg[0] == '+';
    if (length == 1 && hasSign) {
        return false;
    }

    if (hasSign) {
        arg++;
        length--;
    }
    for (int i = 0; i < length; i++) {
        if (arg[i] < '0' || arg[i] > '9') {
            return false;
        }
    }
    return true;
}

bool is_float(char* arg) {
    int length = strlen(arg);
    bool hasSign = arg[0] == '-' || arg[0] == '+';
    if (length == 1 && hasSign) {
        return false;
    }

    if (hasSign) {
        arg++;
        length--;
    }
    
    bool has_dot = false;
    for (int i = 0; i < length; i++) {
        if (arg[i] == '.') {
            if (has_dot) {
                return false;
            }
            has_dot = true;
        } else if (arg[i] < '0' || arg[i] > '9') {
            return false;
        }
    }
    return true;
}

void free_string_list(char** list, int len) {
    for (int i = 0; i < len; i++) {
        free(list[i]);
    }
    free(list);
}