#include "mbed.h"
#include "MQ2.h"
DigitalOut led(D2);   
PwmOut PWM1(A5);
int on = 1, off = 0;   

Serial pc(USBTX, USBRX);                                                        // tx, rx
MQ2 mq2(A1);                                                                    // Analog Port to read from
int main() 
{
    PWM1.period_ms(500); 
    int x; x=1;
    pc.baud(115200);
    mq2.begin();                                                                // 'Calibrate' sensor
    MQ2_data_t MQ2_data;                                                        // Structure to hold data.
    while (true) 
    {
        pc.printf("CO PPM: %.0f\r\n",mq2.readCO());                             // Read of CO
        pc.printf("Smoke PPM: %.0f\r\n",mq2.readSmoke());                       // Read of Smoke
        pc.printf("LPG PPM: %.0f\r\n",mq2.readLPG());                           // Read of LPG
        wait(.1);
        pc.printf("................................\r\n");                      
      mq2.read(&MQ2_data);                                                      // Alt reading method, reading to structure
        pc.printf("CO PPM: %.0f\r\n",MQ2_data.co);                              // Return data from structure
        pc.printf("Smoke PPM: %.0f\r\n",MQ2_data.smoke);                        // Return data from structure
        pc.printf("LPG PPM: %.0f\r\n",MQ2_data.lpg);                            // Return data from structure
        pc.printf("................................\r\n");                      
        wait(.1);
        if (MQ2_data.co > 300)
         {
            led = on; 
            PWM1.pulsewidth_ms(x); 
            x=x+1; 
            wait(.1); 
         }        
        else   
         { 
         x=0;
         PWM1.pulsewidth_ms(x);
         led = off;
         }
    }
}