#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "unlikely.h"
#include "cli/cli.h"
#include "cli/types.h"

static void default_error_handler(cli_parse_status_t status, const char* details) {
    const char* msg = "No details";

    switch (status) {
        case CLI_PARSE_OK: return;
        case CLI_PARSE_ERR_UNKNOWN_CMD: msg = "Unknown command"; break;
        case CLI_PARSE_ERR_INVALID_TYPE: msg = "Invalid argument type"; break;
        case CLI_PARSE_ERR_MISSING_ARG:  msg = "Missing required arguments"; break;
        case CLI_PARSE_ERR_UNKNOWN_FLAG: msg = "Unknown flag"; break;
        case CLI_PARSE_ERR_NULL_APP: msg = "Uninitialized app"; break;
        case CLI_PARSE_ERR_OOM: msg = "Out of memory"; break;
        case CLI_PARSE_ERR_INCOMPLETE_ESCAPE: msg = "Incomplete escape sequence"; break;
        case CLI_PARSE_ERR_UNBALANCED_QUOTES: msg = "Unbalanced quotes"; break;
    }

    fprintf(stderr, "%s %s\n", msg, (details ? details : ""));
}

void cli_app_run(cli_app_t* cli_app) {
    if (unlikely(!cli_app->error_handler)) cli_app->error_handler = default_error_handler;
    cli_app->running = true;
    while (cli_app->running) {
        char* input = readline("> ");
        if (!input) break;
        if (*input) {
            add_history(input);
            cli_parse_status_t status = cli_parse_line(cli_app, input);
            if (status != CLI_PARSE_OK) {
                cli_app->error_handler(status, NULL);
            }
        }
        free(input);
    }
}

cli_app_t* cli_app_init(void* ctx, const cli_cmd_t* cmds, size_t cmds_count, cli_error_handler_t error_handler) {
    cli_app_t* cli_app = malloc(sizeof(cli_app_t));
    if (unlikely(!cli_app)) return NULL;
    cli_app->cmds = cmds;
    cli_app->cmds_count = cmds_count;
    cli_app->running = false;

    cli_app->error_handler = error_handler ? error_handler : default_error_handler;

    cli_app->context = ctx;
    return cli_app;
}

void cli_app_destroy(cli_app_t* cli_app) {
    if (unlikely(!cli_app)) return;
    clear_history();
    rl_cleanup_after_signal();
    rl_free_line_state();
    free(cli_app);
}