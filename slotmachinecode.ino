// TODO- Bare metal definitions
#include <Wire.h>

#define MPU_addr 0x68  // I2C address of the MPU-6050
#define latch // storage register latch
#define ledDATA // data in
#define ledCLK // shift register clock

// Defining every line of the 7 segment display 
#define SEGA  B00000001  // Letter a of segment 
#define SEGB  B00000010  // Letter b of segment 
#define SEGC  B00000100  // Letter c of segment 
#define SEGD  B00001000  // Letter d of segment 
#define SEGE  B00010000  // Letter e of segment 
#define SEGF  B00100000  // Letter f of segment 
#define SEGG  B01000000  // Letter g of segment 
#define SEGDP B10000000  //  dp of segment 
#define SEGOFF B00000000  //  off segment 

// define images for the 7Seg display
// In here the lines are combined to form letters letters and numbers
#define LED_OFF (SEGOFF)  // /* bitmap for the image 
#define LED_ZERO (SEGA|SEGB|SEGC|SEGD|SEGE|SEGF) // bitmap for the image
#define LED_ONE (SEGB|SEGC) // bitmap for the image
#define LED_TWO (SEGA|SEGB|SEGD|SEGE|SEGG)  //bitmap for the image 
#define LED_THREE (SEGA|SEGB|SEGC|SEGD|SEGG) //bitmap for the image
#define LED_FOUR (SEGF|SEGG|SEGB|SEGC) 
#define LED_FIVE (SEGA|SEGF|SEGG|SEGC|SEGD)
#define LED_SIX (SEGA|SEGF|SEGE|SEGD|SEGC|SEGG)
#define LED_SEVEN (SEGB|SEGC|SEGA)
#define LED_EIGHT (SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG)
#define LED_NINE (SEGA|SEGB|SEGC|SEGD|SEGF|SEGG)
#define LED_A (SEGA|SEGB|SEGC|SEGE|SEGF|SEGG)
#define LED_b (SEGF|SEGE|SEGD|SEGC|SEGG)
#define LED_C (SEGA|SEGF|SEGE|SEGD)
#define LED_d (SEGB|SEGC|SEGD|SEGE|SEGG)
#define LED_E (SEGA|SEGF|SEGE|SEGD|SEGG)
#define LED_F (SEGA|SEGF|SEGE|SEGG)

#define LED_SEVENUD (SEGB|SEGC|SEGD)

///////////////////////
// analog pins for the buttons
// PORT C maps arduino pins from A0 to A5, buttons are in pins A0,A1 & A2 so they are made high
#define _B1 B00000001 //button pin
#define _B2 B00000010 //button pin
#define _B3 B00000100//button pin
//#define notPRESSED 0
//#define partialPRESS 1
//#define normalPRESS 2
//#define B1_state
//#define B2_state 
//#define B3_state
///////////////////////
// Pin for  the RGBs are assigned. 
#define RGB_LED_R 9  // rgb red pin (pwm)
#define RGB_LED_B 10// rgb blue pin (pwm)
#define RGB_LED_G 11// rgb green pin (pwm)

//red amber and green LED pins
// PORT  D maps arduino pins from D0  to D7. All the leds are in pins from D2 to D7, so they are made high
#define mainRED B00000100
#define mainYELL B00001000
#define mainGREEN B00010000
#define secRED B00100000
#define secYELL B01000000 
#define secGREEN B10000000 


// This code is to turn on and off the LEDs
#define mainRED_HI      PORTD |=  mainRED 
#define mainRED_LO      PORTD &= ~mainRED  
#define mainGREEN_HI    PORTD |=  mainGREEN
#define mainGREEN_LO    PORTD &= ~mainGREEN
#define mainYELL_HI     PORTD |=  mainYELL
#define mainYELL_LO     PORTD &= ~mainYELL

