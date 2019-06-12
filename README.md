# Submission by Nagaarjun Nagarajan 

# Garrison Coding Test 

## Compile and Run

In order to compile, simply run the following command : `make`to get the executables for `server.exe` and `client.exe`. These can only be compiled in linux machines since Windows socket programming was harder and libraries were very buggy. 

In order to delete the executables, run `make clean`

To run the executables, run one instance of `./server` on a terminal and several instances of `./client` on seperate terminals. 

Then, simply enter the string you wish you HASH on client terminals and server will return hash values. In order to disconnect from server, type `:exit` in the client terminal you wish to close
