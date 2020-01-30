// Linear stepper motor driver for bin selector coin selector

#include <AccelStepper.h>                                                                                               // Include the stepper motor library

// Define preprocessor directives stepper motor connections and steps per revolution:
#define MOTOR_X_DIRECTION_PIN                   5                                                                       // Rotation direction stepper motor ClockWise (CW) of CounterClockWise (CCW)
#define MOTOR_X_STEP_PIN                        2                                                                       // Stepper motor clockpulse 

AccelStepper motor_X(1, MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);                                                       // Create X-motor

#define END_SWITCH_RIGHT                        8                                                                       // End switch right side

#define OFF                                     0                                                                       // Control is switched off
#define CALIBRATION_MODE                        1                                                                       // Calibrate the zero point  
#define RUNNING_MODE                            2                                                                       // Steps to a certain position  (positive and negative) 


#define PITCH                                   3                                                                       // Traveling distance in mm for 1 rotation
#define MAXIMUM_TRAVELING_DISTANCE              -200                                                                    // Maximum traveling distance of the spindle in mm
#define STEPS_PER_REVOLUTION                    200                                                                     // Stepper amount of pulses for 1 revolution  
#define MAX_MOTOR_SPEED                         1500
#define MINIMUM_AMOUNT_1US_PULSES               1000                                                                    // Minimum amout of interrupt counts that are required that the motor needs to get started load included
                                                                                                                        // The minimum pulse time for this motor is 1  millisecond so 1000 pulses need te be counted. So: MAY NOT 
                                                                                                                        // BE SMALLER THEN <1000 OR > 65536
                                                                                                                        // Because the timer is a 16 bit timer the maximum setting in 65535
                                                                                                                        // So a seeting of 1000 gives each millisecond 1 step so with a 200 steps motor eacht 1/5 af a second a 
                                                                                                                        // rotation (is 5 rotations per second)
                                                                                                                        // So for 1 rotation per second the number need to be 5 * 1000 = 5000  
                                                                                                                        // So for 1 rotation per 2 seconds the number need to be 10 * 1000 = 10000
                                                                                                                        // So for 1 rotation per 4 seconds the number need to be 20 * 1000 = 20000
                                                                                                                        // The formula for the needed speed then becomes: abs(100000/calculatedActualPercentage) 



// Declare variables
volatile unsigned int controlMode               = OFF;                                                                  // 0 = off, 1 = calibration, 2 = running mode
volatile boolean endSwitchRightActivated        = false;                                                                // Calibration must be done before running other programs

volatile unsigned int zeroPointOffsetInMm       = 5;                                                                    // Offset to the left zero point to end switch right in mm 
volatile int newRulerPositionInMm               = 0;                                                                    // New ruler position in mm (xxx = xxx mm so -145 = -145 mm = -14,5 cm)  
volatile long newRulerPositionInSteps           = 0;                                                                    // New ruler position in steps

String inputString                              = "";                                                                   // String to hold incoming data of the computer to the arduino
String commandTypeString                        = "blabla";                                                                  // String with the command send by the computer        
volatile boolean sendedStringComplete           = false;                                                                // See of the received string is complete

volatile boolean getNewDataFlag                 = false;                                                                // Get new data from the serial bus  
volatile boolean newDataAvailableFlag           = true;                                                                 // New available data on the serial bus

volatile long hulpje                            = 0L;

  String dataString = "";
volatile unsigned long delayTimer              = 0;

