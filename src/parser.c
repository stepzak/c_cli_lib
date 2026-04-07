#include <stdio.h>

#include "cli/types.h"
#include "cli/cli.h"

#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "unlikely.h"

cli_parse_status_t _cli_next_token(char **line_ptr, char **token_out) {
    char *src = *line_ptr;
    if (!src || *src == '\0') { *token_out = NULL; return CLI_PARSE_OK; }

    while (*src && (*src == ' ' || *src == '\t')) src++;
    if (*src == '\0') {
        *line_ptr = src;
        return CLI_PARSE_OK;
    }

    char *token_start = src;
    char *dest = src;
    bool in_quotes = false;
    bool escaped = false;

    while (*src) {
        if (escaped) {
            *dest++ = *src++;
            escaped = false;
            continue;
        }

        if (*src == '\\') {
            escaped = true;
            src++;
            continue;
        }

        if (*src == '\"') {
            in_quotes = !in_quotes;
            src++;
            continue;
        }

        if (!in_quotes && (*src == ' ' || *src == '\t' || *src == '\n')) {
            break;
        }

        *dest++ = *src++;
    }

    if (in_quotes) return CLI_PARSE_ERR_UNBALANCED_QUOTES;
    if (escaped) return CLI_PARSE_ERR_INCOMPLETE_ESCAPE;

    if (*src != '\0') {
        *src = '\0';
        *line_ptr = src + 1;
    } else {
        *line_ptr = src;
    }

    *dest = '\0';
    *token_out = token_start;
    return CLI_PARSE_OK;
}

cli_parse_status_t cli_parse_line(cli_app_t *app, char *line) {
    if (unlikely(!app)) return CLI_PARSE_ERR_NULL_APP;
    if (!line || *line == '\0') return CLI_PARSE_OK;

    char *current_pos = line;
    char *token = NULL;

    cli_parse_status_t status = _cli_next_token(&current_pos, &token);
    if (status != CLI_PARSE_OK) return status;
    if (!token) return CLI_PARSE_OK;

    cli_cmd_t *cmd = NULL;
    for (size_t i = 0; i < app->cmds_count; i++) {
        if (strcmp(token, app->cmds[i].name) == 0) {
            cmd = &app->cmds[i];
            break;
        }
    }
    if (!cmd) return CLI_PARSE_ERR_UNKNOWN_CMD;

    cli_value_t pos_args[cmd->args_count > 0 ? cmd->args_count : 1];
    cli_value_t flag_vals[cmd->flags_count > 0 ? cmd->flags_count : 1];

    for (size_t i = 0; i < cmd->args_count; i++) pos_args[i].occurred = false;
    for (size_t i = 0; i < cmd->flags_count; i++) flag_vals[i].occurred = false;

    size_t current_pos_idx = 0;

    while (true) {
        status = _cli_next_token(&current_pos, &token);
        if (status != CLI_PARSE_OK) return status;
        if (!token) break;

        if (token[0] == '-') {
            int f_idx = _cli_find_flag_idx(cmd, token);
            if (f_idx == -1) return CLI_PARSE_ERR_UNKNOWN_FLAG;

            flag_vals[f_idx].occurred = true;

            if (cmd->flags_spec[f_idx].type != CLI_TYPE_NONE) {
                char *val_token;
                status = _cli_next_token(&current_pos, &val_token);
                if (status != CLI_PARSE_OK) return status;
                if (!val_token) return CLI_PARSE_ERR_MISSING_ARG;

                if (!_cli_convert_arg(val_token, cmd->flags_spec[f_idx].type, &flag_vals[f_idx]))
                    return CLI_PARSE_ERR_INVALID_TYPE;
            }
        } else {

            if (current_pos_idx >= cmd->args_count) {
                return CLI_PARSE_ERR_INVALID_TYPE;
            }
            cli_parse_status_t convert_status = _cli_convert_arg(token, cmd->args_spec[current_pos_idx], &pos_args[current_pos_idx]);
            if (!convert_status) return CLI_PARSE_ERR_INVALID_TYPE;

            pos_args[current_pos_idx].occurred = true;
            current_pos_idx++;
        }
    }
    if (current_pos_idx < cmd->args_count) {
        return CLI_PARSE_ERR_MISSING_ARG;
    }

    cli_result_t result = {.pos_args = pos_args, .flags = flag_vals};
    cmd->handler(app, &result);
    return CLI_PARSE_OK;
}
