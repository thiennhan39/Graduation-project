#include <16F877A.h>
#use delay(crystal=20000000)
#device ADC=10
#define LCD_ENABLE_PIN PIN_D0             
#define LCD_RS_PIN PIN_D1                                                   
#define LCD_RW_PIN PIN_D2                          
#define LCD_DATA4 PIN_D4                                                          
#define LCD_DATA5 PIN_D5                                    
#define LCD_DATA6 PIN_D6                                                      
#define LCD_DATA7 PIN_D7   

#define MFRC522_CS  PIN_C2                 
#define MFRC522_SCK PIN_C3
#define MFRC522_SI  PIN_C4                           
#define MFRC522_SO  PIN_C5             
#define MFRC522_RST PIN_C6   
