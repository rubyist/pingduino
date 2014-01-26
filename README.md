# Pingduino

Pingduino an Arduino based score keeper for games of ping pong. This repository
contains everything you need to build your own. Except the hardware, of course!

## Features

Pingduino is set up such that there are two scoring buttons and a display. I've
mounted the buttons underneath my ping pong table, one on each side. When a player
scores, they press their button.

The display is built to show each score and an indicator showing whose who is
currently serving. The serve indicator updates automatically as scores are input.

The program code contains game winning logic, meaning it knows the rules of
ping pong and will indicate the winner when it detects the game has been won.

Upon detecting a win, Pingduino will play a victory song and flash the serve
indicator of the game winner.

If Pingduino detects no activity for 20 minutes it will automatically turn off
the display lights and the scoring button lights. Pressing any button will wake
the display and Pingduino will pick up where you left off.

## Usage Instructions

Once you've built and installed Pingduino it's time to play! Simply start playing
ping pong. Pingduino lets you know which player should serve. If you score, press
your button. Eventually somebody will win!

Once the game has been won, the player who will serve the next game will reset
the system by doing a long press (press and hold for a second or two) on their
button.

## Building Your Own

Building your own Pingduino should be easy. It's my first Arduino project and
I'm a pretty novice hardware hacker. This repository contains the program code
to load onto the Arduino as well as schematics, a parts list, and a PCB layout
should you choose to go beyond the breadboard. Once completed I'll post a link
to pictures of my build process.

### Display

I was initially going to use some giant ~8" 7-segment LEDs for the score display,
however the two that I found that were big enough were too expensive and not well
built or out of stock. I decided to build my own large 7-segment displays out of
ping pong balls and individual LEDs. Once assembled, these behave pretty much like
regular 7-segment displays and I'm driving them with 4 shift registers chained
together.

I've kept the display logic code fairly encapsulated so that you can be free to
implement whatever kind of display you'd like. My intitial prototypes were built
using an LCD display.

### Scoring Buttons

There are scoring buttons mounted under each side of the table. I used some
[big dome push buttons](https://www.sparkfun.com/products/11275) from Sparkfun.
These are pretty big, you could certainly get away with much smaller buttons,
but I ordered them so I rolled with it.

To connect the buttons to the main module I used some 1/8" audio jacks and cables.
These are readily available, cheap, and perfectly support the button signal and
a line to light the LED in the button.

To mount the buttons under the table I designed and printed some simple boxes.
You can find the openscad and stl files in the models directory. I printed these
on a Makerbot Replicator 2. It's just a box that goes under a table so you can
get away with printing at a lower resolution (0.3mm), but be aware that the hole
for the audio jack will probably have to be drilled and smoothed out some before
it fits nicely.

## Contributing

Got an idea for improving Pingduino? Found a bug? I'd love to know about it!
Simply file an issue or open a pull request here on GitHub.

If you build and use a Pingduino I'd love to know about it, especially if you
come up with a different kind of display.

## License

Everything in this repo is released under the MIT license.