#define secRED_HI      PORTD |=  secRED 
#define secRED_LO      PORTD &= ~secRED  
#define secGREEN_HI    PORTD |=  secGREEN
#define secGREEN_LO    PORTD &= ~secGREEN
#define secYELL_HI     PORTD |=  secYELL
#define secYELL_LO     PORTD &= ~secYELL
//



// LATCH,  DATA and CLOCK 
// this code  set the pins for latch,clock,data and latch
// the latchhigh, clocklow code is to turn on and off the latch, clock and  data
#define LATCH  B00001000
#define LATCHHIGH PORTC |= LATCH 
#define LATCHLOW PORTC &= ~LATCH    


#define CLOCK  B00010000
#define CLOCKHIGH PORTB |= CLOCK 
#define CLOCKLOW PORTB &= ~CLOCK    

#define DATA  B00000001
#define DATAHIGH PORTB |= DATA 
#define DATALOW PORTB &= ~DATA    





// An unsigned data type that occupies 1 byte of memory. Same as the byte datatype. The unsigned char datatype encodes numbers from 0 to 255.
unsigned char ledState; // state of the 7 segment display

//module1 variables   
// boolean  data type is a data type that allows two possible values, either true or false.    
//module5 variables    
 
bool init_module1_clock;

bool init_module5_clock;

//module6 variables    
bool init_module6_clock;

// accelerometer debounce module
bool init_moduleAcc_clock;   // written by me

#define debounce 300 //normal debounce duration

enum button_t { notPRESSED, partialPRESS, normalPRESS, heldPRESS, preDblClick, partialDblClick, DblClick, afterDblClick, Stuck };
button_t B1_state; // button B1 state variable
button_t B2_state; // button B2 state variable
button_t B3_state; // button B3 state variable


#define SW1_PRESSED  !(PINC & _B1)
#define SW1_RELEASED !SW1_PRESSED

#define SW2_PRESSED  !(PINC & _B2)
#define SW2_RELEASED !SW1_PRESSED

#define SW3_PRESSED  !(PINC & _B3)
#define SW3_RELEASED !SW1_PRESSED

enum accel_t { faceup,facedown, landscape, udlandscape, POTL, POTR, partialREAD };
accel_t PL_state;
accel_t PR_state;
accel_t LS_state;
accel_t UDLS_state;
accel_t FU_state;
accel_t FD_state;



// // An unsigned data type that occupies 1 byte of memory. Same as the byte datatype. The unsigned char datatype encodes numbers from 0 to 255.
// boolean  data type is a data type that allows two possible values, either true or false.  

bool init_module13_clock; // master duration module
bool init_module14_clock, init_module15_clock, init_module16_clock; // wheel 1, 2, 3
unsigned char w1State, w2State, w3State, credit;
unsigned int md1=0, md2=0, md3 = 0; // master duration variables
bool ew1=false, ew2=false, ew3=false; // wheel enable



void setup() {
  // you need to setup the bare metal port access for all input-output pins
  // the names of pins you need to consider here are
  // ledDATA, ledCLK, latch, _B1, _B2,
  // RGB_LED_R, RGB_LED_B, RGB_LED_G,
  // mainRED, mainYELL, mainGREEN,
  // secRED, secYELL, secGREEN, 


// The bitwise AND assignment operators (&=) and uses the binary representation of both
//operands, does a bitwise AND operation on them and assigns the result to the variables _B1, _B2, _B3

// The bitwise OR assignment operators (|=) and uses the binary representation of both
//operands, does a bitwise OR operation on them and assigns the result to the variables _B1, _B2, _B3


  DDRC &= ~(_B1 | _B2 | _B3);
  PORTC |= (_B1 | _B2 | _B3);

     DDRB = B11011111;  // digital pins 8 and 12 are set as outputs for data and clock. Then D9,D10,D11 as set as outputs for RGB
     DDRC = B00001000;  // analog pins A3 is set as an output for latch. Then B1,B2,B3 are set as inputs. And accelerometer pins SDA and SCL are set
     DDRD = B11111100;  // D2 to D7 are set as outputs for LEDs
 
     Serial.begin(9600); // open the serial port at 9600 bps:
     
  
  toggleLatch();
  ledState = LED_OFF;


  // for(int i=-1; ++i<256; shiftByteMSF(i)) delay(50);
  init_module1_clock=true;
  init_module5_clock=true;
  init_module6_clock=true;
  init_module13_clock = true;
  init_module14_clock = true;
  init_module15_clock = true;
  init_module16_clock = true;
  init_moduleAcc_clock = true;

  credit = 0;
  
   // PWR_MGMT_1 register for the accelerometer here
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}


