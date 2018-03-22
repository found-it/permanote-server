# TCP Server Program
## Written by: James Petersen

### Port:    4444
### Address: 0.0.0.0 (ANY)
This program is a simple command line TCP Server implemented in C.

## Current Commands

```
help    - Displays a help menu.
hello   - Responds with a greeting.
```

## Usage

You can connect to the server by opening up another terminal and using netcat
```
nc 0.0.0.0 4444
```

The server will prompt you for a username at first, go ahead and pick one and then begin issuing commands. You can quit the client by ctrl-C.
