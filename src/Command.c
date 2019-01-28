#include <stdlib.h>
#include <string.h>
#include "Command.h"

Command_t* new_Command() {
    Command_t *cmd = (Command_t*) malloc(sizeof(Command_t));
    cmd->type = UNRECOG_CMD;
    cmd->args = NULL;
    cmd->args_len = 0;
    cmd->args_cap = 0;
    return cmd;
}

int add_Arg(Command_t *cmd, char *arg) {
    char **new_buf;
    if (cmd->args == NULL) {
        new_buf = (char **) malloc(sizeof(char*) * 5);
        if (new_buf == NULL)
            goto error;

        cmd->args = new_buf;
        cmd->args_cap = 5;
        memset((void*)cmd->args, 0, sizeof(char*) * 5);
    } else if (cmd->args_cap == cmd->args_len) {
        new_buf = (char **) malloc(
                                sizeof(char*) * (cmd->args_cap + 5));
        if (new_buf == NULL)
            goto error;

        memset((void*)new_buf, 0,
            sizeof(char*) * (cmd->args_cap+5));
        memcpy((void*)new_buf, (void *)cmd->args,
            sizeof(char*) * (cmd->args_cap));
        free(cmd->args);
        cmd->args = new_buf;
        cmd->args_cap += 5;
    }
    cmd->args[cmd->args_len] = strdup(arg);
    cmd->args_len++;
    return 0;

error:
    return 1;
}

/*
 * TODO: This func will release the allocated args memories
 */
void cleanup_Command(Command_t *cmd) {
    int idx;
    for (idx = 0; idx < cmd->args_cap; idx++) {
        free(cmd->args[idx]);
        cmd->args[idx] = NULL;
    }
    cmd->type = UNRECOG_CMD;
    cmd->args_len = 0;
}