#include <main.h>
#include <lcd.c>
#include <Built_in.h>
#include <stdlib.h>
#include <stdio.h>

#use rs232(baud=115200,parity =N, xmit=pin_c6,rcv=pin_c7,bits=8) 

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used FOR I/O


#define BUFFER_SIZE 11
BYTE buffer[32];
BYTE next_in = 0;
BYTE next_out = 0;

unsigned int8 bdt;
//Ham ngat timer1
#INT_TIMER1
void ngat()
{
   bdt++;
   set_timer1(3036);
//!   if(bdt>100)
//!   bdt = 100;
  
}

//

#int_rda
void serial_isr() {
   int t;

   buffer[next_in]=getc();
   t=next_in;
   next_in=(next_in+1) % BUFFER_SIZE;
   if(next_in==next_out)
     next_in=t;           // Buffer full !!
}

#define bkbhit (next_in!=next_out)

BYTE bgetc() {
   BYTE c;

   while(!bkbhit) ;
   c=buffer[next_out];
   next_out=(next_out+1) % BUFFER_SIZE;
   return(c);
}


 unsigned char getkey()
{

output_low(col1);output_high(col2);output_high(col3);output_high(col4);
if(!input(row1)) delay_ms(100); while (!input(row1)) return 'D';
if(!input(row2)) delay_ms(100); while (!input(row2)) return 'C';
if(!input(row3)) delay_ms(100); while (!input(row3)) return 'B';
if(!input(row4)) delay_ms(100); while (!input(row4)) return 'A';
 
output_high(col1);output_low(col2);output_high(col3);output_high(col4);
if(!input(row1)) delay_ms(100); while (!input(row1)) return '#';
if(!input(row2)) delay_ms(100); while (!input(row2)) return '9';
if(!input(row3)) delay_ms(100); while (!input(row3)) return '6';
if(!input(row4)) delay_ms(100); while (!input(row4)) return '3';

output_high(col1);output_high(col2);output_low(col3);output_high(col4);
if(!input(row1)) delay_ms(100); while (!input(row1)) return '0';
if(!input(row2)) delay_ms(100); while (!input(row2)) return '8';
if(!input(row3)) delay_ms(100); while (!input(row3)) return '5';
if(!input(row4)) delay_ms(100); while (!input(row4)) return '2';

output_high(col1);output_high(col2);output_high(col3);output_low(col4);
if(!input(row1)) delay_ms(100); while (!input(row1)) return '*';
if(!input(row2)) delay_ms(100); while (!input(row2)) return '7';
if(!input(row3)) delay_ms(100); while (!input(row3)) return '4';
if(!input(row4)) delay_ms(100); while (!input(row4)) return '1';
 

 
 return 'n';
 
}


int1 kt_the (char UID[])
{  int i;
   for (int j=6;j<=20;j=j+5)
   {
      for ( i=0;i<5;i++)
      {
         if (read_eeprom(i+j)!= UID[i])
            break;
      }   
      if (i==5) 
         break;
   } 
   if (i==5)
   return 1;
   else 
   return 0;
   
}

void them_the(char UID[])
{
   int i,j;
   for ( j=6;j<=20;j=j+5)
      {
         for ( i=0;i<5;i++)
         {
            if(read_eeprom(i+j)==0xFF)
            {
               write_eeprom(i+j, UID[i]);
            }
            else
               break;
         }
         if (i==5) 
            break;
      }
}

void xoa_the(char UID[])
{
    int i,j;
   for (j=6;j<=20;j=j+5)
   {
      for ( i=0;i<5;i++)
      {
         if (read_eeprom(i+j)!= UID[i])
            break;
      }   
      if (i==5)
      {
         for (i=0;i<5;i++)
            write_eeprom(i+j, 0xFF);
      }
      if(i==5)
         break;
   }
}

void bipbip(unsigned int8 hoi,unsigned int8 tieng) 
{ 
   unsigned int8 i, j;
   for(i = 0; i < hoi; i ++){
      for(j = 0; j < tieng; j ++){
         output_high(pin_c0);
         delay_ms(1);
      }
      output_low(pin_c0);
      delay_ms(10);
   }
}

int1 door=0;
char mk1,mk2,mk3,mk4;

