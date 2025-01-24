
#ifndef UTIL_H
#define UTIL_H

char **split_str(char *str, char del);
void handle_cmd(char **cmd); 
void free_tokens(char **tokens); 

#endif
