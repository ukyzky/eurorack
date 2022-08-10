# Marbles alternative firmware v11

This firmware adds some functions to Marbles latest v1.2+ firmware.

## alternative settings mode

- X external clock input can work as T trigger reset or T gate hold. (In the reset/hold mode, setting option of internal self patch from T1/T2/T3 to X clock added.)
- T rate cv input can work as deja vu loop length or start position select (four alternative modes).
- T jitter cv input can work as X quantizer scale slot select.
- X spread cv input can work as X quantizer root select (two alternative modes).
- chord mode (five alternative modes). x always quantized with new steps mapping. x generates different note as possible. controlling x same note probability. controlling x slew amount.
- deja vu loop buffer size (five alternative modes).
- save/load loop sequence in volatile memory (RAM).

## additional function in normal mode

- activate Markov algorithm of t mode.

## Cheat sheet

![image](marbles/doc/manual.png?raw=true "image")

- Enter alternative settings mode

  - Long press t range button **[B]**

  (In the alternative settings mode, t deja vu LED, X deja vu LED and X external processing mode LED are animated.)

- Edit alternative settings

  In the alternative settings mode,

  - Change current alternative setting target : Press t range button **[B]**

  - Change current alternative setting type : Press X range button **[J]**

  (changed values are reflected after exiting alternative settings mode)

  **(alternative setting target is represented by t mode LED and X mode LED)**

  *(alternative setting type is represented by t range LED and X range LED)*

  Below remarks: (G: Green, Y: Yellow, R: Red)