void kt_pass(void)
{
   int e,f,g,h,i=0;
   for(;i<=2;i++)
   {
   do
   {
   do
   {
      do
      {
         do
         {
               lcd_putc('\f');
               lcd_gotoxy(1,1);
               lcd_putc("PASS:");
               lcd_gotoxy(4,2);
            do
            {
               getkey();
               e=getkey();
            }
            while (e=='n');
               lcd_putc(e);
            do
            {
               getkey();
               f=getkey();
            }
            while (f=='n');
               lcd_putc(f);
         }
         while (f=='#');
         do
         {
            getkey();
            g=getkey();
         }
         while (g=='n');
            lcd_putc(g);
      }
      while (g=='#');
      do
      {
         getkey();
         h=getkey();
      }
      while (h=='n');
         lcd_putc(h);
         
   }
   while (f=='#');
   // Nhan enter//
          do
      {
         getkey();
         
      }
      while ( getkey()!='*' && getkey()!='#');
         //
  }
   while (getkey()=='#');
  if (e==mk1 && f==mk2 && g==mk3 && h==mk4)
      {
      door=1;
      break;
      }
   }
   if (i==3)
   {
      lcd_putc('\f');
      do 
      {  
         lcd_gotoxy(2,1);
         lcd_putc("WRONG PASSWORD");
          bipbip(10,10);
      }
      while (getkey()!='#');
   }
}


