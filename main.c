#include <xc.h>
#pragma config FOSC_FOSFPR = FRC_PLL16     // FRC w/PLL 16x 
#pragma config FOSC_FCKSMEN = CSW_FSCM_OFF // Sw Disabled, Mon Disabled 
#pragma config FICD_ICS = ICS_PGD         // Use PGC/EMUC and PGD/EMUD
#pragma config FWDT_WDT = WDT_OFF         // Disable watchdog

int numberOfInterrupts = 0;
int capture;

void __attribute__((interrupt)) _IC2Interrupt(void) {
    // Reset the interruption flag:
    IFS0bits.IC2IF = 0;  

    // Extract all available input captures:
    while(IC2CONbits.ICBNE) {
        capture = IC2BUF;  // 
    }

    // Count the number of interruptions:
    numberOfInterrupts++;
}

void initialization() {
    // IC2/INT2/RD9 has to be an input:
    TRISDbits.TRISD9 = 1;

    // Configure timer 2, because we want to use it with input capture
    T2CONbits.TCKPS = 0b11; // 1:256 pre-scale value
    T2CONbits.TCS = 0;      // Internal clock (FOSC /4)
    T2CONbits.TON = 1;      // Starts 16-bit timer
    
    // Enable Input Capture:
    IC2CONbits.ICSIDL = 1;  // Input capture module will halt in CPU Idle mode
    IC2CONbits.ICTMR = 1;   // Timer 2 contents are captured on capture event
    IC2CONbits.ICI = 0;     // Interrupt on every capture event
    IC2CONbits.ICM = 0b010; // Capture mode, every falling edge
    
    // Enable Input Capture interrupts:
    IEC0bits.IC2IE = 1;     // Enable interrupts for Input Capture
    IPC1bits.IC2IP = 1;     // Input capture has the lowest priority interrupts.
    
    // Set CPU base level of interrupts:
    CORCONbits.IPL3 = 0;    // Any interruption with level higher...
    SRbits.IPL = 0;         // ... than 0 is serviced.
}

int main(void) {
    int c = 0;
    initialization();
    
    while(1) {
        c++;
    }
}