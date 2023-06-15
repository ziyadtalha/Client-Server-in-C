Written in C language.

Follows a client-server architecture pattern but without any actual socket programming.

Used to demonstrate how shared memory can be used to allow two unrelated processes to communicate.

A client process reads data from a file named “number.txt" and sends this data to a server process (unrelated process) via shared memory.
Note that the file name is passed as a command-line argument by the user.

The server process reads the data from the shared memory and display the sum and average of the integers on the console.
