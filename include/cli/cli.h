#ifndef CLI_H
#define CLI_H

#include "types.h"

cli_parse_status_t cli_parse_line(cli_app_t* app, char* line);

cli_app_t* cli_app_init(void* ctx, const cli_cmd_t* cmds, size_t cmds_count, cli_error_handler_t error_handler);
void cli_app_run(cli_app_t* app);
void cli_app_destroy(cli_app_t* cli_app);


#endif //CLI_H
