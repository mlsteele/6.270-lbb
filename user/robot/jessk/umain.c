
#define ENCODER_PIN 25

// Include headers from OS
#include <joyos.h>
#include <encoder_utils.h>

// usetup is called during the calibration period. 
int usetup (void) {
    return 0;
}

// Entry point to contestant code.
int umain (void) {
	printf("\nDistance: %d", encoder_distance());
	printf("\nSpeed: %d", encoder_speed());
    // YOUR CODE GOES HERE

    // Will never return, but the compiler complains without a return
    // statement.
    return 0;
}
