# Transferring files between Windows/Unix and DEC TOPS-10 using KERMIT

## Getting started

Once you have the simh PDP-10 emulator booting TOPS-10 and listening to port 2020 for terminal connections, use C-Kermit or your terminal emulator to connect to it. You should see (if using simh v3.9):

```
Connected to the PDP-10 simulator DZ device, line 0
```

Now hit return until you get a '.' prompt character, then login as public:

```
.login public
Job 3  VNW  TTY0
18:20   11-Jun-6   Monday

.
```

To run Kermit, enter:

```
.r k10mit
```

You should see:

```
TOPS-10 KERMIT version 3(136)
Kermit-10>
```

Be sure to set the file byte-size properly:

```
Kermit-10>set file byte-size 36-bit
Kermit-10>
```

You may wish to either create a KERMIT.INI configuration file on TOPS-10 using an editor (for example TECO), or transfer a configuration file to TOPS-10 from your host per the approaches that follow. Once KERMIT.INI is on TOPS-10, you will no longer need to set the byte-size... it will be done automatically each time you run Kermit.

My Kermit configuration file contains the following lines:

```
SET SEND TIMEOUT 2
SET RECEIVE TIMEOUT 2
SET SEND PACKET-LENGTH 768
SET RECEIVE PACKET-LENGTH 768
SET FILE BYTE-SIZE 36-BIT
```

I think the only essential line is the last one. You must set the byte-size to 36 bits, or transferred files will be corrupted.


## Downloading files from TOPS-10

Simply type `send filename` at the Kermit-10 prompt. If you are running C-Kermit, it should automatically begin to receive the file. If you are running a terminal
emulator, you will likely need to initiate the Kermit receive file function.


## Uploading files to TOPS-10

### If using c-kermit

At the Kermit-10 prompt type `receive filename`. You won't get a prompt back and it will just sit there waiting for a file. The next step is to escape back into C-Kermit by typing Control-\c (hit control and \ and then type c and possibly a carriage return). If using Kermit 95 the default escape sequence is Alt-x.

You should now be back the C-Kermit> prompt. Once there, enter `send filename`.

To return to TOPS-10 after sending the file, enter 'c' (for connect).

### If using a terminal emulator

Simply initiate the Kermit send file function at this point.
