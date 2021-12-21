# Rings woody firmware v02

This firmware can select Rings dsp sample rate 48kHz (normal) or 32kHz (like Elements).

The 32kHz sound will be more woody, natural and organic like Elements.

## Operation of toggle sample rate

We can toggle sample rate setting 48kHz and 32kHz.

The sample rate setting is saved to non volatile memory.

The instruction is nearly like Rings easter egg, but difference is
- all attenuator knobs fully ccw.
- after LED blinks, reboot power cycle needed.

### Instruction

1. **set the knobs:**

Frequency: fully ccw

Structure: fully cw

Brightness: fully ccw

Damping: centre

Position: fully cw

**set the attenuator knobs:**

Brightness: fully ccw

Frequency: fully ccw

Damping: fully ccw

Structure: fully ccw

Position: fully ccw

2. long-press polyphony button (long-press mode button is also ok)

Then LED blinks a few seconds.
The sample rate setting is saved to non volatile memory.

3. reboot (power off and power on) Rings module

then sample rate changes (toggle 48kHz or 32kHz).

## Change log

### v02

- Add user interface of selecting Rings dsp sample rate 48kHz or 32kHz.

### v01

- Change Rings dsp sample rate from 48kHz to 32kHz.

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
