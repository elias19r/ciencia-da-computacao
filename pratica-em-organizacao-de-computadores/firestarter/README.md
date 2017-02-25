# Firestarter v1.1

2013-05-24  
Elias Rodrigues (elias19r)

## About

This game was written in assembly language of ICMC-USP Processor
as a homework of Digital Computer Organization I.

In this game you control a tank and your mission is to destroy
the maximum number of mobs before you run out of time.

__NOTE__: the source code is commented in Portuguese language.

__NOTE__: the game has no sound effects or music.

## Keys

* Use `W` `A` `S` `D` to move the tank
* Hit `J` or `K` to fire
* To pause the game, press `ENTER`

## Install

To assemble and run the game you will need to compile `montador`
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
$ make run
```

and a window should appear with the game. Press `HOME` to start.

## Config

In order to run the game properly, adjust values in the following
routines in `main.asm` file:

```Assembly
delay:
dec_time:
```

## Media

* Video gameplay: <https://www.youtube.com/watch?v=aMQlprVsEXw>
* Screenshot: <http://s18.postimg.org/6e7xtiuax/firestarter_1_1.png>

