#include <cli/cli.h>
#include <stdio.h>

#include <cli/types.h>

void handle_sum(cli_app_t* app, cli_result_t* res) {
    double a = res->pos_args[0].as.f64;
    double b = res->pos_args[1].as.f64;
    printf("Sum is: %lf\n", a + b);
}

void handle_exit(cli_app_t* app, cli_result_t* res) {
    printf("Goodbye\n");
    app->running = false;
}

void handle_help(cli_app_t* app, cli_result_t* res) {
    printf("Example help command\n");
    for (size_t i = 0; i < app->cmds_count; i++) {
        cli_cmd_t cmd = app->cmds[i];
        printf("%s: \t%s", cmd.name, cmd.help);
        if (cmd.args_count != 0) {
            printf(". Usage: %s", cmd.name);
            for (size_t j = 0; j < cmd.args_count; j++) {
                char* arg_type;
                arg_type_t arg = cmd.args_spec[j];
                switch (arg) {
                    case CLI_TYPE_DOUBLE: arg_type = "double"; break;
                    case CLI_TYPE_INT: arg_type = "int"; break;
                    case CLI_TYPE_STRING: arg_type = "string"; break;
                    case CLI_TYPE_NONE: arg_type = "none(fot flags only)"; break;
                    default: arg_type = "unknown"; break;
                }
                printf(" <%s>", arg_type);
            }
        }
        printf("\n");
    }

}

static arg_type_t sum_args[] = {CLI_TYPE_DOUBLE, CLI_TYPE_DOUBLE};

static cli_cmd_t my_cmds[] = {
    {
        .name = "sum",
        .help = "Calculates sum of two numbers",
        .handler = handle_sum,
        .args_spec = sum_args,
        .args_count = 2
    },
    {
        .name = "exit",
        .help = "Exit the program",
        .handler = handle_exit,
        .args_spec = NULL,
        .args_count = 0
    },
    {
        .name = "help",
        .help = "Show this help",
        .handler = handle_help,
        .args_spec = NULL,
        .args_count = 0
    }
};

int main() {
    cli_app_t* app = cli_app_init(NULL, my_cmds, 3, NULL);

    printf("Welcome to Calc CLI! Type 'sum 10 20' to test.\n");
    cli_app_run(app);

    cli_app_destroy(app);
    return 0;
}