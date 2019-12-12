# define ENABLE_MOTORS            8                                                                     // stepper motor enable , active low
# define MOTOR_X_DIRECTION_PIN    5                                                                     // x -axis stepper motor direction control
# define MOTOR_Y_DIRECTION_PIN    6                                                                     // y -axis stepper motor direction control
# define MOTOR_Z_DIRECTION_PIN    7                                                                     // z axis stepper motor direction control
# define MOTOR_X_STEP_PIN         2                                                                     // x -axis stepper control
# define MOTOR_Y_STEP_PIN         3                                                                     // y -axis stepper control
# define MOTOR_Z_STEP_PIN         4                                                                     // z -axis stepper control
# define MOTOR_X_END_STOP         9                                                                    // x -axis end stop
# define MOTOR_Y_END_STOP         10                                                                    // y -axis end stop

volatile char  CW =             HIGH;                                                                   // CW, true clockwise  
volatile char CCW =             LOW;                                                                    // CCW, false counter clockwis 

int hulpje =5;
bool X_Max = false;                                                                                      //De startrichting voor de motor van de X-as
bool Y_Max = false;                                                                                      //De startrichting voor de motor van de Y-as
String test;
void setup () {                                                                                         // The stepper motor used in the IO pin is set to output
    pinMode (MOTOR_X_DIRECTION_PIN, OUTPUT); 
    pinMode (MOTOR_X_STEP_PIN, OUTPUT);
    pinMode (MOTOR_Y_DIRECTION_PIN, OUTPUT); 
    pinMode (MOTOR_Y_STEP_PIN, OUTPUT);
    pinMode (MOTOR_Z_DIRECTION_PIN, OUTPUT); 
    pinMode (MOTOR_Z_STEP_PIN, OUTPUT);
    pinMode (MOTOR_Y_END_STOP, INPUT);
    pinMode (MOTOR_X_END_STOP, INPUT);
     

   
    pinMode (ENABLE_MOTORS, OUTPUT);
    digitalWrite (ENABLE_MOTORS, HIGH);                                                                   // LOW zet de motor aan en HIGH zet hem uit
//    digitalWrite (ENABLE_MOTORS, LOW);

    Serial.begin(9600); 
}

void loop () {

  Y_Aansturen();
  
  X_Aansturen();

  delay(1000);
  
//    step (false, MOTOR_X_DIRECTION_PIN, MOTOR_X_STEP_PIN, 200);                                        // X axis motor reverse 1 ring, the 200 step is a circle.
//    step (false, MOTOR_Y_DIRECTION_PIN, MOTOR_Y_STEP_PIN, 200);                                        // y axis motor reverse 1 ring, the 200 step is a circle.
//    delay (1000);
//    step (true, MOTOR_Y_DIRECTION_PIN, MOTOR_Y_STEP_PIN, 10);                                          // X axis motor forward 1 laps, the 200 step is a circle.
//    step (MOTOR_Y_DIRECTION_PIN, MOTOR_Y_STEP_PIN, 50);                                                // y axis motor forward 1 laps, the 200 step is a circle.
//    runMotor("Motor_X", CCW, 200);
//    runMotor("Motor_Y", CCW, 200);
//    delay (1000);
//    runMotor("Motor_X", CW, 200);
//    runMotor("Motor_Y", CW, 200);
//    delay (1000);

//  Serial.println(digitalRead(MOTOR_Y_END_STOP));
///String help = String(hulpje);

//Serial.write("prutje" + help +" \n");
} 

/*
// Function : X_Aansturen . function: om tussen de maximalen van de X-as heen en weer te blijven bewegen .
// Parameters : /.
*/

void X_Aansturen()
{
  if(digitalRead(MOTOR_Y_END_STOP) == HIGH)                                                              //Als de END_STOP HIGH is dan is zit hij op zijn max.
  {
    runMotor("Motor_X", CCW, 0);                                                                         //Het stilzetten van x-as de motor
    Serial.println("De X-as is op een maximale waarde");
    X_Max = !X_Max;                                                                                      //Het omdraaien van de richting van de X-as
  }else{
    Serial.println("X niet op de max");
  }
  
  if(X_Max == true)
  {
    runMotor("Motor_X", CCW, 200);                                                                        //De motor counterClockWise aanzetten
  }else{
    runMotor("Motor_X", CW, 200);                                                                         //De motor clockWise aanzetten
  }
}

/*
// Function : Y_Aansturen . function: om tussen de maximalen van de Y-as heen en weer te blijven bewegen .
// Parameters : /.
*/

void Y_Aansturen()
{
  if (digitalRead(MOTOR_Y_END_STOP) == HIGH)                                                              //Als de END_STOP HIGH is dan is zit hij op zijn max.
  {
    runMotor("Motor_Y", CCW, 0);                                                                          //Het stilzetten van de Y-as motor
    Serial.println("De X-as is op een maximale waarde");
    Y_Max = !Y_Max;                                                                                       //Het omdraaien van de richting van de Y-as
  }else{
    Serial.println("Y is niet op de max");
  }

  if(Y_Max == true)
  {
    runMotor("Motor_Y", CCW, 200);                                                                        //De motor counterClockWise aanzetten
  }else{
    runMotor("Motor_Y", CW, 200);                                                                         //De motor clockWise aanzetten
  }
}

/*
// Function : step . function: to control the direction of the stepper motor , the number of steps .
// Parameters : dir direction control , dirPin corresponding stepper motor DIR pin , stepperPin corresponding stepper motor " step " pin , Step number of step of no return value.
*/
void stapje (byte stepperPin, int steps)
{
    delay (50);
    for (int i = 0; i <steps; i ++) 
    {
        digitalWrite (stepperPin, HIGH);
        delayMicroseconds (800);
        digitalWrite (stepperPin, LOW);
        delayMicroseconds (800);
    }
}


void runMotor(String motor, char motorDirection, int steps)
{
  /*if(motor.indexOf("Motor_X"))
  {
    if(motorDirection)
    {
     //digitalWrite(MOTOR_X_DIRECTION_PIN,motorDirection);
     digitalWrite(MOTOR_X_DIRECTION_PIN,LOW);
     //digitalWrite(MOTOR_X_DIRECTION_PIN,HIGH);
     stapje(MOTOR_X_STEP_PIN, steps);
    }
  }*/

  if(motor.indexOf("Motor_X") > -1)
  {
     digitalWrite(MOTOR_X_DIRECTION_PIN, motorDirection);
     stapje(MOTOR_X_STEP_PIN, steps);
  }

//  if(MOTOR_Y_END_STOP == false)
//  {
    if(motor.indexOf("Motor_Y") > -1)
    {
       digitalWrite(MOTOR_Y_DIRECTION_PIN, motorDirection);
       stapje(MOTOR_Y_STEP_PIN, steps);
    }
//  }
}
