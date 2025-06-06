# Custom Shell Implementation

## Project Overview

This project is a custom shell implemented in C, designed to simulate basic shell functionalities and offer several advanced features like process management, I/O redirection, piping, job control, and a custom configuration file. The shell supports built-in commands and handles system signals such as `Ctrl-C`, `Ctrl-Z`, and `Ctrl-D` for process management and shell exit.

## Features

### 1. Prompt:
* Files: prompt.c and prompt.h
* Displays username, system name, and current directory
* Shows time taken by previous foreground processes that ran for more than 2 seconds

### 2. Input Processing:
* Files: input.c and input.h
* Parses input commands with maximum length of 4096 characters
* Supports multiple commands separated by `;` and background execution with `&`

### 3. Directory Navigation (`hop`):
* Files: hop.c and hop.h
* Custom implementation of the `cd` command
* Supports navigation with:
  * `~` (home directory)
  * `..` (parent directory)
  * `-` (previous directory)
* Handles up to 1000 arguments
* Executes commands until the first invalid argument
* If hop - is executed and no previous directory is set, the error "OLDPWD is not set" is printed

### 4. File Listing (`reveal`):
* Files: reveal.c and reveal.h
* Lists files and directories in lexicographic order
* Supports flags `-a` (show hidden files) and `-l` (detailed information)
* Color-coded output: blue for directories, green for executables, white for regular files
* Handles up to 4096 files in a directory
* If reveal - is executed and no previous directory is set, the error "OLDPWD is not set" is printed
* Only the name of the file/directory/symlink/etc is colored in "reveal -l"

### 5. Command History (`log`):
* Files: log.c and log.h
* Maintains history of commands across sessions
* Stores commands in log.txt for persistence
* Prevents duplicate consecutive entries
* Ignores inputs containing only spaces
* Commands:
  * `log`: Display command history
  * `log purge`: Clear history
  * `log execute <index>`: Execute a command from history

### 6. Process Management:
* Files: bgfg.c and bgfg.h
* Tracks foreground and background processes
* Reports time taken by foreground processes running longer than 2 seconds
* Uses a hashmap to store process IDs and names
* Displays completion messages for background processes
* Only considers the latest foreground process for time reporting

### 7. Process Information (`proclore`):
* Files: proclore.c and proclore.h
* Displays detailed information about processes:
  * PID
  * Process status (Running/Stopped)
  * Process group
  * Virtual memory usage (in bytes)
  * Executable path (when available)

### 8. File Search (`seek`):
* Files: seek.c and seek.h
* Searches for files and directories within a specified directory
* Supports flags:
  * `-d`: Search only directories
  * `-f`: Search only files
  * `-e`: Execute or open matching files/directories
* Color-coded output (green for files, blue for directories)
* Does not search in hidden folders

### 9. Custom Configuration:
* Files: alias.c and alias.h
* Reads configuration from `.myshrc` file
* Supports aliases and custom functions
* Aliases are stored in history instead of original commands

### 10. I/O Redirection:
* Files: redirection.c and redirection.h
* Supports:
  * `>`: Output redirection
  * `>>`: Append output
  * `<`: Input redirection
* Assumes redirection operators appear at the end of commands and parameters

### 11. Command Piping:
* Files: pipe.c, pipe.h, ifelse.c, ifelse.h
* Supports chaining commands with pipes (`|`)
* Handles multiple pipes
* Background execution with `&` at the end affects only the preceding command
* Built-in commands work properly with pipes (e.g., `hop .. | wc`)

### 12. Background Process Monitoring (`activities`):
* Files: activities.c and activities.h
* Lists all active background processes
* Shows full command, PID, and state (Running/Stopped)

### 13. Signal Handling and Process Control:
* Files: ping.c and ping.h
* Supports sending signals to processes with `ping <pid> <signal_number>`
* Handles:
  * Ctrl+C: Interrupts foreground process
  * Ctrl+Z: Stops foreground process and moves it to background
  * Ctrl+D: Exits the shell
* Does not print termination messages for background processes after Ctrl+D

### 14. Job Control:
* Files: bg.c and bg.h
* `bg <pid>`: Continues a stopped background process
* `fg <pid>`: Brings a background process to foreground
* When a background process is brought to foreground and runs for more than 2 seconds, its name is displayed in the next prompt

### 15. Latest Process ID Monitor (`neonate`):
* Files: neonate.c and neonate.h
* Periodically displays the most recently created process ID
* Command format: `neonate -n [time]` where time is the interval in seconds
* Can only be stopped by pressing 'x' (exits the shell)
* Ctrl+C interrupt does not affect the neonate process

### 16. Interactive Manual Pages (`iMan`):
* Files: iman.c and iman.h
* Fetches and displays manual pages from an online source

## Usage

### Built-in Commands

- **hop [directory]**: Changes the current directory to the specified path. Supports `~`, `..`, and `-`.
- **reveal [directory] [-a] [-l]**: Lists the contents of the specified directory with optional flags for hidden files and detailed view.
- **log**: Displays the command history.
  - `log purge`: Clears the command history.
  - `log execute <index>`: Executes the command at the specified index in the history.
- **proclore [pid]**: Displays detailed information about the specified process.
- **seek [target] [-d] [-f] [-e]**: Searches for directories, files, or executables based on the flags.
- **ping <pid> <signal_number>**: Sends a signal to the process with the specified PID.
- **fg <pid>**: Brings the specified background process to the foreground.
- **bg <pid>**: Continues a stopped background process.
- **activities**: Lists all background processes started by the shell.
- **neonate -n [time]**: Displays the latest process ID every [time] seconds. Press 'x' to stop and log out of the shell.
- **iMan <command>**: Fetches and displays manual pages for the specified command.

### I/O Redirection

- Use `>` to redirect output to a file.
- Use `>>` to append output to a file.
- Use `<` to read input from a file.

### Piping

- Chain commands using `|`. The output of one command becomes the input for the next.

### Signals

- **Ctrl-C**: Interrupt the current foreground process.
- **Ctrl-D**: Exit the shell with the message "Logging Out...".
- **Ctrl-Z**: Stop the foreground process and move it to the background.

## Building and Running

The project includes a Makefile with the following commands:

- **Build the shell**:
  ```bash
  make
  ```

- **Run the shell**:
  ```bash
  ./a.out
  ```

- **Clean build files**:
  ```bash
  make clean
  ```