void loop() {

  { // module 1
    // debouncer for button 1
    static unsigned long module_time, module_delay;
    static unsigned long debounce_count;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1
    
    if (init_module1_clock) {
      module_delay = 17;
      module_time=millis();
      module_doStep=false;
      init_module1_clock=false;
      module_i=0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {  // this line of code checks for the time elapsed, if it is > 17, then it will go to the switch
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
      switch(module_i){
          // the debouncers should be no problem for you now
          // you must put in the FSM code here
          case 0:
          B1_state=notPRESSED;
         // if (digitalRead(_B1)) module_i = 0;
        //  if (PORTC = _B1) module_i = 0;
          if (PINC & _B1) module_i = 0;
          else {
          debounce_count = module_time;
          module_i = 1;
          }
          break;

          case 1:
          B1_state=partialPRESS;
         // if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B1) module_i = 0;
          else if ((long)(millis()-debounce_count) < debounce ) module_i = 1 ;   
          else module_i = 2;
          break;

          case 2:
          B1_state=normalPRESS;
         // SW1_PRESSED;
          
        //  if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B1) module_i = 0;    
          
          else module_i = 2;
          break;
          default: module_i = 0; break;
        }
    }
  }

 {
    static char old;
    if(old!=B1_state){
      Serial.print("B1 = ");Serial.println(B1_state?((B1_state==2)?"pressed":"de-bouncing"):"not pressed");
      old=B1_state;
    }
  }
  

  

  { // module 5
    // debouncer for button 2
    static unsigned long module_time, module_delay;
    static unsigned long debounce_count_B2;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1
    
    if (init_module5_clock) {
      module_delay = 17;
      module_time=millis();
      module_doStep=false;
      init_module5_clock=false;
      module_i=0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
      switch(module_i){
          // the debouncers should be no problem for you now
          // you must put in the FSM code here
          case 0:
          B2_state=notPRESSED;
         // if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B2) module_i = 0;
          else {
          debounce_count_B2 = module_time;
          module_i = 1;
          }
          break;

          case 1:
          B2_state=partialPRESS;
         // if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B2) module_i = 0;
          else if ((long)(millis()-debounce_count_B2) < debounce ) module_i = 1 ;
          else module_i = 2;
          break;

          case 2:
          B2_state=normalPRESS;
        //  if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B2) module_i = 0;    
          
          else module_i = 2;
          break;
          
          default: module_i = 0; break;
        }
    }
  }

  {
    static char old;
    if(old!=B2_state){
      Serial.print("B2 = ");Serial.println(B2_state?((B2_state==2)?"pressed":"de-bouncing"):"not pressed");
      old=B2_state;
    }
  }
  

  { // module 6
    // heartbeat module
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module6_clock) {
      module_delay = 500;   // what should be the module delay of the heartbeat? 
      module_time=millis();
      module_doStep=false;
      init_module6_clock=false;
      module_i=0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
       switch (module_i) {
        case 0: {  
          ledState = ledState | SEGDP;         // there are two states here where the dot-segment bit of ledState 
          module_i = 1;
          break;
        }
        case 1:
        ledState = ledState & ~SEGDP;
        module_i = 0;
       // Serial.print("WORKING"); 
        break;     
        // is flipped to take care of the heartbeat going
        default: module_i=0;
       } 
    }
  }
     

static unsigned char orientation = 0;
     {   
     int16_t AcX,AcY,AcZ; // accelerometer variables
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,6,true);  // request a total of 6 registers
      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

      //
;

   
   //   Serial.print("ORIENTATION  ");
  //    Serial.println(orientation,DEC);
       if (abs(AcZ)>1.75*abs(AcX) && abs(AcZ)>1.75*abs(AcY))   {
        if (AcZ < 0) orientation = 1;  // flat face down
      //  Serial.print("1");
        else orientation = 0;  // flat face up
      }
      if (abs(AcX)>1.75*abs(AcZ) && abs(AcX)>1.75*abs(AcY)) {
        if (AcX < 0) orientation = 3;  // landscape upside down
        else orientation = 2; // landscape
      }
      if (abs(AcY)>1.75*abs(AcX) && abs(AcY)>1.75*abs(AcZ)) {
        if (AcY < 0) orientation = 4;  // portrate left
        else orientation = 5;  // portrate right
      }

  
       



    
   // {
      // this scope displays the credit on the 7Seg display
       unsigned char msb = 0b10000000 & ledState; //why do I need this?
        
       switch(credit){
          case 0:ledState = msb | LED_ZERO; break;
          case 1:ledState = msb | LED_ONE; break;
          case 2:ledState = msb | LED_TWO; break;
          case 3:ledState = msb | LED_THREE; break;
          case 4:ledState = msb | LED_FOUR; break;
          case 5:ledState = msb | LED_FIVE; break;
          case 6:ledState = msb | LED_SIX; break;
          case 7:ledState = msb | LED_SEVEN; break;
          case 8:ledState = msb | LED_EIGHT; break;
          case 9:ledState = msb | LED_NINE; break;
          case 10:ledState = msb | LED_A; break;
          case 11:ledState = msb | LED_b; break;
          case 12:ledState = msb | LED_C; break;
          case 13:ledState = msb | LED_d; break;
          case 14:ledState = msb | LED_E; break;
          case 15:ledState = msb | LED_F; break;


          
     
          
          // 15 more cases here to make sure the credit is displayed on the 7Seg
          default:credit = 0; ledState = msb | LED_OFF;
       }
     }



 // shiftByteMSF(ledState);
  //}

  { // module 11 button 3 debouncer

    //module11 variables    
    static bool init_clock = true;
    static unsigned long module_time, module_delay;
    static unsigned long debounce_count_B3;
    static bool doStep;
    static unsigned char i; // state variable for module 11
    static unsigned char module_i;
    
    if (init_clock) {
      module_delay = 17;
      module_time=millis();
      doStep=false;
      init_clock=false;
      i=0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; doStep = true;
      }
      else doStep = false;
    }

    if (doStep) {
      switch(i){
          // the debouncers should be no problem for you now
          // you must put in the FSM code here
          case 0:
          B3_state=notPRESSED;
         // if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B3) i = 0;
          else {
          debounce_count_B3 = module_time;
          i = 1;    // changed from module_i to i by me // i is variable given in template
          }
          break;

          case 1:
          B2_state=partialPRESS;
         // if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B3) i = 0;
          else if ((long)(millis()-debounce_count_B3) < debounce ) i = 1 ;
          else i = 2;  // changed from module_i to i by me // i is variable given in template
          break;

          case 2:
          B3_state=normalPRESS;
        //  if (digitalRead(_B1)) module_i = 0;
          if (PINC & _B3) i = 0;    
          
          else i = 2;    // changed from module_i to i by me // i is variable given in template
          break;
          default: i = 0; break;
        }
    }
  }
 

