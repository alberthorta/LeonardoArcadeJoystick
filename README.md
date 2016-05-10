# LeonardoArcadeJoystick

Leonardo / Arduino Micro / Pro Micro code to control an Arcade Stick with advanced features.

NOTE: This code needs the library https://github.com/MHeironimus/ArduinoJoystickLibrary to work.

## Mapped pins

### Stick Normal Buttons

| Pin | Function |
| --- | -------- |
| 0 | Button 0 |
| 1 | Button 1 |
| 2 | Button 2 |
| 3 | Button 3 |
| 4 | Button 4 |
| 7 | Button 5 |
| 8 | Button 6 |
| 9 | Button 7 |
| 10 | Button 8 |
| 11 | Button 9 |
| 12 | Button 10 |
| 13 | Button 11 |

### Stick Pad

| Pin | Function |
| --- | -------- |
| A0 | Left |
| A1 | Right |
| A2 | Up |
| A3 | Down |

### Special Buttons

| Pin | Function |
| --- | -------- |
| A4 | Button to SET the TURBO |
| A5 | Button to UNSET the TURBO |

### How special buttons worked

If you want a button to use the TURBO mode (repeat the pressing) you should click on the SET button (A4) and then click to the button you want to set into TURBO mode.

If you want to reset a TURBO mode from any button, just click on the UNSET button (A5) and then press on the button you want to unset.