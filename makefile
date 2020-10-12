CC = gcc
WARNING_FLAGS = -g -Wall -Wextra
#
# Sample makefile updated to include the commands to run the Clang Static Analyzer
#
#    scan-build will "make" your program and run CSA at the same time.
#
#    scan-view will do a scan-build (if necessary) and then open a web browser
#      window to review the results.
#
CC = gcc
WARNING_FLAGS = -Wall -Wextra
OPTS = -pthread
EXE = prodcom
SCAN_BUILD_DIR = scan-build-out

all: main.o module.o
	$(CC) -o $(EXE) main.o module.o $(OPTS)

main.o: main.c module.h 
	$(CC) -c $(WARNING_FLAGS) main.c

module.o: module.c module.h
	$(CC) -c $(WARNING_FLAGS) module.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 
