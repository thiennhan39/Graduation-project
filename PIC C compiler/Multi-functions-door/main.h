#include <16F877A.h>
#device ADC=10
#use delay(crystal=20000000)

#define ROW1  PIN_B0
#define ROW2  PIN_B1
#define ROW3  PIN_B2
#define ROW4  PIN_B3
#define COL1  PIN_B4
#define COL2  PIN_B5
#define COL3  PIN_B6
#define COL4  PIN_B7


#define MFRC522_CS  PIN_C1                
#define MFRC522_SCK PIN_C2
#define MFRC522_SI  PIN_C3                           
#define MFRC522_SO  PIN_C4             
#define MFRC522_RST PIN_C5   
