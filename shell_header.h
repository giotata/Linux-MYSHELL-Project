int parse(char *line, char **envp, char abs[]);
int builtIns(char **cmd, int length, char **envp, char abs[]);
int runExternal(char **cmd, int len);
