/*
  This is the first attempt at communicating with Qwiic Gpio.

  Created May, 4th 2018

  Author: Kevin Kuwat
*/

#include <Wire.h>

/* I2C Command Byte Defines  */
#define REGISTER_INPUT_PORT           0x00    //register 0 
#define REGISTER_OUTPUT_PORT          0X01    //register 1
#define REGISTER_POLARITY_INVERSION   0x02    //register 2
#define REGISTER_CONFIURATION         0X03    //register 3


/* Configuration Register:
  set to 0 for output
  set to 1 for input (default).
*/


byte const qwiicGpioAddress = 0x27; //When all jumpers are closed A0, A1, A2 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start");
  Wire.begin();

  testForConnectivity();

  //configure port as output, so inverted logic. 
  Wire.beginTransmission(qwiicGpioAddress);
  Wire.write(REGISTER_CONFIURATION);
  Wire.write(0x80); //first 7 are outputs, 8th is an input.
  Wire.endTransmission();
}


//#define GPIO_OUTPUT_ON_OFF

void loop() {
	
#ifdef GPIO_OUTPUT_ON_OFF
  // put your main code here, to run repeatedly:
  delay(100);

  Wire.beginTransmission(qwiicGpioAddress);
  Wire.write(REGISTER_OUTPUT_PORT);
  Wire.write(0x00); //LOW
  Wire.endTransmission();
  
  delay(100);
  
  Wire.beginTransmission(qwiicGpioAddress);
  Wire.write(REGISTER_OUTPUT_PORT);
  Wire.write(0xFF); //HIGH
  Wire.endTransmission();
  #endif
  delay(100);
  
  
  
	//read the button switch
	/*
		* write to the address
		* write the register address:  REGISTER_INPUT_PORT
		* *** repeated start, end transmission(false)
		* Read from the address , get an ack back, 
		* then we can do a read with while wire.available();
	*/
  byte portPinReadByte = 54;
  Wire.beginTransmission(qwiicGpioAddress);
  Wire.write(REGISTER_INPUT_PORT);
  Wire.endTransmission(false);
  Wire.requestFrom(qwiicGpioAddress, 1);
  
  byte count = 0;
  while(Wire.available() > 0){
	if(count == 0){
	portPinReadByte = Wire.read();
	}
	count++;
	Wire.read(); //don't collect
  }
  Serial.println("test >> ");
  Serial.print(portPinReadByte);
  
  /*while(Wire.available()){
   portPinReadByte = Wire.read();
  }
  Wire.endTransmission();
  
	Serial.print("test >>> ");
  	Serial.println(portPinReadByte);

  
  //(portPinReadByte & 0b10000000) == 0x80
  
  if(portPinReadByte == 1 ){
	//case its a 1 
	Serial.print("case 1");
  }
  else if(portPinReadByte == 1){
	//(portPinReadByte & 0b10000000) == 0x00)
	  	Serial.println("case 00");
  }
  
  
  delay(10);
  
  


  
  
  
//collect response.
  if (Wire.available() == 0) return 0;
  byte response =  Wire.read();
   Serial.print("we got: ");
   Serial.print(response, 2); //binary
*/
}



// testForConnectivity() checks for an ACK from an Relay. If no ACK
// program freezes and notifies user.
void testForConnectivity() {
  Wire.beginTransmission(qwiicGpioAddress);
  //check here for an ACK from the slave, if no ack don't allow change?
  if (Wire.endTransmission() != 0) {
    Serial.println("Check Connections. No slave attached.");
    while (1);
  }
}

/*
  Send data to slave
    - Start and address
    - Send data
    - Stop conddition

  Read data from slave
    - Start and address
    - send requested register
    - mast rx (this would be the request # of bytes)..
        - like the getFirmwareVersion() function below


  //getFirmwareVersion() returns the firmware version as a float.
  float getFirmwareVersion() {
  Wire.beginTransmission(address);
  Wire.write(__REGISTER___);
  Wire.endTransmission();

  Wire.requestFrom(qwiicRelayAddress, 2); //2 BYTES for the Version Number

//collect response.
  if (Wire.available() == 0) return 0;
  float versionNumber = Wire.read();
  versionNumber += (float)Wire.read() / 10.0;

  return (versionNumber);
  }




*/

