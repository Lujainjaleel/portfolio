////////////////////////////////////////////////////////////////
// Task 1
// Arduino Nano Resource Manager
// Lujain Jaleel Name / Date 26/06/21
////////////////////////////////////////////////////////////////

#include <Wire.h>

// IO-definitions
#define GRANTED                     A0
#define ANALOG_INPUT                A6
#define _B1 13 //button pin
#define _B2 A1
#define _B3 A2
#define red1    2
#define amber1    3
#define green1    4
unsigned char ledState;

#define RGB_LED_R 9  // rgb red pin (pwm)
#define RGB_LED_B 10// rgb blue pin (pwm)
#define RGB_LED_G 11// rgb green pin (pwm)


/////
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
  
// unsigned char msb = 0b10000000 & ledState;
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
#define LED_t (SEGF|SEGE|SEGD|SEGG)


#define LATCH  B00001000
#define LATCHHIGH PORTC |= LATCH 
#define LATCHLOW PORTC &= ~LATCH    


#define CLOCK  B00010000
#define CLOCKHIGH PORTB |= CLOCK 
#define CLOCKLOW PORTB &= ~CLOCK    

#define DATA  B00000001
#define DATAHIGH PORTB |= DATA 
#define DATALOW PORTB &= ~DATA    

//////////

int greendelay = 4000;
float Temp;
int T;
unsigned char tilt;
unsigned char cmd_value;
unsigned char state_main, state_rgb, state_seg;

#define MPU_addr 0x68  // I2C address of the MPU-6050


// State definitions
#define NoTriggerNoDemand    0   // idle state
#define TriggerNoDemand      1   // trigger detected
#define TriggerIamMaster     2   // we are master after a successful claim
#define IamSlave             3   // we are slave after other device has made a successful claim
int state;
bool trigger;

// Global variable for storing the orientation
char orientation;

// Clock initializer for 15ms timer
bool init_module50_clock;
bool init_module0_clock;
//module1 variables 
//module3 variables
bool init_module3_clock;
bool init_module4_clock;
bool init_module5_clock;
bool init_module6_clock;
bool init_module7_clock;
bool init_module8_clock;
bool init_module9_clock;
bool init_module10_clock;
bool init_module11_clock;
bool init_module12_clock;
bool init_module13_clock;
bool init_module14_clock;
bool init_module15_clock;
bool init_module16_clock;
bool init_module17_clock; // B3
bool init_module18_clock; // B3

//
//bool initmodule4 // maintanence
//bool initmodule5 // traffic  lights
//bool iniitmodule6 //red rgb
//bool initmodule7 // blue rgb
//bool initmodule8 // amber rgb
//bool initmodule9 // green rgb

//

#define notPRESSED 0
#define partialPRESS 1
#define normalPRESS 2
#define debounce 500 // debounce duration
#define debouncebutton 300

// This code is to turn on and off the LEDs
#define mainRED B00000100
#define mainYELL B00001000
#define mainGREEN B00010000
#define secRED B00100000
#define secYELL B01000000
#define secGREEN B10000000
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

unsigned char B1_state; // buttons state variables
unsigned char B2_state; // buttons state variables
unsigned char B3_state; // buttons state variables

//==============================================================
// demandRequest()
//
// This function measures the voltage on analogue pin A6
// If this exceeds 1.25V the line is considered to be a 
// logic 1, there is no demand, and the function returns false. 
// Otherwise the line is considered a logic '0', there is
// demand and the function returns true.
//==============================================================
bool demandRequest()
{
  return (!(analogRead(ANALOG_INPUT) >> 8));
}

//==============================================================
// leaveHigh()
//
// This function implements a safe method of asserting a 
// logic 1. The required pin is passed as a parameter.
//==============================================================
void leaveHigh (unsigned char pin) {
  pinMode(pin,INPUT);
}

