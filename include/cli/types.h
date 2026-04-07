#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    CLI_TYPE_NONE,
    CLI_TYPE_INT,
    CLI_TYPE_DOUBLE,
    CLI_TYPE_STRING,
} arg_type_t;

typedef struct {
    arg_type_t type;
    union {
        int i32;
        double f64;
        const char *s;
    } as;
    bool occurred;
} cli_value_t;

typedef struct {
    char short_nm;
    const char* long_nm;
    arg_type_t type;
    const char* help;
} cli_flag_spec_t;

typedef struct {
    cli_value_t* pos_args;
    cli_value_t* flags;
} cli_result_t;

typedef struct cli_app cli_app_t;

typedef void (*cli_handler_t)(cli_app_t* app, cli_result_t* result);

typedef struct cli_cmd_t {
    const char* name;
    const char* help;
    arg_type_t* args_spec;
    size_t args_count;
    cli_flag_spec_t* flags_spec;
    size_t flags_count;

    cli_handler_t handler;
} cli_cmd_t;

typedef enum {
    CLI_PARSE_OK,
    CLI_PARSE_ERR_OOM,
    CLI_PARSE_ERR_INVALID_TYPE,
    CLI_PARSE_ERR_UNKNOWN_FLAG,
    CLI_PARSE_ERR_UNKNOWN_CMD,
    CLI_PARSE_ERR_MISSING_ARG,
    CLI_PARSE_ERR_NULL_APP,
    CLI_PARSE_ERR_INCOMPLETE_ESCAPE,
    CLI_PARSE_ERR_UNBALANCED_QUOTES
} cli_parse_status_t;

typedef void (*cli_error_handler_t)(cli_parse_status_t status, const char* details);

struct cli_app {
    void* context;
    const cli_cmd_t* cmds;
    size_t cmds_count;
    cli_error_handler_t error_handler;
    bool running;
};

#endif //TYPES_H
