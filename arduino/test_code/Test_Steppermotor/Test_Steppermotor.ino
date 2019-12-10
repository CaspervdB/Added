# define ENABLE_MOTORS            8                                                                     // stepper motor enable , active low
# define MOTOR_X_DIRECTION_PIN    5                                                                     // x -axis stepper motor direction control
# define MOTOR_Y_DIRECTION_PIN    6                                                                    // y -axis stepper motor direction control
# define MOTOR_Z_DIRECTION_PIN    7                                                                     // z axis stepper motor direction control
# define MOTOR_X_STEP_PIN         2                                                                     // x -axis stepper control
# define MOTOR_Y_STEP_PIN         3                                                                     // y -axis stepper control
# define MOTOR_Z_STEP_PIN         4                                                                     // z -axis stepper control
# define MOTOR_Y_END_STOP         10                                                                    // z -axis end stop

volatile char  CW =             HIGH;                                                                   // CW, true clockwise  
volatile char CCW =             LOW;                                                                    // CCW, false counter clockwis 

int hulpje =5;

void setup () {                                                                                 // The stepper motor used in the IO pin is set to output
    pinMode (MOTOR_X_DIRECTION_PIN, OUTPUT); 
    pinMode (MOTOR_X_STEP_PIN, OUTPUT);
    pinMode (MOTOR_Y_DIRECTION_PIN, OUTPUT); 
    pinMode (MOTOR_Y_STEP_PIN, OUTPUT);
    pinMode (MOTOR_Z_DIRECTION_PIN, OUTPUT); 
    pinMode (MOTOR_Z_STEP_PIN, OUTPUT);

   
    pinMode (ENABLE_MOTORS, OUTPUT);
    digitalWrite (ENABLE_MOTORS, LOW);

    Serial.begin(9600); 
}

void loop () {
//    step (false, MOTOR_X_DIRECTION_PIN, MOTOR_X_STEP_PIN, 200);                                   // X axis motor reverse 1 ring, the 200 step is a circle.
//    step (false, MOTOR_Y_DIRECTION_PIN, MOTOR_Y_STEP_PIN, 200);                                   // y axis motor reverse 1 ring, the 200 step is a circle.
//    delay (1000);
//    step (true, MOTOR_Y_DIRECTION_PIN, MOTOR_Y_STEP_PIN, 10);                                    // X axis motor forward 1 laps, the 200 step is a circle.
//    step (MOTOR_Y_DIRECTION_PIN, MOTOR_Y_STEP_PIN, 50);                                    // y axis motor forward 1 laps, the 200 step is a circle.
    runMotor("Motor_X", CCW, 200);
    runMotor("Motor_Y", CCW, 200);
    delay (1000);
//    runMotor("Motor_X", CW, 200);
//    runMotor("Motor_Y", CW, 200);
//    delay (1000);

///String help = String(hulpje);

//Serial.write("prutje" + help +" \n");
} 

/*
// Function : step . function: to control the direction of the stepper motor , the number of steps .
// Parameters : dir direction control , dirPin corresponding stepper motor DIR pin , stepperPin corresponding stepper motor " step " pin , Step number of step of no return value.
*/
void step (byte dirPin, byte stepperPin, int steps)
{
    digitalWrite (dirPin, LOW);      // Richting en as
    delay (50);
    for (int i = 0; i <steps; i ++) 
    {
        digitalWrite (stepperPin, HIGH);
        delayMicroseconds (800);
        digitalWrite (stepperPin, LOW);
        delayMicroseconds (800);
    }
}


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
