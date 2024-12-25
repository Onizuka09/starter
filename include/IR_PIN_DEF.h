#ifndef CONF_H
#define CONF_H
#define IR_SEND_PIN                4
#define IR_RECEIVE_PIN          16  // D15

#define DECODE_NEC          // Includes Apple and Onkyo. To enable all protocols, just comment/disable this line.
#define _MICROS_PER_TICK 50

#define SEND_PWM_BY_TIMER // We do not have pin restrictions for this CPU's, so lets use the hardware PWM for send carrier signal generation




/*
 * Helper macro for getting a macro definition as string
 */
#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif
// Structure to hold IR data for Pulse Distance, Samsung, and LG protocols

#endif