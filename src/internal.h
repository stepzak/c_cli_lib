#ifndef INTERNAL_H
#define INTERNAL_H
#include "cli/types.h"

cli_parse_status_t _cli_next_token(char** line_ptr, char** token_out);
bool _cli_convert_arg(const char* token, arg_type_t type, cli_value_t* out);
int _cli_find_flag_idx(cli_cmd_t* cmd, const char* token);
void _cli_print_error(cli_parse_status_t status);

#endif //INTERNAL_H
