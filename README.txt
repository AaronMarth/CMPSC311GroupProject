To compile this program there are a few steps needed to be done.

1. Create a .txt file with some kind of text (to see the program work)

2. Compile the sendrcv.c as follow cc -c sendrcv.c (this will create sendrcv.o)

3. Compile the server as follows cc groupserver.c sendrcv.o -o groupserver

4. Compile the client as follows cc client.c sendrcv.o -o groupclient

5. To run program you must always run the server first
Ex. ./groupserver 127.0.0.1 5000 // address and port

6. Then run the client as follows
Ex. ./groupclient 127.0.0.1 5000 -s test.txt // address, port, type of copy, -s is client to server, -r is server to client, and file name
