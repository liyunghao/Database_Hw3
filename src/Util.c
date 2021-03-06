#include <stdio.h>
<<<<<<< HEAD
#include <stdio_ext.h>
=======
//#include <stdio_ext.h>
>>>>>>> 1107c21686f5228f2543b829eb534a1ab4ca81c5
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Util.h"
#include "Command.h"
#include "Table.h"
#include "SelectState.h"

///
/// Allocate State_t and initialize some attributes
/// Return: ptr of new State_t
///
State_t* new_State() {
    State_t *state = (State_t*)malloc(sizeof(State_t));
    state->saved_stdout = -1;
    return state;
}

///
/// Print shell prompt
///
void print_prompt(State_t *state) {
    if (state->saved_stdout == -1) {
        printf("db > ");
    }
}

///
/// Print the user in the specific format
///
void print_user(User_t *user, SelectArgs_t *sel_args) {
    if(sel_args->fields_len == 0) return;
    size_t idx;
    printf("(");
    for (idx = 0; idx < sel_args->fields_len; idx++) {
        if (!strncmp(sel_args->fields[idx], "*", 1)) {
            printf("%d, %s, %s, %d", user->id, user->name, user->email, user->age);
        } else {
            if (idx > 0) printf(", ");

            if (!strncmp(sel_args->fields[idx], "id", 2)) {
                printf("%d", user->id);
            } else if (!strncmp(sel_args->fields[idx], "name", 4)) {
                printf("%s", user->name);
            } else if (!strncmp(sel_args->fields[idx], "email", 5)) {
                printf("%s", user->email);
            } else if (!strncmp(sel_args->fields[idx], "age", 3)) {
                printf("%d", user->age);
            }
        }
    }
    printf(")\n");
}
void print_aggre(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd) {
    if (cmd->aggre_args.fields_len == 0) return;
    size_t idx;
<<<<<<< HEAD
    int limit = cmd->cmd_args.sel_args.limit;
    int offset = cmd->cmd_args.sel_args.offset;
=======
    int limit = cmd->sel_args.limit;
    int offset = cmd->sel_args.offset;
>>>>>>> 1107c21686f5228f2543b829eb534a1ab4ca81c5

    if (offset == -1) {
        offset = 0;
    }
    printf("(");
    for (idx = offset; idx < cmd->aggre_args.fields_len; idx++) {
        if (limit != -1 && (idx - offset) >= limit) {
            break;
        }
        if (idx > offset) printf(", ");
        if (!strncmp(cmd->aggre_args.type[idx], "count", 5)) {
            printf("%zu", idxListLen);
        } else if (!strncmp(cmd->aggre_args.type[idx], "avg", 3)) {
            double sum = 0;
            for (int i = 0; i < idxListLen; i++) {
                User_t *user = get_User(table, idxList[i]);
                if (!strncmp(cmd->aggre_args.fields[idx], "id", 2)) {
                    sum += user->id;
                } else if (!strncmp(cmd->aggre_args.fields[idx], "age", 3)) {
                    sum += user->age;
                }
            }
            printf("%.3lf", sum / idxListLen);
        } else if (!strncmp(cmd->aggre_args.type[idx], "sum", 3)) {
            int sum = 0;
            for (int i = 0; i < idxListLen; i++) {
                User_t *user = get_User(table, idxList[i]);
                if (!strncmp(cmd->aggre_args.fields[idx], "id", 2)) {
                    sum += user->id;
                } else if (!strncmp(cmd->aggre_args.fields[idx], "age", 3)) {
                    sum += user->age;
                }
            }
            printf("%d", sum);
        }
    }
    printf(")\n");
}

///
/// Print the users for given offset and limit restriction
///
void print_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd) {
    size_t idx;
    int limit = cmd->sel_args.limit;
    int offset = cmd->sel_args.offset;

    if (offset == -1) {
        offset = 0;
    }
    for (idx = offset; idx < idxListLen; idx++) {
        if (limit != -1 && (idx - offset) >= limit) {
            break;
        }
<<<<<<< HEAD
        print_user(get_User(table, idxList[idx]), &(cmd->cmd_args.sel_args));
=======
        print_user(get_User(table, idxList[idx]), &(cmd->sel_args));
>>>>>>> 1107c21686f5228f2543b829eb534a1ab4ca81c5
    }
}

///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " ,\n");
    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " ,\n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