{
    static char old;
    if(old!=B3_state){
      Serial.print("B3 = ");Serial.println(B3_state?((B3_state==2)?"pressed":"de-bouncing"):"not pressed");
      old=B3_state;
    }
  }


  { // module 13 master duration

    //module13 variables    
    static unsigned long module_time, module_delay;
    static bool doStep, buttonsEnable;
    static char i, j, k; // state variables for module 13
    static unsigned long limit;
    static unsigned long t; // aux time for how long a particular duration lasts
  //  static unsigned char orientation;
//    
    if (init_module13_clock) {
      module_delay = 20;
      module_time=millis();
      doStep=false;
      buttonsEnable = init_module13_clock = false;
      md1 = md2 = md3 = t = i = j = k = 0; // stop master duration and reset the time for which it lasts
      ew1=ew2=ew3=false;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; doStep = true;
    }
     else doStep = false;
        }
    if (doStep) {

    { // this bracket is till the accel debounce

//
//      
//      
//   //    
        { //state machine to blink the 7Seg display immediately after a wheel spin
      static unsigned long timeout, blinktime;
      unsigned char msb = 0b10000000 & ledState;
      static unsigned char origLedState;
      switch (j) {
        case 0: {
          // wait for a wheel to start
          if (i>0) j = 1; else j = 0;
          break;
           }
          
        case 1: {
          // wait for all wheels to stop
            if (ew1 || ew2 || ew3) j = 1;
         else {
           j = 2;
           blinktime  =  millis();
           timeout = millis();
           origLedState = ledState;
          }
          break;
              
        }
        case 2:{
         // blink inverted
           if (millis() -  timeout <  2000)  {
             if (millis() -  blinktime  < 500)   {
              j =  2;
              //
             //
             ledState = ~ledState;
             shiftByteMSF(ledState);
             }
             else {
              j = 3;
              blinktime = millis();
             }
           }
           else j = 0;        
          break;
          }
        case 3:{
          // blink clear
       // if ((millis() - timeout < 2000) && (millis() - blinktime < 500)) { 
       if (millis() - timeout < 2000) {
       if (millis() - blinktime < 500) {
          j = 3;
        }
         else {
          shiftByteMSF(origLedState);
          j = 2;
          blinktime = millis();
          }
        }
        else j = 0;
        break;
       }
        }
      }
        
        

 { static unsigned long t, t_delay;  // t_delay written by me
          static unsigned long debounce_tcount; 
          static bool modulet_doStep;
          static unsigned char modulet_i; 
          
  //  Serial.print("K = ");
 //   Serial.println(k,DEC);        
      switch (k) { 
        // state machine to act as a debouncer for the accelerometer.
        // you need this to implement the gesture control
 //////////////////////////////////////////////////////////////
 //orientation 0 = // flat face up
 //orientation = 1 // flat face down
 //orientation = 2 // landscape
 //orientation = 3 // landscape upside down
 //orientation = 4 // portrate left
 //orientation = 5 // portrate right
 
  
        case 0: // assuming flat
          if (orientation == 4) {
            k = 1; // left
            t=millis();
          }
          else if (orientation == 5) {
            k = 3; //right
            t=millis();
          }
          else k = 0;
          break;
        case 1: // asuming left
          // debounce left here and go to state 2 once you've accepted you're in left position
           if((long)(millis() - t) >= debounce)
         {
           k = 2;
         } 
         else if (orientation != 4) // == 1 in pic
         {
           k = 0;
         } 
         else
         {
           k = 1;
         }
          break;
        case 2:
          if (orientation == 0 ) { {k = 0;   credit = credit + 1;}//from left to flat
          
            // increment the credit and return to flat state
          //    credit++;  // To do cap to 15
          // k = 0;
         if  (credit > 15) credit--; {
            
           }
          
         }
         
         else if (orientation == 4) //stay here
         {
           k = 2;
         }
         else // if there is an error, go to flat again
         {
           k = 0;
         }
         break; 
        case 3: //assuming right
          //debounce right here and go to state 4 to accept right orientation
          if(orientation != 5)
         {
           k = 0;
         }
         if((long)(millis() - t) < debounce)
         {
           k = 3;
         }
         else
         k = 4;
          break;
        case 4:
          if (orientation == 0 ) { //from right to flat
           credit--; 
           k = 0;
           if  (credit < 0) { credit++;
            
           }

         }
         else if (orientation == 5) //stay here
          {
           k = 4; // decrement the credit, go to flat again
          }
          else 
          {
           k = 0; 
          }
          
          break;
        default: k=0; break;
     }


         }
         }
         Serial.println(credit, DEC);

//      
      switch(i){
        // state machine to implement the spin of the wheels
        // you need to debounce the lever-pull gesture control
        // you need to implement an individual wheel spin or all wheels together
        // if the conditions allow it
       // int timez;
        static unsigned long t, timez;
        case 0: // assuming flat
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
          if (orientation == 2) { // is the lever pulled  // orientation 2 is because its landscape
            i = -1; // go to state -1 to debounce lever pull
           
          }
          
          else if ((B1_state == normalPRESS) && (credit >= 5))   i=5;  // if B1 is pressed and credit is >= 5 then code will go to case 5
            else if ((B2_state == normalPRESS) && (credit >= 5))   i=6; // if B2 is pressed and credit is >= 5 then code will go to case 6
            else if ((B3_state == normalPRESS) && (credit >= 5))  i=7; // if B3 is pressed and credit is >= 5 then code will go to case 7
            
          else i = 0;
          break;
        case -1: // lever-pull debounce
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
          if (orientation == 0) i=0;
          else if (millis() - t < debounce) i=-1;
          else i=-2;
          break;
        case -2: // lever-pull accepted
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
          if (orientation == 0 && credit) {// lever released start the sequence
            i = 1; buttonsEnable = true;
          }
          else if (orientation == 2) i=-2;
          else i=0;
          break;
          case 1: 
            // prep the wheel spin durations
            // if everything is ok then get the wheels released
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
            if (orientation == 0) {
              i = 2;
              credit--;// decrement the credit
              md1 = md2 = md3 = 50;  // initial master durations md1, md2, md3 are set here for first 500ms but after this it should increment by 50ms for every 20ms of roll             
              ew1=ew2=ew3=true;   //* enable the wheels */;
              limit = random(1000, 1300); // to randomise the limit here
              t=millis();
            }
            else i = 1;
            break;
          case 5: 
            // prep only wheel 1 because we have a button 1 for wheel 1
            // if everything is ok let wheel 1 only spin
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
            if (B1_state == notPRESSED) {      // /* wait for B1 to be  notPRESSED */
              i = 2;
               credit = credit - 5; // credit - 5 
              md1 = 50;// initial master durations here  // whats this ?
              ew1 = true;  // enable wheel 1 only keep the rest halted
              limit = random(1000, 1300); // to randomise the limit
              t=millis();
            }
            else i = 5;
            break;
          case 6: 
            // prep only wheel 2 because we have a button 2 for wheel 2
            // if everything is ok let wheel 2 only spin
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
            if (B2_state == notPRESSED) {
              i = 2;
              credit = credit - 5; // credit - 5 
              md2 = 50;// initial master durations here // whats this ?
              ew2 = true; // enable wheel 2 only keep the rest halted
              limit = random(1000, 1300); // to randomise the limit
              t=millis();
            }
            else i = 6;
            break;
          case 7: 
            // prep only wheel 3 because we have a button 1 for wheel 3
            // if everything is ok let wheel 3 only spin
            md1 = md2 = md3 = 0;
            ew1=ew2=ew3=false;
           Serial.print("B3_state = "); Serial.println(B3_state);
            if (B3_state == notPRESSED) {
              i = 2;
              credit = credit - 5;// credit - 5 
              md3 = 50;// initial master durations here
              ew3 = true;// enable wheel 3 only keep the rest halted
              limit = random(1000, 1300); // to randomise the wheels
              t=millis();
            }
            else i = 7;
            break;
          case 2: 
            // this is the start of the timing sequence   // should i write code for this line ? 
            if (((long)(millis() - t) < 500)) i = 2;
            else if (((long)(millis() - t) >= 500) && md1 <= limit) {
              i = 2;
              md3 = md2 = md1 += 15;
              
            }
            else {
              md1 = 0;
              if (ew2 || ew3) {
                i = 3; limit+=random(1000,1300); t=millis();
              }
              else i = 9;
            }
            break;
          case 3: 
            ew2 = true; // wheel 2 continues to spin
            if (((long)(millis() - t) < 500)) i = 3;
            else if (((long)(millis() - t) >= 500) && md2 <= limit) {
              i = 3;
              md3 = md2 += 10;
              
            }
            else {
              md2 = 0;
              if (ew3) {
                i = 4; limit+=random(1000,1300); t=millis();
              }
              else i = 9;
            }
            break;
          case 4: 
            ew3 = true; // wheel 3 continues to spin
            if (((long)(millis() - t) < 500)) i = 4;
            else if (((long)(millis() - t) >= 500) && md3 <= limit) {
              i = 4;
              md3 += 10;
              
            }
            else {
              i = 9;
              md3 = 0;
            }
            break;
          case 9: 
            ew1=ew2=ew3=false;// all wheels have stopped
            // check for a win
            if (w1State == w2State && w2State == w3State) {
              credit=(credit>5)?15:(credit+10);            
               
              buttonsEnable=false;
              } 
            i=0;
            break;
          default: i = 0; break;
        }
        Serial.println(i, DEC);
    }
  }     // master module should be closed here

  { // module 14 wheel 1
    // this module spins wheel 1
    // it waits for an enable and then it turns
    // the wheel's leds on and off to give the impression
    // of the wheel spinning
    // w1State is a global variable to tell the rest of 
    // the system what is the state of the first wheel
    //module 14 variables    
    static unsigned long module_time, module_delay;
    static bool doStep;
    static unsigned char i; // state variable for module 14
    static unsigned long t;
    
    
    if (init_module14_clock) {    // call this from above
      module_delay = 80;
      module_time=millis();
      doStep=false;
      init_module14_clock=false;
      w1State = i = 0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; doStep = true;
      }
      else doStep = false;
    }
    if (doStep) {
      switch(i){
          case 0: 
            // red = on, amber = off, green = off
            mainRED_HI;mainYELL_LO;mainGREEN_LO;
            if (ew1 && md1 > 0 && ((long)(millis() - t) >= md1)) {
              i = 1;
              t=millis();
              

            }
            else i = 0;
            break;
          case 1: 
            // red = off, amber = on, green = off
            mainRED_LO,mainYELL_HI,mainGREEN_LO;
            if (ew1 && md1 > 0 && ((long)(millis() - t) >= md1)) {
              i = 2;
              t=millis();
              
            }
            else i = 1;
            break;
          case 2: 
            // red = off, amber = off, green = on
            mainRED_LO,mainYELL_LO,mainGREEN_HI;
            if (ew1 && md1 > 0 && ((long)(millis() - t) >= md1)) {
              i = 0;
              t=millis();
              
            }
            else i = 2;
            break;
          default: i = 0; break;
        }
    w1State = i;
    }
  }

  { // module 15 wheel 2
    // this module spins wheel 2
    // it waits for an enable and then it turns
    // the wheel's leds on and off to give the impression
    // of the wheel spinning
    // wheel 2 is th eRGB wheel and you need to build 
    // the corresponding colour using analogWrite and 
    // different colour intensities for the prime colours
    // w2State is a global variable to tell the rest of 
    // the system what is the state of the second wheel
    //module 15 variables    
    static unsigned long module_time, module_delay;
    static bool doStep;
    static unsigned char i; // state variable for module 14
    static unsigned long t;
  
    
    if (init_module15_clock) {
      module_delay = 20;
      module_time=millis();
      doStep=false;
      init_module15_clock=false;
      w2State = i = 0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; doStep = true;
      }
      else doStep = false;
    }
    if (doStep) {
      Serial.print("Wheel 2 i = "); Serial.println(i, DEC);
      switch(i){
          // same as module 14, except that you need to use analogWrite to make amber colour
          // written by me
          case 0: 
            // red = on, amber = off, green = off
              analogWrite(RGB_LED_R,255);
              analogWrite(RGB_LED_G,0);  
              analogWrite(RGB_LED_B,0);  
            if (ew2 && md2 > 0 && ((long)(millis() - t) >= md2)) {
              i = 1;
              t=millis();
                                  
            }
            else i = 0;
            break;
          case 1: 
            // red = off, amber = on, green = off
              analogWrite(RGB_LED_R,255);
              analogWrite(RGB_LED_G,15);  
              analogWrite(RGB_LED_B,0);  
            if (ew2 && md2 > 0 && ((long)(millis() - t) >= md2)) {
              i = 2;
              t=millis();
              
            }
            else i = 1;
            break;
          case 2: 
            // red = off, amber = off, green = on
              analogWrite(RGB_LED_R,0);
              analogWrite(RGB_LED_G,235);  
              analogWrite(RGB_LED_B,0);
            if (ew2 && md2 > 0 && ((long)(millis() - t) >= md2)) {
              i = 0;
              t=millis();
              
            }
            else i = 2;
            break;

            // written by me
      
          
          default: i = 0; break;
        }
    w2State = i;
    }
  }

  { // module 16 wheel 3
    // this module spins wheel 3
    // it waits for an enable and then it turns
    // the wheel's leds on and off to give the impression
    // of the wheel spinning
    // w3State is a global variable to tell the rest of 
    // the system what is the state of the third wheel
    //module 16 variables    
    static unsigned long module_time, module_delay;
    static bool doStep;
    static unsigned char i; // state variable for module 16
    static unsigned long t;
    
    
    if (init_module16_clock) {
      module_delay = 80;
      module_time=millis();
      doStep=false;
      init_module16_clock=false;
      w3State = i = 0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) > module_delay) {
        module_time = m; doStep = true;
      }
      else doStep = false;
    }
    if (doStep) {
      switch(i){
          // same as module 14 but modified for the third wheel
          // Written by me
      case 0: 
            // red = on, amber = off, green = off
            secRED_HI; secYELL_LO; secGREEN_LO;
            if (ew3 && md3 > 0 && ((long)(millis() - t) >= md3)) {
              i = 1;
              t=millis();
              
            }
            else i = 0;
            break;
          case 1: 
            // red = off, amber = on, green = off
            secRED_LO; secYELL_HI; secGREEN_LO;
            
            if (ew3 && md3 > 0 && ((long)(millis() - t) >= md3)) {
              i = 2;
              t=millis();
              
            }
            else i = 1;
            break;
          case 2: 
            // red = off, amber = off, green = on
             secRED_LO; secYELL_LO; secGREEN_HI;
              
            if (ew3 && md3 > 0 && ((long)(millis() - t) >= md3)) {
              i = 0;
              t=millis();
             
            }
            else i = 2;
            break;

            // written by me   
          
          default: i = 0; break;
        }
    w3State = i;
    }
  }

  shiftByteMSF(ledState);
 }
 
void shiftBit(bool data) {
  // shift a single bit to the shift register using bare metal
 if (data)
    DATAHIGH;
 else   
  DATALOW;
  CLOCKHIGH;
  CLOCKLOW;  
}

void toggleLatch() {
  LATCHHIGH;
  LATCHLOW; 
  
  
  // toggle the latch using bare metal
}

void shiftByteMSF(unsigned char b) {
  unsigned char m;
  // one line of code here to shift the bits to the shift register.
for (m = B10000000; m != 0; m = m >> 1)
{
  bool data = m &  b;
  shiftBit(data);
}
  
  toggleLatch();  
}
