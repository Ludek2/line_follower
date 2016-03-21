/* 
 *  Line Follower robot 
 *  City University Robotics Society 2016
 *  author: Ludek Navratil
 *  
*/

//PWM pins where DC motors are connected
#define LEFT_MOTOR 10
#define RIGHT_MOTOR 11

//number of IR opto transistors 
const uint8_t numberOfInputs=8;
//pin numbers where inputs from IR opto transistors are connected 
uint8_t inputPins[numberOfInputs]={2,3,4,5,6,7,8,9};

//just alias names for values, used in move()
#define FORWARD 0
#define LEFT 1
#define RIGHT 2

//8bit value to store the input from the sensors into one variable
uint8_t inputByte;

// the setup routine runs once when you press reset:
void setup() {
  irArrayInit();
  move(FORWARD,255);
}

// the loop routine runs over and over again forever:
void loop() {
  uint8_t theSpeed=255;
  inputByte=readInputs(inputPins,numberOfInputs);
  
  if((inputByte | 0b01111111) == 0b01111111)
    move(LEFT,theSpeed,100);  
  else if((inputByte | 0b11111110) == 0b11111110)
    move(RIGHT,theSpeed,100); 
    
  else if((inputByte | 0b10111111) == 0b10111111)
    move(LEFT,theSpeed,70); 
  else if((inputByte | 0b11111101) == 0b11111101)
    move(RIGHT,theSpeed,70); 
 
  else if((inputByte | 0b11011111) == 0b11011111)
    move(LEFT,theSpeed,30);
  else if((inputByte | 0b11111011) == 0b11111011)
    move(RIGHT,theSpeed,30); 
    
  else if((inputByte | 0b11101111) == 0b11101111)
    move(FORWARD,theSpeed);
  else if((inputByte | 0b11110111) == 0b11110111)
    move(FORWARD,theSpeed);

  delay(10);
}

//movementSpeed is expressed from min 0 to max 255
void move(uint8_t direction,uint8_t movementSpeed){
  if (direction==FORWARD){
      analogWrite(LEFT_MOTOR, movementSpeed);
      analogWrite(RIGHT_MOTOR, movementSpeed);
  }
}

//movementSpeed is expressed from min 0 to max 255
//turningSpeed is expressed in percentage from min 0 to max 100
void move(uint8_t direction,uint8_t movementSpeed, uint8_t turningSpeed){
  if(turningSpeed<=100){
    double spinningMotor;

    //speed for motor that should be spinning slower to enable turning
    // spinningMotor=movementSpeed-(movementSpeed*(turningSpeed/100))
    spinningMotor=(uint8_t)((double)movementSpeed-((double)movementSpeed*((double)turningSpeed/100)));
  
    //set the speed for both motors
    move(FORWARD, movementSpeed);

    //make one motor slower to reflect turningSpeed percentage
    switch (direction){
      case LEFT:{
        analogWrite(LEFT_MOTOR, spinningMotor);
        analogWrite(RIGHT_MOTOR, movementSpeed);
        break;
      }
      case RIGHT:{
        analogWrite(LEFT_MOTOR, movementSpeed);
        analogWrite(RIGHT_MOTOR, spinningMotor);
        break;
      }
    }
  }
}

//reads logic values that are presented on the pins specified by inputPins[] 
//returns 8bit value that reflect states on the selected pins 
// e.g. if first sensor on the left can see line the function returns input = 0b01111111
uint8_t readInputs(uint8_t inputPins[],uint8_t numberOfInputs){
  uint8_t input=0;
  for(uint8_t i=0;i<numberOfInputs;++i)
    if(digitalRead(inputPins[i])==true)
      input += pw(2,i);

  return input;
}

//Returns value raised to the power exponent:
uint8_t pw(uint8_t value, uint8_t exponent){
  uint8_t result=value;
  if (exponent == 0)result=1;
  else if (exponent == 1) result=value;
  else{
    result=value;
    for(uint8_t i=1; i<exponent; ++i){
      result*=value;
    }
  }
    return result;
}

//set pins specified by array inputPins as input pins
void irArrayInit(){
  for(uint8_t i=0; i<numberOfInputs; ++i)
    pinMode(inputPins[i], INPUT_PULLUP);  
}
