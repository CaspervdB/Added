# define ENABLE_MOTORS            8                                                                     // stepper motor enable , active low
# define MOTOR_X_DIRECTION_PIN    5                                                                     // x -axis stepper motor direction control
# define MOTOR_Y_DIRECTION_PIN    6                                                                     // y -axis stepper motor direction control
# define MOTOR_Z_DIRECTION_PIN    7                                                                     // z axis stepper motor direction control
# define MOTOR_X_STEP_PIN         2                                                                     // x -axis stepper control
# define MOTOR_Y_STEP_PIN         3                                                                     // y -axis stepper control
# define MOTOR_Z_STEP_PIN         4                                                                     // z -axis stepper control
# define MOTOR_X_END_STOP         9                                                                     // x -axis end stop
# define MOTOR_Y_END_STOP         10                                                                    // y -axis end stop

volatile char  CW =             HIGH;                                                                   // CW, true clockwise  
volatile char CCW =             LOW;                                                                    // CCW, false counter clockwis 

int hulpje = 5;
bool X_Max = false;                                                                                      //De startrichting voor de motor van de X-as
bool Y_Max = false;                                                                                      //De startrichting voor de motor van de Y-as
int CoordinatesTotal = 6;                                                                                //Totaal aantal coordinaten * 2
int Coordinates[] = {5, 6, 1, 3, 7, 2, 1, 1};                                                            //De coordinaten X, Y, X, Y

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
    digitalWrite (ENABLE_MOTORS, LOW);

    GoToZero();                                                                                           //Go to start position

    delay(10000);

//    MoveTo(Coordinates);
//
//    delay(1000);
    
    Serial.begin(9600); 
}

void loop () {
  
  MoveTo(Coordinates);

  delay(1000);
  
  //Turn_Y_Axis();
  
  //Turn_X_Axis();
  
  //MoveToCoordinate(5,1);

  //delay(1000);

//  GoToZero();
//
//  delay(1000);
//  
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
// Function : Turn_X_Axis . function: To make the X-Axis move .
// Parameters : /.
*/

void Turn_X_Axis()
{
  X_AxisDirectionChange();
  
  if(X_Max == true)
  {
    runMotor("Motor_X", CCW, 200);                                                                        //Turns X-Axis counterClockWise
  }else{
    runMotor("Motor_X", CW, 200);                                                                         //Turns the X-Axis clockWise
  }
}

/*
// Function : X_AxisDirectionChange . function: To change direction of the X-Axis when it has reached its max .
// Parameters : /.
*/

void X_AxisDirectionChange()
{
  if(digitalRead(MOTOR_Y_END_STOP) == HIGH)                                                              //When END_STOP is HIGH the X-Axis has reached its max
  {
    runMotor("Motor_X", CCW, 0);                                                                         //Stops the X-axis from moving
    Serial.println("De X-as is op een maximale waarde");
    X_Max = !X_Max;                                                                                      //Changes the direction of movement for the X-Axis
  }else{
    Serial.println("X niet op de max");
  }
}

/*
// Function : Turn_Y_Axis . function: To make the Y-Axis move .
// Parameters : /.
*/

void Turn_Y_Axis()
{
  Y_AxisDirectionChange();
  
  if(Y_Max == true)
  {
    runMotor("Motor_Y", CCW, 200);                                                                        //Turns the Y-Axis counterClockWise
  }else{
    runMotor("Motor_Y", CW, 200);                                                                         //Turns the Y-Axis clockWise
  }
}

/*
// Function : Y_AxisDirectionChange . function: To change direction of the Y-Axis when it has reached its max .
// Parameters : /.
*/

void Y_AxisDirectionChange()
{
  if (digitalRead(MOTOR_Y_END_STOP) == HIGH)                                                              //When END_STOP is HIGH the Y-Axis has reached its max
  {
    runMotor("Motor_Y", CCW, 0);                                                                          //Stops the X-axis from moving. Could be removed???
    Serial.println("De X-as is op een maximale waarde");
    Y_Max = !Y_Max;                                                                                       //Changes the direction of movement for the Y-Axis
  }else{
    Serial.println("Y is niet op de max");
  }
}

void MoveTo(int Coordinates[])                                                     //Array containts: Coordinate X, Coordinate Y, Coordinate X, etc.
{
  for (int i = 0; i <= (CoordinatesTotal - 1); i++)
  {
    for (int j = (CoordinatesTotal - 1); j >= 0 && j > i; j--)
    {
      if((i % 2) == 0 && (j % 2) == 0)
      {
        Coordinates[j] = Coordinates[j] - Coordinates[i];
      } 
      if((i % 2) == 1 && (j % 2) == 1)
      {
        Coordinates[j] = Coordinates[j] - Coordinates[i];
      }
//    int Coordinates[6] = {5, 6, 1, 3, 7, 2}; 
//    sizeof(Coordinates) / sizeof(Coordinates[0])
//    Serial.println(Coordinates[i]);
//    Serial.println(Coordinates[j]);
      
//      delay(10000);
    }
  }

  for (int i = 0; i <= (CoordinatesTotal - 1); i = i+2)
  {
    MoveToCoordinate(Coordinates[i], Coordinates[i+1]);
    Serial.println(Coordinates[i]);
    Serial.println(Coordinates[i+1]);
    delay(10000);
  }
  GoToZero();
}

void MoveToCoordinate(int Coordinate_X, int Coordinate_Y)
{
  if(Coordinate_Y > 0)
  {
    runMotor("Motor_Y", CW, 200);
    Coordinate_Y = Coordinate_Y - 1;
    delay(1000);
  }
  if(Coordinate_X > 0)
  {
    runMotor("Motor_X", CW, 200);
    Coordinate_X = Coordinate_X - 1;
    delay(1000);
  }
  
  while(Coordinate_Y != 0 || Coordinate_X != 0)
  {
    if(Coordinate_Y > 0)
    {
      if(digitalRead(MOTOR_Y_END_STOP) == LOW)
      {
        runMotor("Motor_Y", CW, 200);
        delay(1000);
      }
      Coordinate_Y = Coordinate_Y - 1;
    }
    if(Coordinate_Y < 0)
    {
      if(digitalRead(MOTOR_Y_END_STOP) == LOW)
      {
        runMotor("Motor_Y", CCW, 200);
        delay(1000);
      }
      Coordinate_Y = Coordinate_Y + 1;
    }
    if(Coordinate_X > 0)
    {
      if(digitalRead(MOTOR_X_END_STOP) == LOW)
      {
        runMotor("Motor_X", CW, 200);
        delay(1000);
      }
      Coordinate_X = Coordinate_X - 1;
    }
    if(Coordinate_X < 0)
    {
      if(digitalRead(MOTOR_X_END_STOP) == LOW)
      {
        runMotor("Motor_X", CCW, 200);
        delay(1000);
      }
      Coordinate_X = Coordinate_X + 1;
    }
  }
//  GoToZero();
}

void GoToZero()
{
  while(digitalRead(MOTOR_Y_END_STOP) == LOW || digitalRead(MOTOR_X_END_STOP) == LOW)
  {
    if(digitalRead(MOTOR_Y_END_STOP) == LOW)
    {
      runMotor("Motor_Y", CCW, 200);
    }
    if(digitalRead(MOTOR_X_END_STOP) == LOW)
    {
      runMotor("Motor_X", CCW, 200);
    }
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