void setup() 
{
  cli();                                                                                                                // Stop the interrupts
  
  // Declare pins as output:
  pinMode(MOTOR_X_STEP_PIN , OUTPUT);
  pinMode(MOTOR_X_DIRECTION_PIN, OUTPUT);

  // Declare pins as input:
  pinMode(END_SWITCH_RIGHT, INPUT);                                                                                     // Used as normal and interrupt input

  // Initialize the external interrupts

  attachInterrupt(digitalPinToInterrupt(END_SWITCH_RIGHT),ISRcalibrationPointEmercengyStopRight,RISING);                // Start Interrupt service routine calibrationPointEmercengyStopRight
 
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------                                                                                
    
  // Timer 0 is used for delay, millis, and micros so don't use it for anything else 
   
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------                                                                                
      
  // Set timer 1 interrupt timer as a scheduler control CTC mode with clk/256 prescaler so one tick every 33,33333 micro seconds

  TCCR1A = 0;                                                                                                           // Set entire TCCR1A register to 0
  TCCR1B = 0;                                                                                                           // Set entire TCCR1B register to 0
  TCNT1 = 0;                                                                                                            // Reset counter value
  OCR1A =  MINIMUM_AMOUNT_1US_PULSES;                                                                                   // Set compare match register for...........   (OCRn =  [ (clock_speed / Prescaler_value) * 
                                                                                                                        // Desired_time_in_Seconds ] - 1) (this is the wanted motorspeed)
  TCCR1B |= (1 << WGM12 | 1 << CS11);                                                                                   // Set mode 4 OCR1A to CTC and 8 for prescaler 
  TIFR1 |= (1 << OCF1A);                                                                                                // Clear interrupt flag OCF1A
  TIMSK1 |= (1 << OCIE1A);                                                                                              // Enable timer compare interrupt, interrupt on Compare A Match

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------                                                                                
     

  // Initialize and start serial communication
  Serial.begin(9600);                                                                                                   // Starts the serial communication
  sei();                                                                                                                // Allow interrupts

  controlMode = CALIBRATION_MODE;
}


void ISRcalibrationPointEmercengyStopRight()                                                                            // Interrupt Service Routine for calibration
{                   
  
  if(controlMode == CALIBRATION_MODE)
  {
    endSwitchRightActivated = true;
  }
}

 
ISR (TIMER1_COMPA_vect)                                                                                                 // Interrupt service routine timer 1: free running timer    
{
    
    switch(controlMode)
    {
      case CALIBRATION_MODE : 
              calibrationMode();
              break;
      case RUNNING_MODE: 
              runningMode();
              break;        
      default: break;
    }
  
  TIFR1 = 1 << OCF1A;                                                                                                  // Clear the Output Compare A Match Flag timer 1
}

  
void calibrationMode()
{
  if(digitalRead(END_SWITCH_RIGHT))                                                                                   // Head is already in the most right position (be aware, now used as an normal input but als as an interrupt
  {                                                                                                                   // input.)
    endSwitchRightActivated = true;
  }

  if(!endSwitchRightActivated)                                                                                        // table not at the most right side, move to the right
  {
    motor_X.setSpeed(500);                                                                                            // motor must run to the right so positive number
    motor_X.runSpeed();
    motor_X.setCurrentPosition((zeroPointOffsetInMm * STEPS_PER_REVOLUTION)/ PITCH);                                  // The ofset for the zero point to the endswitch        
  }

  else
  {
     if(delayTimer < 500)                                                                                             // Create an delay of 0.5 second
     {
       delayTimer++;
     }
     else
     {
        motor_X.moveTo(0);                                                                                            // Move to the zero point
        motor_X.setSpeed(-500);                                                                                       // motor must run to the left so negative number 
        motor_X.run();
        controlMode = RUNNING_MODE;
     }
  }
}

void runningMode()
{
  motor_X.moveTo(newRulerPositionInSteps);
  
  if(newRulerPositionInSteps > 0)                                                                                    // Motor want's to go through the zero point
  {
    newRulerPositionInSteps = 0;    
  }
  
  if(newRulerPositionInSteps < motor_X.currentPosition())                                                            // motor must run to the left
  {
    motor_X.setSpeed(MAX_MOTOR_SPEED * -1); 
  }
    
  else
  {
    motor_X.setSpeed(MAX_MOTOR_SPEED);                                                                                // motor must run to the right
  }
      
      
  

  
   
  motor_X.run();
    
    
   
 
  
}

