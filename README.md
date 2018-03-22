# TCP Server Program
### Written by: James Petersen

This program is a simple command line TCP Server implemented in C.

### Port:    4444
### Address: 0.0.0.0 (ANY)

## Current Commands

```
help    - Displays a help menu.
hello   - Responds with a greeting.
write   - Reads text from the user and stores in file on server.
          Use ...<return> to end the user input.
read    - Displays the notes file for the logged in user.
```

## Usage

To build and run the project is as simple as:
```
make
./server
```

Connect to the server by opening up another terminal and using netcat
```
nc 0.0.0.0 4444
```

The server will prompt you for a username at first, go ahead and pick one and then begin issuing commands. You can quit the client by ctrl-C.