Pair_t where_users(Table_t *table, Command_t *cmd) {
    WhereArgs_t wArgs = cmd->whe_args;
    int fields_len = wArgs.fields_len;
    int *idxList = NULL;
    int listLen = 0;
    Pair_t p;
    if (fields_len >= 1) {
        for (int i = 0, j, cnt; i < table->len; i++) {
            User_t *user = get_User(table, i);
            cnt = 0;
            for (j = 0; j < fields_len; j++) {
                if (!strncmp(wArgs.conditions[j], "\"", 1)) {
                    if (!strncmp(wArgs.fields[j], "name", 4)) {
                        if (!strncmp(wArgs.operators[j], "=", 1)) {
                            if(!strncmp(user->name, wArgs.conditions[j], strlen(wArgs.conditions[j]))) cnt++;
                        }else if(!strncmp(wArgs.operators[j], "!=", 2)) {
                            if(strncmp(user->name, wArgs.conditions[j], strlen(wArgs.conditions[j]))) cnt++;
                        }
                    } else if (!strncmp(wArgs.fields[j], "email", 5)) {
                        if (!strncmp(wArgs.operators[j], "=", 1)) {
                            if(!strncmp(user->email, wArgs.conditions[j], strlen(wArgs.conditions[j]))) cnt++;
                        }else if(!strncmp(wArgs.operators[j], "!=", 2)) {
                            if(strncmp(user->email, wArgs.conditions[j], strlen(wArgs.conditions[j]))) cnt++;
                        }
                    }
                } else {
                    double num = atof(wArgs.conditions[j]);
                    if (!strncmp(wArgs.fields[j], "id", 2)) {
                        if(!strncmp(wArgs.operators[j], "=", 1)) {
                            if(user->id == num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], "!=", 2)) {
                            if(user->id != num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], ">=", 2)) {
                            if(user->id >= num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], "<=", 2)) {
                            if(user->id <= num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], ">", 1)) {
                            if(user->id > num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], "<", 1)) {
                            if(user->id < num) cnt++;
                        } 
                    } else if (!strncmp(wArgs.fields[j], "age", 3)) {
                        if(!strncmp(wArgs.operators[j], "=", 1)) {
                            if(user->age == num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], "!=", 2)) {
                            if(user->age != num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], ">=", 2)) {
                            if( user->age >= num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], "<=", 2)) {
                            if(user->age <= num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], ">", 1)) {
                            if(user->age > num) cnt++;
                        } else if (!strncmp(wArgs.operators[j], "<", 1)) {
                            if(user->age < num) cnt++;
                        } 
                    }
                }
            }
            if (wArgs.A == 1) {
                if (cnt == fields_len) {
                    int *buf = (int *)malloc(sizeof(int)*(listLen+1));
                    if (idxList) {
                        memcpy(buf, idxList, sizeof(int)*listLen);
                        free(idxList);
                    }
                    idxList = buf;
                    idxList[listLen] = i;
                    listLen++;
                }
            } else {
                if (cnt >= 1) {
                    int *buf = (int *)malloc(sizeof(int)*(listLen+1));
                    if (idxList) {
                        memcpy(buf, idxList, sizeof(int)*listLen);
                        free(idxList);
                    }
                    idxList = buf;
                    idxList[listLen] = i;
                    listLen++;
                }
            }
        }
    } else {
        int *buf = (int *)malloc(sizeof(int)*table->len);
        listLen = table->len;
        idxList = buf;
        for (int i = 0; i < table->len; i++) {
            idxList[i] = i;
        }
    }
    p.idxList = idxList;
    p.listLen = listLen;

    return p;
    
}
void updater(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd) {
    UpdateArgs_t uArgs = cmd->up_args;
    for (int i = 0; i < idxListLen; i++) {
        User_t *user = get_User(table, idxList[i]);
        if(!strncmp(uArgs.fields, "name", 4)) {
            strcpy(user->name, uArgs.dest);
        } else if (!strncmp(uArgs.fields, "email", 5)) {
            strcpy(user->email, uArgs.dest);
        } else if (!strncmp(uArgs.fields, "id", 2)) {
            user->id = atoi(uArgs.dest);
        } else if (!strncmp(uArgs.fields, "age", 3)) {
            user->age = atoi(uArgs.dest);
        }
    }
    return;
}
void deleter(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd) {
    Table_t *newTable = new_Table(NULL);
    for (int i = 0, cnt = 0; i < table->len; i++) {
        if (cnt == idxListLen || i != idxList[cnt]) {
            add_User(newTable, get_User(table, i));
        } else {
            cnt++;
        }
    }
    table->len = newTable->len;
    for (int i = 0; i < table->len; i++) {
        table->users[i].id = newTable->users[i].id;
        memcpy(table->users[i].name, newTable->users[i].name, sizeof(newTable->users[i].name));
        memcpy(table->users[i].email, newTable->users[i].email, sizeof(newTable->users[i].email));
        table->users[i].age = newTable->users[i].age;
    }
}
void handle_builtin_cmd(Table_t *table, Command_t *cmd, State_t *state) {
    if (!strncmp(cmd->args[0], ".exit", 5)) {
        archive_table(table);
        exit(0);
    } else if (!strncmp(cmd->args[0], ".output", 7)) {
        if (cmd->args_len == 2) {
            if (!strncmp(cmd->args[1], "stdout", 6)) {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1], 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
<<<<<<< HEAD
                 __fpurge(stdout); //This is used to clear the stdout buffer
=======
//                 __fpurge(stdout); //This is used to clear the stdout buffer
>>>>>>> 1107c21686f5228f2543b829eb534a1ab4ca81c5
            }
        }
    } else if (!strncmp(cmd->args[0], ".load", 5)) {
        if (cmd->args_len == 2) {
            load_table(table, cmd->args[1]);
        }
    } else if (!strncmp(cmd->args[0], ".help", 5)) {
        print_help_msg();
    }
}

