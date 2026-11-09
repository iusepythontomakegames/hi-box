# hi box

A 640x480 white window with an 80x80 red box, centred on launch.
Press **B** and the box snaps to the top left corner of the client area.

## Build

    x86_64-w64-mingw32-windres box.rc -O coff -o box.res.o
    x86_64-w64-mingw32-gcc hi-box.c box.res.o -o hi-box.exe -mwindows -O2 -s \
      -Wl,--major-subsystem-version,6 -Wl,--minor-subsystem-version,0 \
      -Wl,--major-os-version,6 -Wl,--minor-os-version,0

## Binary

`hi-box.exe` is x64, GUI subsystem, subsystem version 6.0, with a VERSIONINFO
resource and an `asInvoker` manifest. Unsigned.

    size    19,456 bytes
    sha256  976c6fbcc509d46ece1bc31c868c110b0c00654ee64bc2112576f208f14403cc

Verified under Wine at 800x600: red pixel block measures 80x80 and sits at
client (280,200) on launch, and at client (0,0) after a `B` keypress.

## Note on antivirus

Unsigned, 19KB, zero prevalence. Heuristic engines may flag it. It imports only
`GDI32`, `KERNEL32`, `msvcrt` and `USER32` - no networking, no process creation,
no persistence. File a false-positive report with the vendor if it matters.
