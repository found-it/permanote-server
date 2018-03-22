# Perma-note TCP Server
### Written by: James Petersen

Have you ever accidentally deleted something you shouldn't have? Ever fallen asleep on your backspace key and deleted all of your notes? Yes? Ever wanted to store your notes permanently? Well you're in luck! With this simplistic server, you can store notes based on your custom username without fear of them being deleted. You can go back and read what you have previously written whenever you want! The notes are permanent which means no accidentally (or intentionally) deleting them. Give it a try and prepare to never use OneNote again.

## Current Supported Commands

```
help    - Displays a help menu.
hello   - Responds with a greeting.
write   - Reads text from the user and stores in file on server.
          Use ;;;<return> to end the user input.
read    - Displays the notes file for the logged in user.
```

## Usage

To build and run the project is as simple as:
```
make
./server
```

The server is bound to:

**Port:    4444**

**Address: 0.0.0.0 (ANY)**

It can be connected to locally using netcat
```
nc 0.0.0.0 4444
```

or from another machine
```
nc your.machine.ip.addr 4444
```

The server will prompt you for a username at first, go ahead and pick one and then begin issuing commands. You can quit the client by ctrl-C.

## Notes

**Compiled on:    Ubuntu 16.04 LTS**

**Kernel version: 4.13.0-32-generic**

_Comment style supports Doxygen, however I have not created a Doxyfile for the project. I would be happy to create one upon request._