| normal function | alternative function | **t mode LED** | **X mode LED** | *t range LED* | *X range LED* | *(Type)* |
|---|---|---|---|---|---|---|
| X clock in | (same as normal) | G | G | G | G | 0 |
| X clock in | Reset trigger in | G | G | G | Y | 1 |
| X clock in | Hold gate in | G | G | G | R | 2 |
| T rate cv in / Loop length knob | (same as normal) | G | Y | G | G | 0 |
| T rate cv in / Loop length knob | Loop length cv in / Loop start pos knob | G | Y | G | Y | 1 |
| T rate cv in / Loop length knob | Loop start pos cv in / Loop length knob | G | Y | G | R | 2 |
| T rate cv in / Loop length knob | Loop start pos cv in / Loop start end pos offset(rotate) knob (Loop end pos is fixed at 16 when offset is 0. So loop start pos and length are changed at the same time.) | G | Y | Y | G | 3 |
| T rate cv in / Loop length knob | T rate cv in / Loop start pos knob (Loop end pos is fixed at 16. So loop start pos and length are changed at the same time. It's inspired from Serge Sequencer.) | G | Y | Y | Y | 4 |
| T jitter cv in | (same as normal) | G | R | G | G | 0 |
| T jitter cv in | Quantizer scale select cv in | G | R | G | Y | 1 |
| X spread cv in | (same as normal) | Y | G | G | G | 0 |
| X spread cv in | Quantizer root cv in | Y | G | G | Y | 1 |
| X spread cv in | Quantizer root cv in (without offset) | Y | G | G | R | 2 |
| X clock internal source in reset/hold mode | (same as normal) | Y | Y | G | G | 0 |
| X clock internal source in reset/hold mode | T1 | Y | Y | G | Y | 1 |
| X clock internal source in reset/hold mode | T2 | Y | Y | G | R | 2 |
| X clock internal source in reset/hold mode | T3 | Y | Y | Y | G | 3 |
| deja vu loop buffer size mode | 16 (same as normal) | Y | R | G | G | 0 |
| deja vu loop buffer size mode | 128 (Jthrw's 1,2,3,4,5,6,7,8,10,12,14,16, 24,32,48,64,96,128) | Y | R | G | Y | 1 |
| deja vu loop buffer size mode | 128 (1,2,4,8,16,32,64,128) | Y | R | G | R | 2 |
| deja vu loop buffer size mode | 192 (1,3,6,12,24,48,96,192) | Y | R | Y | G | 3 |
| deja vu loop buffer size mode | 320 (1,5,10,20,40,80,160,320) | Y | R | Y | Y | 4 |
| deja vu loop buffer size mode | 233 (1,2,3,5,8,13,21,34,55,89,144,233) | Y | R | Y | R | 5 |
| chord mode | (same as normal) | R | G | G | G | 0 |
| chord mode | x always quantized and generated different note as possible. | R | G | G | Y | 1 |
| chord mode (x steps cv in) | x always quantized and generated different note as possible. (x same note probability cv in) | R | G | G | R | 2 |
| chord mode (x bias cv in) | x always quantized and generated different note as possible. (x same note probability cv in) | R | G | Y | G | 3 |
| chord mode (x bias cv in / x steps cv in) | x always quantized and generated different note as possible. (x same note probability cv in / x slew amount cv in) | R | G | Y | Y | 4 |
| chord mode (x bias knob / x steps knob) | x always quantized and generated different note as possible. (x same note probability knob / x slew amount knob) | R | G | Y | R | 5 |

- Save loop sequence

  In the alternative settings mode,

  - Press deja vu T button **[F]**

- Load loop sequence

  In the alternative settings mode,

  - Press deja vu X button **[G]**

- Exit alternative settings mode

  In the alternative settings mode,

  - Press X external processing mode button **[O]**

- Reset all alternative settings and exit alternative settings mode

  In the alternative settings mode,

  - Long press X external processing mode button **[O]**

- Activate Markov algorithm of t mode.

  In the normal mode,

  - hold t mode button **[E]** and press T rate button **[B]**

## Change log

### v11

- Add chord mode (five alternative modes). x always quantized with new x steps mapping (7 o'clock: only rare notes. 12 o'clock: all notes. 5 o'clock: only general notes.). x generates different notes as possible. controlling x same note probability. controlling x slew amount.

### v10

- Add deja vu loop buffer size mode (128, base2 up to 128, base3 up to 192, base5 up to 320, fibonacci up to 233)
- Improve t deja vu loop pattern stability of "drum mode" on "Reset trigger in" enabled mode (GGGY).
- Improve t deja vu loop pattern stability of "drum mode" on "Loop start pos" enabled mode (GYGY, GYGR, GYYG, GYYY).
- Change t "drum mode" pattern immediately in t deja vu DISABLED state when "Reset trigger in" triggered on "Reset trigger in" enabled mode (GGGY).

### v09.1

- Add setting internal self patch from T1/T2/T3 to X clock (for X external clock input set as T trigger reset or T gate hold).
- Remove limitation of "always insert jack to X clock in" on reset/hold mode. So no longer need to take care of jack for X clock on reset/hold mode.
- **(breaking change)** change "T rate cv in / Loop length knob" setting Type3 Loop length knob behavior from loop start pos cv attenuverter to loop start/end pos offset(rotate) (0 to 15).

### v09

- Add deja vu loop start pos and length simultaneous control mode (Type3 and Type4) inspired from Serge sequencer.
- Add boot up LED animation for indicating alternative firmware installed.
- Change user interface for alternative settings. It's a menu system using LEDs.
- Remove deprecated function of alternative deja vu length knob during in the alternative settings mode.

### v08

- Add T rate cv input alternative mode for deja vu loop length or start position select.

### v07

- Activate hidden Markov algorithm of t mode.

### v06

- Add X spread cv input alternative mode for X quantizer root select.
- Add resetting operation for returning all alternative settings value to default value.
- alternative mode settings immediately saved on exiting alternative settings mode.

### v05

- Add T jitter cv input alternative mode for X quantizer scale slot select.
- Save/Load alternative mode setting (X external clock input mode and T jitter cv input mode) to non volatile memory. (loop start position also saved and loaded from previous firmware version.)

### v04

- Add X external clock input alternative mode for T trigger reset and T gate hold.

### v03

- Change setting start position UI.
- Fix save/load reproductibity problem when t mode is "drum algorithm".

### v02

- Change save/load UI.
- Fix save/load reproductibity problem when t mode is "coin toss" / "multiply and divide".
- Add function of setting loop start position.

### v01

- Add function of save/load loop sequence.

## Loop length cv (using T rate cv input)

We can use T rate cv input as selecting deja vu loop length or start position.

The cv range is from 0V to 5V. The cv value is reflected to deja vu loop value linearly (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16).

During deja vu loop cv input mode, T rate setting is set by T rate knob.

The deja vu loop cv input mode setting is saved to non volatile memory.

1. Type1

  - T rate cv input works as deja vu loop length cv (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) mode.
  - deja vu knob act as deja vu loop start position (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) knob.
  - This loop start position value is active only in this mode.

2. Type2

  - T rate cv input works as deja vu loop start position cv (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) mode.
  - This loop start position value is active only in this mode.
  - deja vu length knob act as normal function (1,2,3,4,5,6,7,8,10,12,14,16).

3. Type3 (added at v9)

  - T rate cv input works as deja vu loop start position cv (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) mode (loop end position is fixed at 16. So loop start position and length are controlled simultaneously). It's inspired from Serge Sequencer.
  - For example, loop start position is 1 then loop length is 16. Loop start position is 16 then loop length is 1.
  - deja vu length knob acts as deja vu loop start/end position offset(rotate) (CCW is 0, CW is 15). So generally deja vu length knob position is CCW. Loop start position is calculated by adding loop start position cv value (1 to 16) and loop start position offset knob value (0 to 15). If calculated value overflow 16, the value is subtracted by 16.
  - This loop start position value is active only in this mode.

4. Type4 (added at v9)

  - T rate cv input is the same as normal function.
  - deja vu length knob works as deja vu loop start position (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) mode (loop end position is fixed at 16. So loop start position and length are controlled simultaneously). It's inspired from Serge Sequencer.
  - For example, loop start position is 1 then loop length is 16. Loop start position is 16 then loop length is 1.
  - For example, deja vu length knob position: (CCW is loop start position 1 and loop length 16. CW is loop start position 16 and loop length 1.)
  - This loop start position value is active only in this mode.

## T trigger reset (using X external clock input)

We can use X external clock input as T trigger rising edge reset mode (x is clocked by internal t clock).

And we can use X external clock input as T gate hold mode (x is clocked by internal t clock).

The X external clock input mode setting is saved to non volatile memory.

1. Type1

  - X external clock input works as T trigger rising edge reset mode. When trigger/gate rising edge signal detected at X external clock input, internal T and X sequence are resetted.

2. Type2

 - X external clock input works as T gate hold mode. During trigger/gate ON signal at X external clock input, internal T and X sequence are holded (in another word, pause and resume behavior).

## X clock internal source in T trigger reset/hold mode

When T trigger reset/hold mode is set, self patching from T1/T2/T3 to X clock input is impossible physically.

So as alternative way, in T trigger reset/hold mode, we can set X clock source.

1. Type1

 - T1 is internal patched to X clock.

2. Type2

 - T2 is internal patched to X clock.

3. Type3

 - T3 is internal patched to X clock.

## X Quantizer scale select cv (using T jitter cv input)

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

The T jitter cv input mode setting is saved to non volatile memory.

1. Type1

  - T jitter cv input works as X quantizer slot select mode.

## X Quantizer root cv (using X spread cv input)

We can use X spread cv input as selecting X quantizer root.

During X quantizer root cv input mode, X spread setting is set by X spread knob.

The X spread cv input mode setting is saved to non volatile memory.

1. Type1

  - X spread cv input works as X quantizer root mode Type A (adding offset root mode).

2. Type2

 - X spread cv input works as X quantizer root mode Type B (not adding offset but reflect root).

## chord mode

- Type1,2,3,4,5:

x always quantized with new x steps mapping (7 o'clock: only rare notes. 12 o'clock: all notes. 5 o'clock: only general notes.).

x generates different note from the others x. (Limitation: If variation of notes is a few amount, the same note may be generated.)

- Type2,3,4,5:

Controlling x same note probability. (0V: different note a.k.a. normal behavior. 2.5V: 50% the same note. 5V: always the same note.)

- Type4,5:

Controlling x slew amount. (0V: no slew. 5V: large slew amount.)

- notes:

Type2,3,4,5 replace some cv in or knob to alternative setting (x same note probability or x slew amount).
When the knob is replaced, the cv in works as original function (x steps or x bias).
When the cv in is replaced, the knob works as original function (x steps or x bias).


## deja vu loop buffer size mode

We can increase deja vu loop length.

The panel's marking value of LENGTH knob may differ from actual value.

If deja vu loop start position setting enabled, additional step (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) is also selectable.

The deja vu loop buffer size mode setting is saved to non volatile memory.

1. Type1 (Jthrw's)

  - 128 (1,2,3,4,5,6,7,8,10,12,14,16,24,32,48,64,96,128)

2. Type2 (base 2)

  - 128 (1,2,4,8,16,32,64,128)

3. Type3 (base 3)

  - 192 (1,3,6,12,24,48,96,192)

4. Type4 (base 5)

  - 320 (1,5,10,20,40,80,160,320)

5. Type5 (fibonacci)

  - 233 (1,2,3,5,8,13,21,34,55,89,144,233)

## Save/Load loop sequence in volatile memory (RAM)

We can explore random rhythm and return to the previous discovered random rhythm.

Play scenario is:

1. Explore random rhythm.
2. Lock the rhythm by setting "deja vu knob" to 50%(12 o'clock) with deja vu enabled.
3. (NEW FUNCTION) Save the rhythm
  - 3a. long-press "T rate switch".
  - 3b. then LEDs run animation. (and detouch from the switch.) (It represents currently in "alternative settings" mode.)
  - 3c. then press "T deja vu switch".
  - 3d. then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents saving is executed and "alternative settings" mode exited.)
4. Explore another random rhythm by changing "deja vu knob" or switching deja vu enabled-disabled.
5. Lock the rhythm by setting "deja vu knob" to 50% with deja vu enabled.
6. (NEW FUNCTION) Load the previous rhythm
  - 6a. long-press "T rate switch".
  - 6b. then LEDs run animation. (and detouch from the switch.) (It represents currently in "alternative settings" mode.)
  - 6c. then press "X deja vu switch".
  - 6d. then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents loading is executed and "alternative settings" mode exited.)

Notes:

The knobs position values aren't saved/loaded.

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

### Enter "alternative settings" mode

In the normal mode,

- long-press "T rate switch".

  (In alternative settings mode, t deja vu LED, x deja vu LED and X external processing mode LED are animating.)

### Exit/Cancel "alternative settings" mode

- press "X external processing mode button".

### Save timing of alternative settings

Saving alternative settings (X external clock input mode, T jitter cv input mode, X spread cv input mode, T rate cv input mode) to non volatile memory will be done automatically when exiting/canceling alternative settings mode.

### Resetting operation for returning all alternative settings value to default value

  In alternative settings mode,

  - long-press "X external processing mode button".
  
  Then "T rate LED" becomes fast-blink in a short moment and stop blink. (It represents "alternative setting edit" mode exited.) Then all alternative settings values return to default value.

### Policy of this alternative firmware

- It is based on latest official firmware. (So superlock feature is included.)

- Using the alternative firmware with all normal setting, the behavior is the same as latest official firmware.

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
