# gate
lindsay's gate control.

The two arduino files here control an RFM69B and a mosfet to trigger some lights when the front gate is operational.

## circuits
The design is simple, communications is via a pair of Sparkfun RFM69B units, the gate unit uses a resistive voltage divider to drop 12V to 3.3V without much protection. The lights unit uses a jaycar arduino mosfet breakout to switch the 12V for the lights.

The lack of protection on the circuits is because they are battery powered and shouldn't exceed around 14V. The circuits are designed to run at 18V indefinitley and at 24V for short periods.

The power supplies used are pololu buck switching power supplies that will work up to 30V.

## Wiring
The cases are marked with the following stickers
* ANT - antenna
* SPN - Signal (gate 12V) / Positive 12V / Neutral
* PN - Positive 12V / Neutral
* 12V LOAD - The lights load circuit

