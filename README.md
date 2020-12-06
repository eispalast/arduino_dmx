# Ardulight

- [Ardulight](#ardulight)
  - [What is it good for?](#what-is-it-good-for)
  - [Inputs/Outputs](#inputsoutputs)
  - [Modes](#modes)
    - [Digital effects](#digital-effects)
## What is it good for?
You can control up to 6 analogue LED stripes and 1 digital LED stripe (like WS2811) via DMX. You can dim the analogue channels individually and acces multiple effects for the digital one.

## Inputs/Outputs
* DMX In (3 pin XLR male)
* DMX Out (3 pin XLR female)
* Power in 12 V DC up to 95W
* 6 analogue out 12V PWM dimmable
* digital out + constant 12 V

## Modes
You can use it in DMX mode or static function mode.
Use the leftmost dip switch to choose between the modes: 
* ON:  static function mode 
* OFF: DMX-mode.
  
There are currently two DMX modes (6 ch an 16 ch). 
The 6-channel-mode is meant to be used, when you only need the analogue channels. 

| channel | value   | function                |
| ------- | ------- | ----------------------- |
| 1       | 0-100   | Dim analogue 1          |
| 2       | 0-100   | Dim analogue 2          |
| 3       | 0-100   | Dim analogue 3          |
| 4       | 0-100   | Dim analogue 4          |
| 5       | 0-100   | Dim analogue 5          |
| 6       | 0-100   | Dim analogue 6          |
| 7       | 0-100   | digital primary red     |
| 8       | 0-100   | digital primary green   |
| 9       | 0-100   | digital primary blue    |
| 10      | 0-100   | digital secondary red   |
| 11      | 0-100   | digital secondary green |
| 12      | 0-100   | digital secondary blue  |
| 13      |         | **EFFECTS**             |
|         | 0-10    | OFF                     |
|         | 11-20   | analogue style          |
|         | 21-255  | Rising star             |
| 14      | 0-255   | Effect Speed (bpm)      |
| 15      |         | **BEAT TYPE**           |
|         | 0-25    | quarter note            |
|         | 26-36   | quarter dotted          |
|         | 37-72   | whole note              |
|         | 73-89   | half note               |
|         | 90-99   | half dotted             |
|         | 100-136 | quarter triplets        |
|         | 137-163 | eighth triplets         |
|         | 164-189 | eighth                  |
|         | 190-200 | eighth dotted           |
|         | 201-255 | sixteenth               |
| 16      | 0-255   | effect intensity        |

### Digital effects
| DMX value | Effect         | Explanation                                                                                                 | Intensity |
| --------- | -------------- | ----------------------------------------------------------------------------------------------------------- | --------- |
| 0-10      | OFF            | All lights are turned off -                                                                                 | -         |
| 11-20     | Analogue Style | The strips acts like an analogue strip. Use the primary colors (ch.7 -ch.9) to mix the color and brightness | -         |
| 21-255    | Rising star    | A star with primary color shoots from start to end. Has a trail of secondary color behind it. Variable speed  | length of trail|