//==============================================================
// pullLow()
//
// This function implements a safe method of asserting a 
// logic 0. The required pin is passed as a parameter.
//==============================================================
void pullLow (unsigned char pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

//



void setup() {
 // leaveHigh(GRANTED);
  toggleLatch();

  DDRB = B11011111;  // digital pins 8 and 12 are set as outputs for data and clock. Then D9,D10,D11 as set as outputs for RGB
     DDRC = B00001000;  // analog pins A3 is set as an output for latch. Then B1,B2,B3 are set as inputs. And accelerometer pins SDA and SCL are set
 
  // Initialize state and trigger to sensible values.
  state = NoTriggerNoDemand;
  trigger = false;

  // Initialize the orientation variable to a sensible value
 

  // Initialize the I2C Wire object. This should be configured
  // as a slave. It should have event-driven function(s). As the
  // Arduino needs to access the I2C bus to turn-on the accelerometer
  // consideration needs to be given to where that can be safely 
  // carried out.
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */

  Serial.begin(115200);  
  // Start the 15ms timer
  init_module50_clock = true;

  pinMode(_B1, INPUT);
  pinMode(_B2, INPUT_PULLUP);
  pinMode(_B3, INPUT_PULLUP);
  
//  pinMode(red1, OUTPUT);
//  pinMode(amber1, OUTPUT);
//  pinMode(6, OUTPUT);
//  pinMode(green1, OUTPUT);
  pinMode(RGB_LED_R,OUTPUT);
  pinMode(RGB_LED_G,OUTPUT);
  pinMode(RGB_LED_B,OUTPUT);
 
  init_module0_clock=true;
  init_module3_clock=true;
  init_module4_clock = true;
  init_module5_clock = true;
  init_module6_clock = true;
  init_module7_clock = true;
  init_module8_clock = true;
  init_module9_clock = true;
  init_module10_clock = true;
  init_module11_clock = true;
  init_module12_clock = true;
  init_module13_clock = true;
  init_module14_clock = true;
  init_module15_clock = true;  // fruit 1
  init_module16_clock = true;  // fruit 2
  init_module17_clock = true;
  init_module18_clock = true;
  init_module50_clock = true;
  
  B1_state=notPRESSED;
  B2_state=notPRESSED;
  B3_state=notPRESSED;
  Serial.begin(115200);

  DDRD = B11111100;

  state_main = 0x61; // equal  priority


  
}



void loop() {
  static boolean  firstTime = true;
  int             AcX, AcY, AcZ;
  
  // This is the code for the Resource Manager. This has the highest
  // priority and must run every time the loop() function is executed.
  switch(state)
  {
  case NoTriggerNoDemand:
    leaveHigh(GRANTED);
    if (!trigger && !demandRequest())
    {
      state = NoTriggerNoDemand;
    }
    else if (trigger)
    {
      state = TriggerIamMaster;
    }
    else
    {
      state = IamSlave;
    }
    break;

  case TriggerIamMaster:
    leaveHigh(GRANTED);
    if (trigger)
    {
      state = TriggerIamMaster;
    }
    else if (demandRequest())
    {
      state = IamSlave;
    }
    else
    {
      state = NoTriggerNoDemand;
    }
    break;

  case IamSlave:
    pullLow(GRANTED);
    if (demandRequest())
    {
      state = IamSlave;
    }
    else
    {
      state = NoTriggerNoDemand;
    }
    break;
  }

  // This will implement an FSM containing 4 states.
  // State 0: Set a delay time of 15 ms, then move immediately to state 1
  // State 1: Stay here until the delay time has elapsed, 
  //          activate the trigger, then move to state 2 
  // State 2: Stay here until the Resource Manager reaches the IamMaster
  //          state. Then move to state 3.
  // State 3: Read the accelerometer data, disable the trigger, process the
  //          data, and return back to state 0. 
  {  // module 50
   
    static unsigned long module_time, module_delay;
    static unsigned long timeout;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module50_clock) {
      module_delay = 1;
      module_time=millis();
      module_doStep=false;
      init_module50_clock=false;
      module_i = 0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) >= module_delay) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }
 
    if (module_doStep) {      
      // Act on the current state (module_i).
      switch(module_i)
      {
      case 0:
        // Acquire the current time
        timeout = millis();
        module_i = 1;
        break;

      case 1:
        // Stay in State 1 until the 15ms tumeout has elapsed
        if ((long)(millis() - timeout) >= 15)
        {
          module_i = 2;
          trigger = true;
        }
        break;

      case 2:
        // Stay in State 2 until Resource Manager ends up in
        // the IamMaster state
        if (state == TriggerIamMaster)
        {
          module_i = 3;
        }
        break;
       
      case 3:        
        // Now master, it is safe to access the I2C bus.
        // Initializing the accelerometer if it needs to be.
        if (firstTime)
        {
          Wire.beginTransmission(MPU_addr);
          Wire.write(0x6B);  // PWR_MGMT_1 register
          Wire.write(0);     // set to zero (wakes up the MPU-6050)
          Wire.endTransmission(true);
          firstTime = false;      
        }
    
        // Read the accelerometer.
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_addr,8,true);  // request a total of 6 registers
        AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
        AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        T = Wire.read()<<8|Wire.read();
        
        if (abs(AcZ)>1.75*abs(AcX) && abs(AcZ)>1.75*abs(AcY)) {
          if (AcZ < 0) orientation = 'b';
          else orientation = 'F';
        }
        if (abs(AcX)>1.75*abs(AcZ) && abs(AcX)>1.75*abs(AcY)) {
          if (AcX < 0) orientation = 'L';
          else orientation = 'U';
        }
        if (abs(AcY)>1.75*abs(AcX) && abs(AcY)>1.75*abs(AcZ)) {         //  Serial.print("Scheduler state = ");Serial.println(state,DEC);
          if (AcY < 0) orientation = 'r';
          else orientation = 'l';
        }

        Temp = (T / 340.0) + 36.53;
        
        if (AcZ < 10000 && AcZ > -10000) {
          
        tilt = 'D';
        }
        else {
        tilt = 'N';
        }
        
     //  Serial.print("Temperature = "); Serial.println(Temp);
        

        // Now release the resouce and go back to the start
        trigger = false;
        module_i = 0;
        break;

      default:
        module_i = 0;
      }
    }
  } 

