
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define BYTE7_HOURS        0x00   
#define BYTE6_MINUTES      0x01
#define BYTE5_SECONDS      0x02
#define BYTE4_AMOUNTS      0x03

#define ONEDAY_ON_SECONDS      86400
#define ONE_HOUR_IN_SECONDS    3600  

// ===============================================================================
//    Byte7     Byte6    Byte5    Byte4     Byte3     Byte2     Byte1    Byte0
//  || hour  ||  Min  ||  Sec  || Amount || On/Off || T_Alarm||       ||       ||
// ===============================================================================

uint8_t FrameDL[8]={17,33,44,96, 4,3,2,1};

/*
 **  ============ Convert seconds to Hours, Minutes, seconds ==================
 */
void Seconds2HMS(uint32_t seconds){
    uint8_t hour, minu, seco;
    if(seconds >= 3600) hour = (uint8_t)(seconds/3600); else hour = 0;              printf("%u:",hour);
    if(seconds >= 60) minu = (uint8_t)((seconds-(hour*3600))/60); else minu = 0;    printf("%u:",minu);
    seco = (uint8_t)(seconds-((hour*3600)+(minu*60)));                              printf("%u \n",seco);
}
/*
 **  =============== Time to Seconds ==================
 */
uint32_t Time2Seconds(uint8_t hour, uint8_t minutes, uint8_t Seconds) {
    return(uint32_t)((hour*3600)+(minutes*60)+(Seconds));
}
/*
 **  =============== Calculate how many hours you will send ==================
 */
uint32_t TransmissionFrequencyCalculation(uint8_t NumofTime) {
    if(NumofTime > 0) return (uint32_t)(ONEDAY_ON_SECONDS/NumofTime);
    else return -1;
}
/*
 **  ============ Calculate how much time for the first transmission ==================
 */
uint32_t FirstTransmission(uint32_t CurrentTime, uint32_t TrnFreqCal, uint8_t NumofTime){
    uint8_t i;  uint32_t ret = 0;
    
    for(i=0; i <= NumofTime; i++){
        if((ret = TrnFreqCal*i) >= CurrentTime){ printf("%u\n",ret); return (uint32_t)(ret-CurrentTime);} 
    }
}
/*
 **  ============ Calculate next transmission ==================
 */
uint32_t NextTransmissonCalculate(uint8_t NumofTime, uint32_t CurrentTime){
    uint32_t TrnFreqCal, ret = 0;  uint8_t i;
    
    if(NumofTime == 0) return -1;
   
    TrnFreqCal = (uint32_t)(ONEDAY_ON_SECONDS/NumofTime);
    for(i=0; i <= NumofTime; i++){
        if((ret = TrnFreqCal*i) >= CurrentTime){ printf("%u\n",ret); return (uint32_t)(ret-CurrentTime);}
    }
}

/*
 **  ============ Integer division ==================
 */

uint32_t division(uint32_t Dividend, uint32_t Divisor) { 
    uint32_t quotient,r = Dividend;
    for(quotient = 0; r > Divisor; quotient++) {
        r = r - Divisor;
    }
    return quotient;
}

int main(int argc, char** argv) {
    uint32_t CurrentTime, FirsTrans, TranTime;
    
    CurrentTime = Time2Seconds(FrameDL[BYTE7_HOURS],FrameDL[BYTE6_MINUTES],FrameDL[BYTE5_SECONDS]);
    TranTime = TransmissionFrequencyCalculation(FrameDL[BYTE4_AMOUNTS]);
    FirsTrans = FirstTransmission(CurrentTime, TranTime, FrameDL[BYTE4_AMOUNTS]);
    
//    FirsTrans = NextTransmissonCalculate(FrameDL[BYTE4_AMOUNTS] ,CurrentTime);
    
    Seconds2HMS(CurrentTime);
    printf("%u \n",division(CurrentTime, ONE_HOUR_IN_SECONDS));
    
    printf("\nHora actual en segundos:         %u s - ",CurrentTime);     Seconds2HMS(CurrentTime);
    printf("Frecuencia de transmision:         %u s - ",TranTime);        Seconds2HMS(TranTime);
    printf("Tiempo para la primera tranmision: %u s - ",FirsTrans);       Seconds2HMS(FirsTrans);   
    
    return (EXIT_SUCCESS);
}

