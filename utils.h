#ifndef UTILS_H
#define UTILS_H

// Function declarations
void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_parse_line(char *line);
int lsh_execute(char **args);
int lsh_launch(char **args);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

#endif