/////////////////////////////////////////////////////////////////////////////////////////////
  { // module 4 // maintanence mode
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module4_clock) {
      module_delay = 500;
      module_time=millis();
      module_doStep=false;
      init_module4_clock=false;
      state=0;
      //digitalWrite(amber1, LOW);
    }
    else {
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
     // Serial.print("MAINT = ");
     // Serial.println(state, DEC);
      switch(state){
          case 0:
             //digitalWrite(green1, HIGH);
             mainYELL_HI ;
             secYELL_HI;

          
             state = 1;
             break;
          case 1:
            // digitalWrite(green1, LOW)
            mainRED_LO;
            mainGREEN_LO;
            mainYELL_LO;
            secRED_LO;
            secGREEN_LO ;
            secYELL_LO;

        
             state = 0;
             break;
          default: state = 0; break;
      }
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////

 { // module 5 // traffic lights
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module5_clock) {
      module_delay = 500;
      module_time=millis();
      module_doStep=false;
      init_module5_clock=false;
      state=0;
      //digitalWrite(amber1, LOW);
    }
    else {
     
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

 
    if (module_doStep) {   
    //  Serial.print("TL STATE (Module5) = ");
    //  Serial.println(state);
    
      switch(state){
        case 0:
          mainYELL_HI ;
          secYELL_HI;
         
          mainRED_LO;
          mainGREEN_LO;
          secRED_LO;
          secGREEN_LO;
          state = 1;
          module_delay = 2000;
          break;
         
        case 1:
          mainRED_HI;
          secRED_HI;
         
          secGREEN_LO ;
          secYELL_LO;
          mainGREEN_LO;
          mainYELL_LO;     
          state = 2;
          module_delay = 1000;
          break;

        case 2:
          mainRED_HI;
          mainYELL_HI; 
          secRED_HI;

          secGREEN_LO ;
          secYELL_LO;
          mainGREEN_LO;
          state =  3;
          module_delay = 1000;
          break;

        case 3:
          mainGREEN_HI;
          secRED_HI;

          secGREEN_LO ;
          secYELL_LO;
          mainYELL_LO;
          mainRED_LO;
          state = 4;
          module_delay = greendelay;
          break;

        case 4:
          mainYELL_HI;   
          secRED_HI;

          secGREEN_LO;
          mainGREEN_LO;
          secYELL_LO;
          //mainYELL_LO;
          mainRED_LO;
          state = 5;
          module_delay = 1000;
          break;

        case 5:
          mainRED_HI;
          secRED_HI;

          secGREEN_LO;
          mainGREEN_LO;
          secYELL_LO;
          mainYELL_LO;
          state = 6;
          module_delay = 1000;
          break;

        case 6:
          mainRED_HI;
          secRED_HI;
          secYELL_HI;

          secGREEN_LO;
          mainGREEN_LO;
          //secYELL_LO;
          state = 7;
          module_delay = 1000;
          break;

        case 7:
          mainRED_HI;
          secGREEN_HI;

          mainGREEN_LO;
          secRED_LO;
          secYELL_LO;
          state = 8;
          module_delay = 8000 - greendelay;
          break;

        case 8:
          mainRED_HI;
          secYELL_HI;

          mainGREEN_LO;
          secGREEN_LO;
          secRED_LO;
          mainYELL_LO;
          module_delay = 1000;
          state = 1;
          break;
          
        default: state = 0; break;
      }
    } 
  }
/////////////////////////////////////////////////////////////////////////////////////////
  { // module 6
    // red fade in lights
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1
    static float intensity,max_intensity,lvl;
    static int intens = 0;
 
   {
    static unsigned char old;
   
    if(old!=module_i){
    //  Serial.println(old=module_i);
     
    }
  }
   
    if (init_module6_clock ) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      max_intensity = 254;
      intensity = 0;
      intens = 0;
      lvl =   (max_intensity / ((1000.0 / module_delay)));
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
//      Serial.print(module_i);
//      Serial.print(" IN RED ");
//      Serial.print(intens);      
//      Serial.print(" lvl ");
//      Serial.println(lvl);
       //analogWrite(RGB_LED_R,(unsigned char) intensity);
       analogWrite(RGB_LED_R, intens);
       switch (module_i) {
        case 0: { 
        intens = intens + 12;
          
        //intensity = intensity + lvl;
        //if (intensity > 255) {
        //  intensity = 255;
        //  module_i = 1;
        //}

        
//        if (intens > 255)
//        {  
//          intens= 255;
//          module_i = 1;
//        }
        break;
        }
       
        case 1:
        //intensity = intensity - lvl;
        //if (intensity < 0) {
        //intensity = 0; 
        //module_i = 0;
        //}

        
        intens = intens - 12;
        if (intens < 0) {
        intens = 0; 
        module_i = 0;
        }
        break;    
        default: module_i=0;
       }
    }
  }