void serialEvent()                                                                                                    // New serial information available
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();                                                                                // Get the new byte:
    inputString += inChar;                                                                                            // Add it to the inputString:
    
    if(inChar == '\n')                                                                                                // If the incoming character is a newline, string is complete
    {
      sendedStringComplete = true;
    }
  }
}



void getStringValues()                                                                                                // Get the string with new values
{                                                                                                                     // c:ppp: c = command type (o=off, nrp= new ruler postion0, ppp = position (must end with :)
                                                                                                                      // It uses t x-as as reference, the zero point is at the right side so a momevent to the left side is
                                                                                                                      // an negative number. So to move 100 mm to the left the command must be: nrp:-100: 
  int x = 0;
  int separatorSignPosition[5];
  int separatorSignNumber = 0;
    
  for(x = 0; x < inputString.length(); x++)                                                                           // Search for the position of the separator sign :
  {
    if(inputString.charAt(x) == ':')
    {
      separatorSignPosition[separatorSignNumber] = x;
      separatorSignNumber++;
    }
  }
   
  commandTypeString = inputString.substring(0,separatorSignPosition[0]); 

    
  if(commandTypeString == "o")  // OFF
  {
    controlMode = OFF;
  }

  if(commandTypeString == "nrp")  // New ruler position
  {
      newRulerPositionInMm = (inputString.substring(separatorSignPosition[0] + 1,separatorSignPosition[1]).toInt());    // New ruler position in mm (xxx = xx.x mm so 145 = 14.5 mm)
      
      if(newRulerPositionInMm < MAXIMUM_TRAVELING_DISTANCE)
      {
        newRulerPositionInMm = MAXIMUM_TRAVELING_DISTANCE;
      }
      newRulerPositionInSteps = ((long)newRulerPositionInMm *  STEPS_PER_REVOLUTION / PITCH);
      
  }
  /*else
    controlMode = CALIBRATION_MODE;

  hulpString = inputString.substring(separatorSignPosition[0] + 1,separatorSignPosition[1]);
/*
  if(commandTypeString == "s")  // CALIBRATE MODE
  {
    
  
    if(inputString.substring(separatorSignPosition[0] + 1,separatorSignPosition[1]) == "r")
    {
      calibrationMode = RIGHT_CALIBRATION;
    }

    else if(inputString.substring(separatorSignPosition[0] + 1,separatorSignPosition[1]) == "l")
    {
      calibrationMode = LEFT_CALIBRATION;
    }

    else
      calibrationMode = STOP_CALIBRATION;
   
    controlMode = CALIBRATION_MODE;
  }

  if(totalCalibrationDone)
  {
    if(commandTypeString == "p")  // STEPPER MODE
    {
      newRulerPositionInMmTicks = 8 * (inputString.substring(separatorSignPosition[0] + 1,separatorSignPosition[1]).toInt());    // New ruler position in mm (xxx = xx.x mm so -145 = 14.5 mm)
    
      stepAmount = newRulerPositionInMmTicks - rulerPositionTicks;
      controlMode = STEPPER_MODE;
    } 
  }*/
 
  //newDataAvailableFlag = true;


    // !!!!!!!!!!!!!!!!  check op calibration done
    
  }

void writeString(String stringData)                                                                                   // Used to serially push out a String with Serial.write()
{
  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);                                                                                      // Push each char 1 by 1 on each loop pass
  }

}


void loop() 
{
  
  getStringValues();
  inputString = "";    

     //dataString = "fase " + String(hulpje) + " Target Position: " + String(motor_X.targetPosition()) + " control mode: " + String(controlMode) ;
      //writeString(dataString);
      // Serial.write("\n");  
     dataString = "Speel: " + String(motor_X.speed()) + " Target Position: " + String(motor_X.targetPosition()) + " Current Position: " + String(motor_X.currentPosition()) + "Hulpje" + String(newRulerPositionInSteps)  ;
      
    
      
      writeString(dataString);
      
       Serial.write("\n");  
   
    //writeString("help\n");

}
