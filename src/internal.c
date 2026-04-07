#include "internal.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int _cli_find_flag_idx(cli_cmd_t *cmd, const char *token) {
    bool is_long = (token[1] == '-');
    const char *name = is_long ? &token[2] : &token[1];

    for (size_t i = 0; i < cmd->flags_count; i++) {
        if (is_long) {
            if (cmd->flags_spec[i].long_nm && strcmp(cmd->flags_spec[i].long_nm, name) == 0) return (int) i;
        } else {
            if (cmd->flags_spec[i].short_nm == name[0] && name[1] == '\0') return (int) i;
        }
    }
    return -1;
}

bool _cli_convert_arg(const char *token, arg_type_t type, cli_value_t *out) {
    if (!token) return false;
    char *endptr;
    out->type = type;
    errno = 0;

    switch (type) {
        case CLI_TYPE_INT: {
            long val = strtol(token, &endptr, 10);
            if (token == endptr || *endptr != '\0' || errno == ERANGE) return false;
            out->as.i32 = (int) val;
            return true;
        }
        case CLI_TYPE_DOUBLE: {
            double val = strtod(token, &endptr);

            if (token == endptr || *endptr != '\0' || errno == ERANGE) return false;
            out->as.f64 = val;
            return true;
        }
        case CLI_TYPE_STRING:
            out->as.s = token;
        return true;
        case CLI_TYPE_NONE:
            return true;
        default: return false;
    }
}