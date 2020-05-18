#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define BYTE0_HOURS        0x00   
#define BYTE1_MINUTES      0x01
#define BYTE2_SECONDS      0x02

#define BYTE3_HOURS        0x03
#define BYTE4_MINUTES      0x04

#define BYTE5_AMOUNTS      0x05


#define ONEDAY_ON_SECONDS      86400
#define ONE_HOUR_IN_SECONDS    3600  

/* ================================================================================================  */
/* ||                                   Frame 1 Down-Link                                         || */
/* ================================================================================================  */
/* ||  Byte0   ||  Byte1   ||  Byte2   ||  Byte3   ||   Byte4  ||  Byte5   ||  Byte6   ||  Byte7  || */
/* ================================================================================================  */
/* ||   hour   ||   Min    ||   Sec    ||   hour   ||   Min    ||  Frequ   ||          || Frame 1 || */
/* ================================================================================================  */
/* ||         Current datetime         ||time-hour first trans ||                                 || */
/* ================================================================================================  */

uint8_t Frame_2[8]= {16,19,9, 2,15, 1 ,2,1};


/*
 **  ============ Convert seconds to Hours, Minutes, seconds ==================
 */
void Seconds2HMS(uint32_t seconds) {
    uint8_t hour, minu, seco;
    if(seconds >= 3600) hour = (uint8_t)(seconds/3600); else hour = 0;              printf("%u:",hour);
    if(seconds >= 60) minu = (uint8_t)((seconds-(hour*3600))/60); else minu = 0;    printf("%u:",minu);
    seco = (uint8_t)(seconds-((hour*3600)+(minu*60)));                              printf("%u",seco);
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
 **  =============== function that shows the hours to transmit ==================
 */
void TransmissionHours(uint32_t updateTime, uint32_t FreqTransm, uint32_t Amounts) {
    uint8_t i;
    
    Seconds2HMS(updateTime); printf(" || ");
    for(i = 2; i <= Amounts; i++) {
        if((updateTime = updateTime + FreqTransm) >= ONEDAY_ON_SECONDS) {
            updateTime = updateTime - ONEDAY_ON_SECONDS;
        }
        Seconds2HMS(updateTime); printf(" || "); 
    }
}

/*
 **  ============ Calculate how much time for the first transmission ==================
 */
uint32_t FirstTransmission(int32_t CurrentTime, int32_t updateTime, uint32_t FreqTransm, uint32_t Amounts) {
    uint32_t i;

    if(updateTime > CurrentTime) {
        for(i = 0; i <= Amounts; i++) {
            if((updateTime -= FreqTransm) < CurrentTime) { 
                updateTime += FreqTransm; break; }
        }
    }
    else if (updateTime < CurrentTime) {
        for(i = 0; i <= Amounts; i++) {
            if((updateTime = updateTime + FreqTransm) >= CurrentTime) break;
        }  
    }   
    Seconds2HMS(updateTime);
    return (updateTime - CurrentTime);
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
    uint32_t CurrentTime, FirsTransm, FreqTransm, updateTime;
    
    CurrentTime = Time2Seconds(Frame_2[BYTE0_HOURS],Frame_2[BYTE1_MINUTES],Frame_2[BYTE2_SECONDS]);
    updateTime = Time2Seconds(Frame_2[BYTE3_HOURS], Frame_2[BYTE4_MINUTES], 0); 
    FreqTransm = TransmissionFrequencyCalculation(Frame_2[BYTE5_AMOUNTS]);
    
    printf("Veces que el dispositivo transmitirá al día: %u \n", Frame_2[BYTE5_AMOUNTS]);
    printf("Empezara a transmitir a partir de las: "); Seconds2HMS(updateTime); printf("\n");
    printf("Horas en el día en que el dispositivo transmitirá: ");
    
    TransmissionHours(updateTime, FreqTransm, Frame_2[BYTE5_AMOUNTS]);         printf("\n");
    
    printf("Hora Acual: "); Seconds2HMS(CurrentTime);                          printf("\n");
    printf("Hora de la proxima transmision: ");                
    FirsTransm = FirstTransmission(CurrentTime, updateTime, FreqTransm, Frame_2[BYTE5_AMOUNTS]);    printf("\n\n");

    printf("\n ===================================================================================================\n");
    printf("||  Hora actual en segundos:            %u s - ",CurrentTime); Seconds2HMS(CurrentTime);  printf("\n");
    printf("||  Hora de transmision                 %u s - ",updateTime);  Seconds2HMS(updateTime);   printf("\n");
    printf("||  Frecuencia de transmision:          %u s - ",FreqTransm);  Seconds2HMS(FreqTransm);   printf("\n");
    printf("||  Tiempo para la primera tranmision:  %u s - ",FirsTransm);  Seconds2HMS(FirsTransm);   printf("\n");
    printf(" ====================================================================================================");

//    printf("\n\n");
//    Seconds2HMS(72765);  printf("\n\n");
//    Seconds2HMS(72300);  printf("\n\n");
//    Seconds2HMS(338);    printf("\n\n"); 
//    Seconds2HMS(211); 
    return (EXIT_SUCCESS);
}