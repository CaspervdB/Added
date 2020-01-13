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

bool X_Max = false;                                                                                      //De startrichting voor de motor van de X-as
bool Y_Max = false;                                                                                      //De startrichting voor de motor van de Y-as

int CoordinatesTotal = 4;                                                                                //Totaal aantal coordinaten
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

    //runMotor("Motor_X", CCW, 200);                                                                      //Uitcommenten als je de X motor handmatig wilt aanzetten. Direction moet wel even veranderd worden.
    //runMotor("Motor_Y", CCW, 200);                                                                      //Uitcommenten als je de Y motor handmatig wilt aanzetten. Direction moet wel even veranderd worden.

    GoHome();                                                                                             //Go to start position. coordinate (0,0).

    delay(1000);
    
    Serial.begin(9600); 
}

void loop () {
  MoveTo(Coordinates);                                                                                    //beweegt naar de coordinaten die in de Coordinates[] array staan.
  delay(1000);
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
  if(digitalRead(MOTOR_X_END_STOP) == HIGH)                                                              //When END_STOP is HIGH the X-Axis has reached its max
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

/*
// Function : MoveTo . function: To calculate the amount of steps to move from current location.
// Parameters : int Coordinates[] array of Coordinates.
*/

void MoveTo(int Coordinates[])                                                     //Array containts: Coordinate X, Coordinate Y, Coordinate X, etc.
{
  Serial.println(Coordinates[0]);
  Serial.println(Coordinates[1]);
  
  MoveToCoordinate(Coordinates[0], Coordinates[1]);
  
  Serial.print("Ik ben op: ");
  Serial.print(Coordinates[0]);
  Serial.print(",");
  Serial.println(Coordinates[1]);

  delay(1000);
  
  for (int i = 0; i <= CoordinatesTotal; i = i+2)
  {
    int MoveX = Coordinates[i+2] - Coordinates[i];
    int MoveY = Coordinates[i+3] - Coordinates[i+1];
    Serial.println(MoveX);
    Serial.println(MoveY);
    
    MoveToCoordinate(MoveX, MoveY);

    delay(1000);
    
    Serial.print("Ik ben op: ");
    Serial.print(Coordinates[i+2]);
    Serial.print(",");
    Serial.println(Coordinates[i+3]);
  }
  Serial.println(0 - Coordinates[CoordinatesTotal]);
  Serial.println(0 - Coordinates[CoordinatesTotal + 1]);
  GoHome();
}

/*
// Function : MoveToCoordinate . function: to move the XYZ-Table.
// Parameters :int Coordinate_X is coordinate X, int Coordinate_Y is coordinate Y.
*/

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
}

/*
// Function : GoHome . function: To move the XYZ-Table to Coordinates 0,0(home).
// Parameters :
*/

void GoHome()
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
  Serial.println("Ik ben op: 0,0");
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
