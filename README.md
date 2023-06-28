# Marbles 1.3 alternative firmware v0.1.0

This firmware adds some functions to Marbles latest v1.3 firmware.

## List of alternative function

- Activate Markov algorithm of t mode

## Cheat sheet

![image](marbles/doc/manual.png?raw=true "image")

- Activate Markov algorithm of t mode

  - hold t mode button **[E]** and press T rate button **[B]**

## Change log

### v0.1.0

#### Added

- Initial release based on official latest v1.3 firmware. So superlock feature and reset input option are included.

- Activate hidden Markov algorithm of t mode.

> **NOTES**
>
> Alternative features of previous releases based on v1.2+ firmware (Chord mode, adjustable loop points, increased loop duration, temporary save/load sequence etc.) aren't included. They will be included in future.

## Activate Markov algorithm of t mode

We can select "Markov algorithm" of t mode.

The t mode setting is saved to non volatile memory.

Play scenario is:

1. (NEW FUNCTION) Set Markov algorithm of t mode

  - 1a. **hold "t mode switch"**
  - 1b. press "T rate switch"
  - t mode becomes Markov algorithm. During Markov algorithm mode, t mode LED blinks faster with red color.

2. Unset Markov algorithm of t mode

  - 2a. press or long-press "t mode switch".
  - t mode becomes normal "drum algorithm" or "three state algorithm".

## Notes

### Policy of this alternative firmware

- This is based on official latest v1.3 firmware. So superlock feature and reset input option are included.

- Using this alternative firmware with all normal setting, the behavior is the same as official firmware.

=======

Mutable Instruments' Eurorack Modules.

* [Blades](http://mutable-instruments.net/modules/blades): Dual multimode filter.
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