void main()
{
setup_timer_1(t1_internal|t1_div_by_8);
enable_interrupts(INT_TIMER1);
enable_interrupts(INT_RDA);
enable_interrupts(GLOBAL);
set_timer1(3036);
MFRC522_Init ();
lcd_init(); 
lcd_putc('\f');
//!lcd_putc("Connecting Wifi");
//!otp_init();
//!
int a='1',b='1',c='1',d='1',m;
CHAR UID[6];
UNSIGNED int TagType;  
char str [10];
//!   int16 adc_value, vout_value;
//!   float voltage_value, current_value; 
//!   setup_adc_ports( AN0 );
//!   setup_adc(ADC_CLOCK_INTERNAL );  
//!   set_adc_channel( 0 );
//!///
delay_ms(3000);


   

///
   while(TRUE)
   {
door = 0;
output_low(PIN_D3);
 printf("CLOSE");

lcd_putc('\f');
//!  do
//!       {   
//!         
//!          output_low(PIN_D3);
//!           delay_ms(500);
//!           adc_value = read_adc();
//!            lcd_gotoxy(1,1);
//!              lcd_putc("CUA CHUA DONG");
//!        }
//!        while (adc_value<510);
do
   {  
        door = 0;
        output_low(PIN_D3);
       
   do   
   {
   
    lcd_putc('\f');  
      getkey();
      m=getkey();
//!      lcd_gotoxy(4,1);
      //Xac nhan d�ng//
//!      delay_us(100);
//!      adc_value = read_adc();
   //!      if( adc_value>495)
   //!      {
   //!         printf("DONG");
   //!      }
//!    lcd_gotoxy(3,2);
//!    printf(lcd_putc, "%.2f  ",  (float)adc_value);
    //
   //!   lcd_putc("\f  HELLOOOO");
//!      lcd_gotoxy(1,2);
//!      lcd_putc(" press 1 to enter pass");


// N�T CHU�NG///
   if (input(PIN_A2)==1)
   {
   printf("BELL");
    lcd_gotoxy(3,2);
    lcd_putc("CHUONG");
   delay_ms(2000);
   }
///
 if (input(PIN_A0)==1)
 door=1;


   //ESP32//
   
         if (bkbhit)
         {
          for (int i=0; i<11;i++)
         {
             if (bkbhit)
         {
        
            str[i]=bgetc();
            
   //!         printf(str[i]);
            
            lcd_putc(str[i]);
            }
      }
   }
          lcd_gotoxy(1,1);
          lcd_putc(str[0]); lcd_putc(str[1]); lcd_putc(str[2]); lcd_putc(str[3]);lcd_putc(str[4]);
          lcd_putc(str[5]); lcd_putc(str[6]); lcd_putc(str[7]);lcd_putc(str[8]);lcd_putc(str[9]);lcd_putc(str[10]);
//!        
      if(str[1]>=0x30&&str[1]<=0x39&&str[2]>=0x30&&str[2]<=0x39)
      {
         mk1=str[1];mk2=str[2];mk3=str[3];mk4=str[4];
      }
      
      if(str[5]=='O'&&str[6]=='P'&&str[7]=='E'&&str[8]=='N'&&str[9]=='N')
      {
         door=1;
         str[5]=='C'&&str[6]=='L'&&str[7]=='O'&&str[8]=='S'&&str[9]=='E';
      }
   
   //
   
   IF (MFRC522_isCard (&TagType)) //Check any card
         {                                           
            //Read ID 
            IF (MFRC522_ReadCardSerial (&UID))             
            {
               
               if( kt_the(UID))
               { 
                     bipbip(3,3);
                     printf(LCD_PUTC, "\f the hop le");
                     lcd_gotoxy(0,2);
                     printf(LCD_PUTC, "xin moi vao");
                     delay_ms(1000);
                     door = 1;
               }
               else
               {
                  lcd_putc('\f');
                  lcd_gotoxy(0, 1);
                  printf (LCD_PUTC, "SAI THE");
                  lcd_gotoxy(4, 2);
                  printf (LCD_PUTC, "WARNING!!!");
                  bipbip(20,10);
                  lcd_putc('\f');
                  door = 0;
               } 
            }
        }
        
   if (m =='1')
   {
   lcd_gotoxy(1,2);
   lcd_putc(str[0]); lcd_putc(str[1]); lcd_putc(str[2]); lcd_putc(str[3]);
   mk1=read_eeprom(1);
   mk2=read_eeprom(2);
   mk3=read_eeprom(3);
   mk4=read_eeprom(4);
   kt_pass();
   }
   if (m=='2'&&str[1]!='F')
   {
   //!   otp();
   //!
   //!    lcd_gotoxy(1,2);
   //!    lcd_putc(mk1); lcd_putc(mk1); lcd_putc(mk1); lcd_putc(mk1);
      
      kt_pass();
   }
   }
   while ( door!= 1 );
   
   output_high(PIN_D3);
     
   lcd_putc("\f");
   delay_ms(2000);
   output_low(PIN_D3);
   }
while(input(PIN_A5)==0);
//Gui xac nhan///
printf("OPEN");
//Bat dau dem thoi gian//
bdt = 0;
do
{
bdt = 0;
do
{  
   getkey();
   m=getkey();
//!   lcd_gotoxy(1,1);
//!   lcd_putc("1. DOI PASS");
   lcd_gotoxy(1,2);
   lcd_putc("2. KHOA   3.RFID");
   //ACS712//
//!    delay_us(100);
//!      adc_value = read_adc();
//!      if( adc_value<495)
//!      {
//!         printf("OPEN");
//!      }
//!    lcd_gotoxy(2,1);
//!    printf(lcd_putc, "%.2f  ",  (float)adc_value);
//!    delay_ms(100);
   //
   if(bdt>100)
   {
      if (input(PIN_A5)==1)
      {
         bipbip(50,10);
         bdt=0;
         lcd_gotoxy(1,1);
         lcd_putc("CUA DANG MO");
      }
      else
      {
         door = 0;
      }
   }
}
while (m!='1'&& m!='2' && m!='3' && door==1);
 

if (m=='1')
{
   lcd_putc('\f');
   lcd_putc("Nhap mat khau");
   delay_ms(1000);
   lcd_putc('\f');
do
{
   getkey();
   a=getkey();
}
while (a=='n');
   lcd_putc(a);
write_eeprom(1,a);
do
{
   getkey();
   b=getkey();
}
while (b=='n');
   lcd_putc(b);
write_eeprom(2,b);
do
{
   getkey();
   c=getkey();
}
while (c=='n');
   lcd_putc(c);
write_eeprom(3,c);
do
{
   getkey();
   d=getkey();
}
while (d=='n');
   lcd_putc(d);
   write_eeprom(4,d);
   delay_ms(1000);
   lcd_putc('\f');
   lcd_putc("Press 1 to block");
do
{
   getkey();
   m=getkey();
}
while (m!='1');

   bdt=50;
   
}

 else if (m=='3')
 {
   lcd_putc("\f1. Them the");
   lcd_gotoxy(1,2);
   lcd_putc("2. Xoa the");
   do
   {
      getkey();
      m=getkey();
   }  
   while (m!='1' && m!='2');
   if(m=='1')
   {
      lcd_putc("\f Quet the moi");
      do{}
      while (MFRC522_isCard (&TagType)!=1); //Check any card
      MFRC522_ReadCardSerial (&UID);
      bipbip(10,10);
      if (kt_the(UID))
      {
         lcd_gotoxy(1,1);
         lcd_putc("\f The da them");
      }
      else
      {
         them_the(UID);
         lcd_gotoxy(1,1);
         lcd_putc("\f XONG");   
      }
   }
   else 
   {  
      lcd_putc("\f Quet the can xoa");
      do{}
      while (MFRC522_isCard (&TagType)!=1); //Check any card
      MFRC522_ReadCardSerial (&UID);
      bipbip(10,10);
      if (kt_the(UID))
      {
         xoa_the(UID);
         lcd_putc("\f Da xoa");
      }
      else
         lcd_putc("\fKhong co the nay");
   }
   lcd_gotoxy(1,2);
   lcd_putc("Press 1 to block");
do
{
   getkey();
   m=getkey();
}
while (m!='1');
 bdt = 50;
 }
 
}
    while(door==1);
    
      //TODO: User Code
   }

}

