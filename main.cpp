
/***********************************************************************************************************************************************
*                                                                                                                                              *
*                                              Waveform Generator ( Embedded Software Assignment 1 )                                           *
* @author Markus                                                                                                                               *
* @version 1.0                                                                                                                                 *
* @lastupdate 27.01.2016                                                                                                                       *
*                                                                                                                                              *
* This program generates a waveform, and a synchronise pulse for an trigger, similar to the following example:                                 *
*                                                                                                                                              *
*                                           in one period or block,                   the end of a block constis of a default wait(b),         *
*                                   the signal a and wait b is exectued 20 times            and an additional waiting time (d)                 *
*                          _______         _________         _________         ________                                     _______            *
*                         |       |       |         |       |         |       |        |       |                     |     |       |           *
*    SIGNAL A             | <---> | <---> | <-----> | <---> | <-----> | <---> |  <-->  | <---> |         <--->       |     | <---> |           *
*     (wave)              |   a   |   b   |  a+i*50 |   b   |  a+i*50 | b...  |  a...  |   b   |           d         |     |   a   | .....     *
*                  _______|       |_______|         |_______|         |_______|        |_______|_____________________|_____|       |__________ *
*                         |                              --> repeated in sum 20 times                                |                         *
*                         |                                                            |_____________________________|________________________ *
*                         |                                                                      end pause           | begin of the next block *
*                    _____|                                                                                          |_____                    *
*     SIGNAL B      |     |      <--------------------------------------------->       |                             |     |                   *
*     (trigger)     | <-> |                            c                               |                             | <-> |                   *
*                  _|  z  |____________________________________________________________|_____________________________|  z  |__________________ *
*                                                                                                                                              *
*                                                                                                                                              *
*                                                                                                                                              *
*                                                                                                                                              *
***********************************************************************************************************************************************/


/*
========================================================================================
=                                                                                      =
=                       INCLUDES and DIGITAL PORT DECLARATION                          =
=                                                                                      =
========================================================================================
*/

#include "mbed.h"                                    // Include the default mbed header file

DigitalOut sig_a(p18);                               // Digital Output for the Wave Signal
DigitalOut sig_b(p17);                               // Digital Output for the Trigger pulse
DigitalIn sw_onoff(p21);                             // Digital Input for the ON / OFF Switch
DigitalIn sw_inv(p22);                               // Digital Input for the Inverted Switch

/*
========================================================================================
=                                                                                      =
=                       MAIN PROCEDURE - generating the Blocks                         =
=                                                                                      =
========================================================================================
*/

int main()
{
    int var_z = 10;                                  // Signal B - Trigger Pulse in us
    int var_c = 7   +   13;                          // Letter M / number of pulses in one block
    int var_d = 500 *   16;                          // Letter P / number of space between the blocks
    int multiplier = 50;                             // is added to each pulse lenth after every pulse

    while(1) {                                       // repeat the signal procedure forever

        if(sw_onoff) {                               // if the ON/OFF Switch is ON, proceede, else do nothing

            sig_b = 1;                               // Send the Trigger pulse (set port B high)
            wait_us(var_z);                          // wait for var_z us
            sig_b = 0;                               // Stop the Signal B (set port B low)

            int var_a = 100 *   12;                  // Letter L / the pulse length in us;
            int var_b = 100 *   1;                   // Letter A / wait us between the pulses


            if(sw_inv) {                             // if the Inverted Switch is ON, proceede, else jump over this
                                                     // this inverts the values of var_a and var_b
                int temp = var_a;                    // save var_a to a temporary variable
                var_a = var_b;                       // replace var_a with var_b
                var_b = temp;                        // replace var_b with temp (value of var_a)
            }

                                                      // This for loop generates the BLOCKS
            for (int i=0; i<var_c; i++) {             // run this loop var_c times
                sig_a = 1;                            // Start the Signal A (set port A to high)
                wait_us(var_a + i * multiplier);      // wait for var_a + i * multiplier us
                sig_a = 0;                            // Stop the Signal A (set port A to low)
                wait_us(var_b);                       // wait for var_b us (wait between the pulses)
            }
            
            wait_us(var_d);                           // wait vor var_d us (final wait)
        }
    }
}