/////////////////////////////////////////////////////////////////////////////////////
   { // module 7
    // blue fade in lights
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1
    static float intensity,max_intensity,lvl;
 
   {
    static unsigned char old;
   
    if(old!=module_i){
    //  Serial.println(old=module_i);
     
    }
  }
   
    if (init_module7_clock) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      max_intensity = 253;
      intensity = 0;
      lvl =   (max_intensity / ((1000.0 / module_delay)));
      module_doStep=false;
      init_module7_clock=false;
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
       analogWrite(RGB_LED_B,(unsigned char) intensity);
       switch (module_i) {
        case 0: { 
        intensity = intensity + lvl;
        if (intensity > 253) {
        intensity = 253;
       
        module_i = 1;
        }
        break;
        }
       
        case 1:
        intensity = intensity - lvl;
        if (intensity < 0) {
        intensity = 0; 
        module_i = 0;
        }
        break;    
        default: module_i=0;
       }
    }
  }
//////////////////////////////////////////////////////////////////////////////
  { // module 8
    // amber fade in lights
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1
    static float intensity,max_intensity,lvl,lvls,intensityg,max_intensitys;
 
   {
    static unsigned char old;
   
    if(old!=module_i){
  //    Serial.println(old=module_i);
     
    }
  }
   
    if (init_module8_clock) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      max_intensity = 254;
      max_intensitys = 20;
      intensity = 0;
      lvl =   (max_intensity / ((1000.0 / module_delay)));
      lvls =   (max_intensitys / ((1000.0 / module_delay)));
      module_doStep=false;
      init_module8_clock=false;
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
       analogWrite(RGB_LED_R,(unsigned char) intensity);
       analogWrite(RGB_LED_G,(unsigned char) intensityg);
       switch (module_i) {
        case 0: { 
        intensity = intensity + lvl;
        intensityg = intensityg + lvls;
        if ((intensity > 255 ||  intensityg > 20)) {
        intensity = 255;  intensityg = 20;
       
        module_i = 1;
        }
        break;
        }
       
        case 1:
        intensity = intensity - lvl;
        intensityg = intensityg -  lvls;
        if ((intensity < 0 || intensityg < 0)) {
        intensity = 0;  intensityg =  0;
        module_i = 0;
        }
        break;    
        default: module_i=0;
       }
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////
  { // module 13
    // green fade in lights
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1
    static float intensity,max_intensity,lvl;
 
   {
    static unsigned char old;
   
    if(old!=module_i){
    //  Serial.println(old=module_i);
     
    }
  }
   
    if (init_module13_clock) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      max_intensity = 255;
      intensity = 0;
      lvl =   (max_intensity / ((1000.0 / module_delay)));
      module_doStep=false;
      init_module13_clock=false;
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
       analogWrite(RGB_LED_G,(unsigned char) intensity);
       switch (module_i) {
        case 0: { 
        intensity = intensity + lvl;
        if (intensity > 255) {
        intensity = 255;
       
        module_i = 1;
        }
        break;
        }
       
        case 1:
        intensity = intensity - lvl;
        if (intensity < 0) {
        intensity = 0; 
        module_i = 0;
        }
        break;    
        default: module_i=0;
       }
    }
  }

