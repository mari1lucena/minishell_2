*This project has been created as part of the 42 curriculum by made-jes, mlucena-.*

# Minishell

## Description

Minishell is a simplified but functional Unix shell that replicates the core behavior of bash. This project provides hands-on experience with fundamental systems programming concepts including process management, file descriptors, signal handling, and command-line parsing.

### What is a Shell?

A shell is a command-line interpreter that provides a user interface for Unix-like operating systems. It takes commands from the user, interprets them, and executes the corresponding programs. Our minishell aims to recreate this functionality with a focus on learning the underlying mechanisms.

---

## ✨ Features

### Built-in Commands
- `echo` with option `-n`
- `cd` with relative or absolute path
- `pwd` (with no options)
- `export` (with no options)
- `unset` (with no options)
- `env` (with no options or arguments)
- `exit` (with no options)

### Shell Features
- **Prompt display** - Shows a prompt while waiting for user input
- **Command history** - Navigate through previously entered commands using arrow keys
- **Command execution** - Search and launch executables based on PATH variable or using relative/absolute paths
- **Quote handling**
  - Single quotes (`'`) prevent interpretation of metacharacters
  - Double quotes (`"`) prevent interpretation except for `$` (dollar sign)
- **Redirections**
  - `<` - Input redirection
  - `>` - Output redirection (truncate)
  - `>>` - Output redirection (append)
  - `<<` - Here-document (heredoc)
- **Pipes** (`|`) - Connect output of one command to input of another
- **Environment variables** - Expand `$VARIABLE` to their values
- **Exit status** - Access last command's exit status with `$?`
- **Signal handling**
  - `ctrl-C` - Display new prompt on a new line
  - `ctrl-D` - Exit the shell
  - `ctrl-\` - Do nothing (ignored)

---

## Instructions

```bash
# Clone the repository
git clone [your-repository-url]
cd minishell

# Compile the project
make

# Run minishell
./minishell
```

## 🔧 Technical Implementation

### Architecture Overview

1. **Lexer** - Breaks input into tokens (words, operators, quotes)
2. **Expander** - Handles variable expansion and quote removal
3. **Parser** - Builds an Abstract Syntax Tree (AST) from tokens
4. **Executor** - Traverses the AST and executes commands

### Key Concepts Learned

- **Process Management**
  - Creating child processes with `fork()`
  - Executing programs with `execve()`
  - Waiting for processes with `waitpid()`

- **File Descriptors**
  - Duplicating file descriptors with `dup2()`
  - Opening/closing files
  - Pipe creation and management

- **Signal Handling**
  - Setting up custom signal handlers
  - Handling `SIGINT`, `SIGQUIT` appropriately in different contexts

- **Memory Management**
  - Proper allocation and deallocation
  - Avoiding memory leaks (tested with Valgrind)

---

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)

### 42 Community Resources
- [42 Minishell Guide](https://42-cursus.gitbook.io/guide)
- [Minishell.org](https://minishell.org/)

### System Programming
- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/)
- [Linux man pages](https://man7.org/linux/man-pages/)
