# Variables
# Compiler
CC = gcc

# List of all sub directories
SUBDIR = . possixOperation possixHandler fileHandler
# List of all include directories with header file
INCDIR = . possixOperation possixHandler fileHandler Common
# Dependency Flags
# DEPFLAGS = -MP -MD  $(DEPFLAGS)
# Flags with include path
CFLAGS = -Wall -Werror -Wextra $(foreach D,$(INCDIR),-I$(D))

# Make a list of C source files in project
# foreach will iterate through SUBDIR and find all .c file
SOURCE = $(foreach D,$(SUBDIR),$(wildcard $(D)/*.c))
file_names = $(notdir $(SOURCE))

# Create dependency files incase headers changed
DEPND_FILES = $(patsubst %.c,%.d,$(SOURCE_FILES))

VPATH = .:possixOperation:possixHandler:fileHandler

# Target : Dependencies
# Specify the rule for Dependencies
all : Debug Executable

# Need to define rules for Debug
Debug : $(SOURCE)
	$(CC) $(CFLAGS) -g $(SOURCE) -o Debug

# Need to define rules for Executable
Executable : $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o Out.exe
        
# Clean is needed to clean everything
# Includes rules to remove and clear
.PHONY = clean
clean : 
	rm -f main
	rm -f Debug
	find . -type f -name "*.o" -delete
	find . -type f -name "*.d" -delete
	find . -type f -name "*.s" -delete
	find . -type f -name "*.exe" -delete

# Include Dependencies
# -include $(DEPND_FILES)