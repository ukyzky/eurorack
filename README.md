# Marbles alternative firmware v04

This firmware adds some functions to Marbles v1.2+.

- x clock input works as t trigger reset or t gate hold.
- set loop start position.
- save/load loop sequence in volatile memory (RAM).

## Change log

### v04

- Add x clock input alternative mode for t trigger reset and t gate hold.

### v03

- Change setting start position UI.
- Fix save/load reproductibity problem when T mode is "drum algorithm".

### v02

- Change save/load UI.
- Fix save/load reproductibity problem when T mode is "coin toss" / "multiply and divide".
- Add function of setting loop start position.

### v01

- Add function of save/load loop sequence.

## Save/Load loop sequence in volatile memory (RAM)

We can explore vast sea of random rhythm and return to the previous discovered precious random rhythm.

It's useful for live performance.

Play scenario is:

1. Explore great random rhythm by changing "deja vu knob" with deja vu enabled.
2. Lock the great rhythm by setting "deja vu knob" to 50%(12 o'clock).
3. (NEW FUNCTION) Save the rhythm
  - 3a. long-press "T rate switch".
  - 3b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in save/load mode.)
  - 3c. then press "T deja vu switch".
  - 3d. then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents saving is executed.)
4. Explore another great random rhythm by changing "deja vu knob".
5. Lock the great rhythm by setting "deja vu knob" to 50%.
6. (NEW FUNCTION) Load the previous rhythm
  - 6a. long-press "T rate switch".
  - 6b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in save/load mode.)
  - 6c. then press "X deja vu switch".
  - 6d. then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents loading is executed.)

7. Notes: After long-press "T rate switch" ("T rate LED" is blinking), for canceling save/load, press "T rate switch".

## Set loop start position

We can select short length loop patterns from precious random rhythm.

It's useful for live performance.

Play scenario is:

1. Explore great random rhythm by changing "deja vu knob" with deja vu enabled.
2. Lock the great rhythm by setting "deja vu knob" to 50%(12 o'clock).
3. Set deja vu loop length using "loop length knob".

4. (NEW FUNCTION) Set deja vu loop start position

  - In save/load mode, "loop length knob" works as setting loop start position.

  - 4a. long-press "T rate switch".
  - 4b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in save/load mode.)
  - 4c. turn "loop length knob" to set loop start position.
  - 4d. push "T rate switch". then "T rate LED" stop blink. (It represents save/load mode exited.)

## x clock input for t trigger reset mode or t gate hold mode

We can use x clock input as t trigger rising edge reset mode (x is clocked by internal t).

And we can use x clock input as t gate hold mode (x is clocked by internal t).

Play scenario is:

1. Connect something to x clock input.

2. (NEW FUNCTION) Set x clock input alternative mode

  - In save/load mode, "external button" works as setting x clock input alternative mode.

  - 2a. long-press "T rate switch".
  - 2b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in save/load mode.)
  - 2c. push "external switch". then "T rate LED" stop blink. (It represents save/load mode exited.) Then x clock input works as t trigger rising edge reset mode.

  - Repeat 2a, 2b and 2c. Then x clock input works as t gate hold mode.

  - 2d. long-press "T rate switch".
  - 2e. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in save/load mode.)
  - 2f. long-press "external switch". then "T rate LED" stop blink. (It represents save/load mode exited.) Then x clock input works as normal x clock mode.

=======

Mutable Instruments' Eurorack Modules.

* [Blinds](http://mutable-instruments.net/modules/blinds): Quad VC-polarizer.
* [Braids](http://mutable-instruments.net/modules/braids): Macro-oscillator.
* [Branches](http://mutable-instruments.net/modules/branches): Dual Bernoulli gate.
* [Clouds](http://mutable-instruments.net/modules/clouds): Texture synthesizer.
* [Ears](http://mutable-instruments.net/modules/ears): Contact microphone.
* [Edges](http://mutable-instruments.net/modules/edges): Quad chiptune digital oscillator.
* [Elements](http://mutable-instruments.net/modules/elements): Modal synthesizer.
* [Frames](http://mutable-instruments.net/modules/frames): Keyframer/mixer.
* [Grids](http://mutable-instruments.net/modules/grids): Topographic drum sequencer.
* [Kinks](http://mutable-instruments.net/modules/kinks): Utility module - rectifier, analog logic, S&H, noise.
* [Links](http://mutable-instruments.net/modules/links): Utility module - buffer, mixer.
* [Marbles](http://mutable-instruments.net/modules/marbles): Random sampler.
* [Peaks](http://mutable-instruments.net/modules/peaks): Dual trigger converter.
* [Plaits](http://mutable-instruments.net/modules/plaits): Macro oscillator.
* [Rings](http://mutable-instruments.net/modules/rings): Resonator.
* [Ripples](http://mutable-instruments.net/modules/ripples): Liquid 2-pole BP, 2-pole LP and 4-pole LP filter.
* [Shades](http://mutable-instruments.net/modules/shades): Triple attenuverter.
* [Shelves](http://mutable-instruments.net/modules/shelves): EQ filter.
* [Stages](http://mutable-instruments.net/modules/stages): Segment generator.
* [Streams](http://mutable-instruments.net/modules/streams): Dual dynamics gate.
* [Tides](http://mutable-instruments.net/modules/tides): Tidal modulator.
* [Veils](http://mutable-instruments.net/modules/veils): Quad VCA.
* [Volts](http://mutable-instruments.net/modules/volts): +5V power module.
* [Warps](http://mutable-instruments.net/modules/warps): Meta-modulator.
* [Yarns](http://mutable-instruments.net/modules/yarns): MIDI interface.

License
=======

Code (AVR projects): GPL3.0.

Code (STM32F projects): MIT license.

Hardware: cc-by-sa-3.0

By: Emilie Gillet (emilie.o.gillet@gmail.com)

Guidelines for derivative works
===============================

**Mutable Instruments is a registered trademark.**

The name "Mutable Instruments" should not be used on any of the derivative works you create from these files.

We do not recommend you to keep the original name of the Mutable Instruments module for your derivative works.

For example, your 5U adaptation of Mutable Instruments Clouds can be called "Foobar Modular - Particle Generator".