///
/// Handle query type commands
/// Return: command type
///
int handle_query_cmd(Table_t *table, Command_t *cmd) {
    if (!strncmp(cmd->args[0], "insert", 6)) {
        handle_insert_cmd(table, cmd);
        return INSERT_CMD;
    } else if (!strncmp(cmd->args[0], "select", 6)) {
        handle_select_cmd(table, cmd);
        return SELECT_CMD;
    } else if (!strncmp(cmd->args[0], "update", 6)) {
        handle_update_cmd(table, cmd);
        return UPDATE_CMD;
    } else if (!strncmp(cmd->args[0], "delete", 6)) {
        handle_delete_cmd(table, cmd);
        return DELETE_CMD;
    } else {
        return UNRECOG_CMD;
    }
}
int handle_update_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = UPDATE_CMD;
    update_state_handler(cmd, 1);
    Pair_t p = where_users(table, cmd);
    updater(table, p.idxList, p.listLen, cmd);
    return table->len;
}

int handle_delete_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = DELETE_CMD;
    delete_state_handler(cmd, 1);
    Pair_t p = where_users(table, cmd);
    deleter(table, p.idxList, p.listLen, cmd);
    return table->len;
}

int handle_insert_cmd(Table_t *table, Command_t *cmd) {
    int ret = 0;
    User_t *user = command_to_User(cmd);
    if (user) {
        ret = add_User(table, user);
        if (ret > 0) {
            cmd->type = INSERT_CMD;
        }
    }
    return ret;
}

int handle_select_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = SELECT_CMD;
    field_state_handler(cmd, 1);
    Pair_t p = where_users(table, cmd);
    print_users(table, p.idxList, p.listLen, cmd);
    print_aggre(table, p.idxList, p.listLen, cmd);
    return table->len;
}
///
/// Show the help messages
///
void print_help_msg() {
    const char msg[] = "# Supported Commands\n"
    "\n"
    "## Built-in Commands\n"
    "\n"
    "  * .exit\n"
    "\tThis cmd archives the table, if the db file is specified, then exit.\n"
    "\n"
    "  * .output\n"
    "\tThis cmd change the output strategy, default is stdout.\n"
    "\n"
    "\tUsage:\n"
    "\t    .output (<file>|stdout)\n\n"
    "\tThe results will be redirected to <file> if specified, otherwise they will display to stdout.\n"
    "\n"
    "  * .load\n"
    "\tThis command loads records stored in <DB file>.\n"
    "\n"
    "\t*** Warning: This command will overwrite the records already stored in current table. ***\n"
    "\n"
    "\tUsage:\n"
    "\t    .load <DB file>\n\n"
    "\n"
    "  * .help\n"
    "\tThis cmd displays the help messages.\n"
    "\n"
    "## Query Commands\n"
    "\n"
    "  * insert\n"
    "\tThis cmd inserts one user record into table.\n"
    "\n"
    "\tUsage:\n"
    "\t    insert <id> <name> <email> <age>\n"
    "\n"
    "\t** Notice: The <name> & <email> are string without any whitespace character, and maximum length of them is 255. **\n"
    "\n"
    "  * select\n"
    "\tThis cmd will display all user records in the table.\n"
    "\n";
    printf("%s", msg);
}


