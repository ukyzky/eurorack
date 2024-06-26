# Marbles alternative firmware v05

This firmware adds some functions to Marbles v1.2+.

- T jitter cv input can work as X quantizer scale slot select.
- X external clock input can work as T trigger reset or T gate hold.
- set loop start position.
- save/load loop sequence in volatile memory (RAM).

## Change log

### v05

- Add T jitter cv input alternative mode for X quantizer scale slot select.
- Save/Load alternative mode setting (X external clock input mode and T jitter cv input mode) to non volatile memory. (loop start position also saved and loaded from previous firmware version.)

### v04

- Add X external clock input alternative mode for T trigger reset and T gate hold.

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

We can explore random rhythm and return to the previous discovered  random rhythm.

Play scenario is:

1. Explore random rhythm.
2. Lock the rhythm by setting "deja vu knob" to 50%(12 o'clock) with deja vu enabled.
3. (NEW FUNCTION) Save the rhythm
  - 3a. long-press "T rate switch".
  - 3b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 3c. then press "T deja vu switch".
  - 3d. then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents saving is executed and "alternative setting edit" mode exited.)
4. Explore another random rhythm by changing "deja vu knob" or switching deja vu enabled-disabled.
5. Lock the rhythm by setting "deja vu knob" to 50% with deja vu enabled.
6. (NEW FUNCTION) Load the previous rhythm
  - 6a. long-press "T rate switch".
  - 6b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 6c. then press "X deja vu switch".
  - 6d. then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents loading is executed and "alternative setting edit" mode exited.)

## Set loop start position

We can select short length loop patterns from locked random rhythm.

The loop start position setting can be saved to non volatile memory.

Play scenario is:

1. Explore random rhythm.
2. Lock the rhythm by setting "deja vu knob" to 50%(12 o'clock) with deja vu enabled.
3. Set deja vu loop length using "loop length knob".

4. (NEW FUNCTION) Set deja vu loop start position

  - (In "alternative setting edit" mode, "loop length knob" works as setting loop start position.)

  - 4a. long-press "T rate switch".
  - 4b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 4c. turn "loop length knob" to set loop start position.
  - 4d. push "T rate switch". then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents "alternative setting edit" mode exited.)

## X external clock input alternative mode

We can use X external clock input as T trigger rising edge reset mode (x is clocked by internal t clock).

And we can use X external clock input as T gate hold mode (x is clocked by internal t clock).

The X external clock input mode setting can be saved to non volatile memory.

Play scenario is:

1. Connect something to X external clock input jack. (It's important.)

2. (NEW FUNCTION) Set X external clock input alternative mode

  - (In "alternative setting edit" mode, "external processing switch" works as setting X external clock input alternative mode.)

  - 2a. long-press "T rate switch".
  - 2b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 2c. push "external processing switch". then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents "alternative setting edit" mode exited.)
  - Then X external clock input works as T trigger rising edge reset mode. When trigger/gate rising edge signal detected at X external clock input, internal T and X sequence are resetted.

  - Repeat above 2a, 2b and 2c.
  - Then X external clock input works as T gate hold mode. During trigger/gate ON signal at X external clock input, internal T and X sequence are holded (in another word, pause and resume behavior).

  - Each repeating above 2a, 2b and 2c, X external clock input mode changes to T trigger rising edge reset mode or T gate hold mode.

3. Set X external clock input normal mode

  - 3a. long-press "T rate switch".
  - 3b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 3c. long-press "external processing switch". then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents "alternative setting edit" mode exited.)
  - Then X external clock input works as normal X external clock mode.

## T jitter cv input alternative mode

We can use T jitter cv input as selecting X quantizer scale slot.

| cv input value | selected quantizer scale slot |
| --- | --- |
| 0v - 0.5v | 1 |
| 0.5v - 1.5v | 2 |
| 1.5v - 2.5v | 3 |
| 2.5v - 3.5v | 4 |
| 3.5v - 4.5v | 5 |
| 4.5v - 5v | 6 |

During X quantizer select cv input mode, T jitter setting is set by T jitter knob.

During X quantizer select cv input mode, normal (original firmware's) quantizer selected slot setting value is ignored.

Selected quantizer scale slot value by X quantizer select cv input doesn't overwrite normal (original firmware's) quantizer selected slot setting value.

The T jitter cv input mode setting can be saved to non volatile memory.

Play scenario is:

1. (NEW FUNCTION) Set T jitter cv input alternative mode

  - (In "alternative setting edit" mode, "X range switch" works as setting T jitter cv input alternative mode.)

  - 1a. long-press "T rate switch".
  - 1b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 1c. push "X range switch". then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents "alternative setting edit" mode exited.)
  - Then T jitter cv input works as X quantizer slot select mode.

2. Set T jitter cv input normal mode

  - 2a. long-press "T rate switch".
  - 2b. then "T rate LED" becomes blink. (and detouch from the switch.) (It represents currently in "alternative setting edit" mode.)
  - 2c. long-press "X range switch". then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents "alternative setting edit" mode exited.)
  - Then T jitter cv input works as normal T jitter cv input mode.

## Notes

### Canceling "alternative setting edit"

After long-press "T rate switch" ("T rate LED" is blinking), for canceling "alternative setting edit", press "T rate switch".

### Save timing of alternative mode setting

For Saving alternative mode setting (loop start position, X external clock input mode and T jitter cv input mode) to non volatile memory,
please press "T mode swith" or "T range switch" or "X mode switch" or "X range switch" after setting alternative mode edit done.

It's the same behavior as normal Marbles firmware.

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
