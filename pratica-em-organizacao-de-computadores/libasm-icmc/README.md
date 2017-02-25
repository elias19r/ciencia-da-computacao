# Basic functions "libasm"

2013  
Elias Rodrigues (elias19r)

## About

This is a list of basic I/O functions written in assembly language of
ICMC-USP Processor as a homework of Digital Computer Organization I.

__NOTE__: the source code is commented in Portuguese language.

## Install

To assemble and run the examples you will need to compile `montador`
and `simulador` programs. To do so, execute:

```Shell
$ make build-montador
$ make build-simulador
```

If any error regarding dependencies occurred while compiling the
`simulador`, you might check for:

```Shell
$ sudo apt-get install libgtk2.0-dev
$ sudo apt-get install libncurses5-dev
```

Then try again:

```Shell
$ make build-simulador
```

After `montador` and `simulador` has been compiled, execute:

```Shell
$ make run asm=filename
```

where _filename_ refers to the file you want to run.

For example:

``` Shell
$ make run asm=printd
```

and a window should appear with the example `printd.asm`. Press `HOME` to start.

