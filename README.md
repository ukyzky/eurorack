# Rings woody firmware v03

This firmware can select Rings dsp sample rate 32kHz (like Elements) or 48kHz (normal).

The 32kHz sound will be more woody, natural and organic like Elements.

And this firmware can select chord constituting in easter egg mode (like Plaits or normal).

(firmware v01 and v02 chord constituting is like Plaits.)

This firmware's default setting value is sample rate 32kHz and chord like Plaits.

## indicating setting value

When power up Rings with this firmware,

If easter egg mode enabled, LEDs blinks a few seconds. (It's the same behavior of normal firmware.)

Then polyphony LED blinks a few seconds (green color: sample rate 32kHz, red color: 48kHz) and mode LED blinks a few seconds (green color: chord like Plaits, red color: normal).

## Operation of toggle sample rate

We can toggle sample rate setting 32kHz or 48kHz.

The sample rate setting is saved to non volatile memory.

The instruction is nearly like Rings easter egg, but difference is
- all attenuator knobs fully ccw.

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

2. long-press **polyphony** button

Then sample rate changes (toggle 48kHz or 32kHz).
The sample rate setting is saved to non volatile memory.

Then automatically reboot module.

(Then if easter egg mode enabled, LEDs blinks a few seconds.)

Then polyphony LED blinks a few seconds (green color: 32kHz, red color: 48kHz).
(And mode LED blinks a few seconds (green color: chord like Plaits, red color: normal).)

## Operation of toggle chord constituting

We can toggle chord constituting normal or like Plaits (by enable/disable BRYAN_CHORDS).

The chord setting is saved to non volatile memory.

The instruction is nearly like Rings easter egg, but difference is
- all attenuator knobs fully ccw.

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

2. long-press **mode** button

Chord constituting changes immediately (normal or like Plaits).

Then mode LED blinks a few seconds (green color: chord like Plaits, red color: normal).
(And polyphony LED blinks a few seconds (green color: 32kHz, red color: 48kHz).)

The chord setting is saved to non volatile memory.

## Change log

### v03

- Add user interface of selecting Rings easter egg mode chord constituting normal or like Plaits.
- Automatically reboot after selecting dsp sample rate.

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