////////////////////////////////////////////////////////////////////////////////////////
   { // module 11
    // heartbeat module
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module11_clock) {
      module_delay = 500;   // what should be the module delay of the heartbeat?
      module_time=millis();
      module_doStep=false;
      init_module11_clock=false;
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

  /////////////////////////////////////////////////////////////////////////////////
  { // module 15 // fruit machine 
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module15_clock) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      module_doStep=false;
      init_module15_clock=false;
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
        case 0: 
          mainRED_HI;      
          mainYELL_LO;
          mainGREEN_LO;
//          module_i = 1; 
          if (B1_state == normalPRESS) {
            module_i = 1;
          }
          break;
        
        case 1:
         mainRED_LO;      
         mainYELL_HI;
         mainGREEN_LO;
        //  module_i = 2;
        if (B1_state == normalPRESS) {
          module_i = 2;
        }
        break;

        case 2:
         mainRED_LO;      
         mainYELL_LO;
         mainGREEN_HI;
      //    module_i = 2;
        if (B1_state == normalPRESS) {
          module_i = 0;
        }
       
       // Serial.print("WORKING");
        break;    
        // is flipped to take care of the heartbeat going
        default: module_i=0;
       }
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////

  //////
{ // module 16 // fruit machine 2
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module16_clock) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      module_doStep=false;
      init_module16_clock=false;
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
        case 0: 
          secRED_HI;      
          secYELL_LO;
          secGREEN_LO;
       //   module_i = 1; 
          if (B2_state == notPRESSED) {
            module_i = 1;
          }
          break;
        
        case 1:
         secRED_LO;      
         secYELL_HI;
         secGREEN_LO;
       //   module_i = 2;
        if (B2_state == notPRESSED) {
          module_i = 2;
        }
        break;

        case 2:
         secRED_LO;      
         secYELL_LO;
         secGREEN_HI;
     //     module_i = 2;
        if (B2_state == notPRESSED) {
          module_i = 0;
        }
        
       // Serial.print("WORKING");
        break;    
        // is flipped to take care of the heartbeat going
        default: module_i=0;
       }
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////
  ///////
