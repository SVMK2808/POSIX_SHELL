# Simple C++ Shell with Signal Handling and I/O Redirection

This is a basic C++ shell implementation that demonstrates:

- **Command execution:** Forks a child process to execute user commands.
- **I/O redirection:** Handles basic input ( < ) and output ( > ) redirection.
- **Signal handling:** Handles SIGINT (CTRL-C), SIGTSTP (CTRL-Z), and SIGQUIT (CTRL-D) signals.

**Features:**

- **Command execution:** Supports simple commands like `ls`, `pwd`, `cd`, `echo`.
- **I/O redirection:**
  - Supports basic input redirection (`<`), e.g., `cat < input.txt`.
  - Supports basic output redirection (`>`, `>>`), e.g., `ls -l > output.txt`, `echo "hello" >> output.txt`.
- **Signal handling:**
  - **CTRL-C (SIGINT):** Interrupts the foreground process.
  - **CTRL-Z (SIGTSTP):** Stops the foreground process.
  - **CTRL-D (SIGQUIT):** Exits the shell.
- **AutoComplete:**
  - Autocomplete can be used to complete the file names while executing a command.
- **Search:**
  - Travels recursively through each and every sub-directory in the working directory to find the file with the given name.
- **Up & Down arrowkey:**
  - Implements the up and down arrow keys to execute the already executed commands.

**To compile:**

1. Compile by typing this in the terminal: `make`

**To run:**

1. Execute the compiled binary: `./shell`

**Usage:**

- **Basic commands:**
  - `ls`
  - `pwd`
  - `cd`
  - `echo "hello"`
- **I/O redirection:**
  - `ls -l > output.txt`
  - `cat input.txt | wc`
  - `cat < input.txt`
  - `echo "hello" > output.txt`
- **Signal handling:**
  - Press CTRL-C to interrupt the current command.
  - Press CTRL-Z to stop the foreground process.
  - Press CTRL-D at the beginning of a line to send EOF.
  - Press CTRL-D at the end of an empty line to exit the shell.

**Limitations:**

- This is a simplified implementation.
- Lacks features like piping (|), background processes, job control, advanced I/O redirection (2>&1, etc.), and more robust signal handling.
- Does not support all shell commands and may have limitations in handling complex command lines.
- Does not support the history and pinfo commands.

**Further enhancements:**

- Implement piping (|).
- Add support for background processes (using &).
- Implement job control (fg, bg, jobs).
- Enhance I/O redirection (2>&1, etc.).
- Add support for shell built-in commands (cd, pwd, etc.).
- Improve error handling and user input validation.
- Add support for shell scripting.
- Add history and pinfo implementation.

**Note:**

- This README provides a basic overview of the shell's functionality.
- The actual behavior and features may vary depending on the specific implementation.
- This code is for educational purposes and may not be suitable for production use.
