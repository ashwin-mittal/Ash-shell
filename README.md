# ash-shell
> A quick overview of the ash-shell.

## Background
 - [x] The tilde `~` represents the home directory of the shell or the directory from which the shell is invoked.
 - [x] Support for semicolon-separated commands, not affected by whitespace or tabs.
 - [x] Stores `MAX_HISTORY_SIZE` most recent commands across all sessions permanently in a file.
 - [x] Execution of system commands with or without arguments, in background or foreground.
 - [x] Notifying when and how the background process terminated automatically with signum or status.
 - [x] Error handling is done for the corresponding features in their files.
 - [x] Input and output redirection, including appending to a file.
 - [x] Piping of commands with I/O redirection support for internal and external commands.
 - [x] cd built-in supports `-`, that is, last working directory.
 - [x] Exit code support is provided for both internal and external commands.
 - [x] Supports command chaining with logical AND and OR.
 - [x] Handling `SIGINT` [C-c] and `SIGTSTP` [C-z] signals is done.
 - [x] To exit the ash-shell, press `<Ctrl>` + `d`.
 
## Shell built-ins or internal commands
   - [x] `cd`
   - [x] `pwd`
   - [x] `echo`
   - [x] `exit`
   - [x] `history`
   - [x] `pinfo`
   - [x] `nightswatch`
   - [x] `ls -[al] [directories..]`
   - [x] `setenv`
   - [x] `unsetenv`
   - [x] `jobs`
   - [x] `kjob`
   - [x] `fg`
   - [x] `bg`
   - [x] `overkill`
   - [x] `quit`
   - [x] `exit`

## Running
To run:
```
:: make
:: ./ash
```
Shell prompt:
* On successful exit:
```
:')<username@system_name:curr_dir>::
```
* In case of errors:
```
:'(<username@system_name:curr_dir>::
```
To exit the ash-shell:
```
:: quit
```
```
:: exit
```
To delete the object and executable files:
```
:: make clean
```
## File structure
 - `background.c`: Contains code for background execution of a process and to check whether it is exited or not.
 - `cd.c`: Contains code for the `cd` built-in.
 - `echo.c`: Contains code for the `echo` built-in.
 - `history.c`: Contains code for the `history` built-in.
 - `list.c`: Contains code for the `ls` built-in.
 - `options.c`: Contains code which checks options for the `ls` built-in. 
 - `pwd.c`: Contains code for the `pwd` built-in.
 - `pinfo.c`: Contains code for the `pinfo` built-in.
 - `nightswatch.c`: Contains code for the `nightswatch` built-in.
 - `parse.c`: Contains code which reads and parses a command.
 - `execute.c`: Contains code which executes a command.
 - `prompt.c`: Contains code which displays the shell prompt.
 - `main.c`: Contains the main function and the ash-shell REPL.
 - `environment.c`: Contains code for the `setenv` and `unsetenv` built-in.
 - `foreground.c`: Contains code for the `fg` built-in.
 - `jobs.c`: Contains code for the `jobs` built-in.
 - `kjob.c`: Contains code for the `kjob` built-in.
 - `chaining.c` - Contains code for the command chaining.
 - `overkill.c`: Contains code for the `overkill` built-in.
 - `piping.c`: Contains code for the implementation of command pipelines.
 - `redirect.c`: Contains code for the implementation of I/O redirection.
 - `shell.h`: Contains macros and function declarations shared between the files given above.
 - `headers.h`: Common headers used in the whole code, specific are included in the corresponding file. 