//////
{ // module 18 // fruit machine 3
    static unsigned long module_time, module_delay;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module18_clock) {
      module_delay = 50;   // what should be the module delay of the heartbeat?
      module_time=millis();
      module_doStep=false;
      init_module18_clock=false;
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
        case 0: 
         analogWrite(RGB_LED_R,230);
         analogWrite(RGB_LED_B,0);
         analogWrite(RGB_LED_G,0);
       //   module_i = 1; 
          if (B3_state == notPRESSED) {
            module_i = 1;
          }
          break;
        
        case 1:
        analogWrite(RGB_LED_R,0);
         analogWrite(RGB_LED_B,0);
         analogWrite(RGB_LED_G,230);
       //   module_i = 2;
        if (B3_state == notPRESSED) {
          module_i = 2;
        }
        break;

        case 2:
         analogWrite(RGB_LED_R,0);
         analogWrite(RGB_LED_B,230);
         analogWrite(RGB_LED_G,0);
     //     module_i = 2;
        if (B3_state == notPRESSED) {
          module_i = 0;
        }
        
       // Serial.print("WORKING");
        break;    
        // is flipped to take care of the heartbeat going
        default: module_i=0;
       }
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////
  ///////

  { // module 3 - SCHEDULER
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module3_clock) {
      module_delay = 10;
      module_time=millis();
      module_doStep=false;
      init_module3_clock=false;
      state=0;
    }
    else {
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }
    {
    static unsigned char old;
    if(old!=state){
    //  Serial.print("Scheduler state = ");Serial.println(state,DEC);
      old=state;
    }
  }
    if (module_doStep) {
      switch(state_main){
        
          case 0x61:   // equal priority

            init_module5_clock = false; // traffic on 
            
          // ledState |=  LED_E; 
          ledState = (ledState & SEGDP) | LED_E;
            init_module4_clock = true;
            init_module6_clock = true;
            init_module7_clock = true;
            init_module8_clock = true;
            init_module15_clock = true;
            init_module16_clock = true;
            init_module18_clock = true;
            greendelay = 4000;
            break;
        
          case 0x62:   // set 1
            init_module5_clock = false;
            ledState =  (ledState & SEGDP) | LED_ONE; 
            init_module4_clock = true;
            init_module6_clock = true;
            init_module7_clock = true;
            init_module8_clock = true; 
            init_module15_clock = true;
            init_module16_clock = true;
            init_module18_clock = true;
            greendelay = 6000;
            break;      
           
            
           case 0x63:   // set 2
            
            init_module5_clock = false;
             ledState =  (ledState & SEGDP) | LED_TWO; 
         
            init_module4_clock = true;
            init_module6_clock = true;
            init_module7_clock = true;
            init_module8_clock = true; 
            init_module15_clock = true;
            init_module16_clock = true;
            init_module18_clock = true;
            greendelay = 2000;
            break;  

                
           
           case 0x64:  // maintanence
             
            init_module4_clock = false;
             ledState =  (ledState & SEGDP) | LED_t; 

            init_module5_clock = true;
            init_module6_clock = true;
            init_module7_clock = true;
            init_module8_clock = true; 
            init_module15_clock = true;
            init_module16_clock = true;
            init_module18_clock = true;
            break;

           case 0x6A: // fruit machine
             ledState =  (ledState & SEGDP) | LED_F; 
            init_module15_clock = false;  
            init_module16_clock = false;  
            init_module18_clock = false;
           // init_module16_clock = false;   
            init_module5_clock = true;     
            init_module4_clock = true;
          
            init_module6_clock = true;
            init_module7_clock = true;
            init_module8_clock = true; 
             init_module13_clock = true; 
            break;
          
            
            case 0x6B:   //  get status

      
          default: state = 0x61; break;
      }
    }
  }
