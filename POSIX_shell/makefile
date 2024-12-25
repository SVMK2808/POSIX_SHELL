# Name of the executable
EXECUTABLE = shell

# Source files
SRC = main.cpp \
      ls.cpp \
      cd.cpp \
      pinfo.cpp \
      echo.cpp \
      history.cpp \
      search.cpp \
      pipeline.cpp \
      pwd.cpp \
      autoComplete.cpp \
	ioredirect.cpp \
	foregroundbackground.cpp

# Include directories (if any)
INCLUDES = -I. 

# Compiler flags
CFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Linker flags
LDFLAGS = -lreadline -pthread

# Target rule to build the executable
$(EXECUTABLE): $(SRC)
	g++ $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

# Clean target to remove object files and executable
clean:
	rm -f $(EXECUTABLE) *.o

# Phony target for dependencies
.PHONY: clean