//////////////////////////////////////////////////////////////////////////////////////////

  { // module 12 // 2nd SCHEDULER
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module12_clock) {
      module_delay = 10;
      module_time=millis();
      module_doStep=false;
      init_module12_clock=false;
      state=0;
    }
    else {
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }
    {
    static unsigned char old;
    if(old!=state){    //should i change variable name ??
    //  Serial.print("Scheduler state = ");Serial.println(state,DEC);
      old=state;
    }
  }
    if (module_doStep) {
      switch(state_rgb){
          case 0x65:   // RGB OFF

            init_module6_clock = true;  // red
            init_module7_clock = true;  // blue
            init_module8_clock = true;  // amber
            init_module13_clock = true; // green
//             init_module15_clock = true;
//              init_module16_clock = true;
//               init_module18_clock = true;
            digitalWrite(RGB_LED_R, LOW);
            digitalWrite(RGB_LED_G, LOW);
            digitalWrite(RGB_LED_B, LOW);
           
            
            break;
        
          case 0x66:   //  amber
            init_module8_clock = false;
         
            
            init_module6_clock = true;
            init_module7_clock = true; 
            init_module13_clock = true;
//             init_module15_clock = true;
//              init_module16_clock = true;
//               init_module18_clock = true;
            digitalWrite(RGB_LED_B, 0);
            break;      
           
            
           case 0x67:   //   blue
            init_module7_clock = false;
         
            init_module6_clock = true;
            init_module8_clock = true;
            init_module13_clock = true;
//             init_module15_clock = true;
//             init_module16_clock = true;
//             init_module18_clock = true;
            digitalWrite(RGB_LED_R, 0);
            digitalWrite(RGB_LED_G, 0);
            break;  

                
           
           case 0x68:  // green
             
            init_module13_clock = false;

          //  init_module6_clock = true;  // red
          //  init_module7_clock = true;  // blue
         //   init_module8_clock = true;  // amber 

//          init_module15_clock = true;
//              init_module16_clock = true;
//               init_module18_clock = true;
            digitalWrite(RGB_LED_R, LOW);
            digitalWrite(RGB_LED_B, LOW);
            
            break;

           case 0x69: // red
            init_module6_clock = false;

            init_module7_clock = true;
            init_module8_clock = true;
            init_module13_clock = true;
//             init_module15_clock = true;
//              init_module16_clock = true;
//               init_module18_clock = true;
            digitalWrite(RGB_LED_G, 0);
            digitalWrite(RGB_LED_B, 0);
            break;
          
              
          default: state = 0x61; break;
      }
    }
  }


  ///////////////////////////////////////////////////////////////////////////////////////

  
  
      
  ///////////////////////////////////////////////////////////////////////////////////
     
   { // module 0  button 1
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module0_clock) {
      module_delay = 17;
      module_time=millis();
      module_doStep=false;
      init_module0_clock=false;
      state=0;
    }
    else {
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
      switch(state){
          case 0:
            B1_state=notPRESSED;
            if (!digitalRead(_B1)) state = 0;
            else {
              debounce_count = module_time;
              state = 1;
            }
            break;
          case 1:
            B1_state=partialPRESS;
            if (!digitalRead(_B1)) state = 0;
            else if ((long)(millis()-debounce_count) < debounce) state = 1;
            else state = 2;
            break;
          case 2:
            B1_state=normalPRESS;
            if (!digitalRead(_B1)) state = 0;
            else state = 2;
            break;
        
          default: state = 0; break;
      }
    }
  }

 /////////////////////////////////////////////////////////////////////////////////////// 

  { // module 9 button 2
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module9_clock) {
      module_delay = 17;
      module_time=millis();
      module_doStep=false;
      init_module9_clock=false;
      state=0;
    }
    else {
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
      switch(state){
          case 0:
            B2_state=notPRESSED;
            if (!digitalRead(_B2)) state = 0;
            else {
              debounce_count = module_time;
              state = 1;
            }
            break;
          case 1:
            B2_state=partialPRESS;
            if (!digitalRead(_B2)) state = 0;
            else if ((long)(millis()-debounce_count) < debounce) state = 1;
            else state = 2;
            break;
          case 2:
            B2_state=normalPRESS;
            if (!digitalRead(_B2)) state = 0;
            else state = 2;
            break;
        
          default: state = 0; break;
      }
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////

   {
    static char old;
    if(old!=B2_state){
      Serial.print("B2 = ");Serial.println(B2_state?((B2_state==2)?"pressed":"de-bouncing"):"not pressed");
      old=B2_state;
    }
  }

  
  ///////////////////////////////////////////////////////////////////////////////////////////

  { // module 17 button 3
    static unsigned long module_time, module_delay, debounce_count;
    static bool module_doStep;
    static unsigned char state; // state variable for module 0
  
    if (init_module17_clock) {
      module_delay = 17;
      module_time=millis();
      module_doStep=false;
      init_module17_clock=false;
      state=0;
    }
    else {
      unsigned long m=millis();
      if ( ( (long)(m - module_time) ) > module_delay ) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
      switch(state){
          case 0:
            B3_state=notPRESSED;
            if (!digitalRead(_B3)) state = 0;
            else {
              debounce_count = module_time;
              state = 1;
            }
            break;
          case 1:
            B3_state=partialPRESS;
            if (!digitalRead(_B3)) state = 0;
            else if ((long)(millis()-debounce_count) < debounce) state = 1;
            else state = 2;
            break;
          case 2:
            B3_state=normalPRESS;
            if (!digitalRead(_B3)) state = 0;
            else state = 2;
            break;
        
          default: state = 0; break;
      }
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////
//  {
//    static char old;
//    if(old!=B3_state){
//      Serial.print("B3 = ");Serial.println(B3_state?((B3_state==2)?"pressed":"de-bouncing"):"not pressed");
//      old=B3_state;
//    }
//  }

  shiftByteMSF(ledState);
  

} // void loop

// Place any event-driven functions here....
//void requestEvent() {
//  Wire.write(orientation);  /*send orientation on request */
//}

void receiveEvent() 
{ 
  while (0 < Wire.available()) {
    cmd_value = Wire.read(); /* receive byte as a character */
    Serial.print(cmd_value); /* print the character */
  }
  Serial.println(); /* to newline */
   //
  if ((cmd_value == 0x61) || (cmd_value == 0x62) || (cmd_value == 0x63) || (cmd_value == 0x64) || (cmd_value == 0x6A) || (cmd_value == 0x6B))
  {
    state_main = cmd_value;
  }

   if ((cmd_value == 0x65) || (cmd_value == 0x66) || (cmd_value == 0x67) || (cmd_value == 0x68) || (cmd_value == 0x69))
  {
    state_rgb = cmd_value;
  }

}
  

void requestEvent()
{
Wire.write((char)Temp);
Wire.write(tilt);

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
