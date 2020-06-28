#include <Servo.h>

String readString = "";
int count = 0;
int pinCodes[3] = {"...", "...", "..."};
void setup()
{
  servo.attach(7);
  Serial.begin(9600);
}

void loop()
{
  
  if(Serial.available()) // Check if there is Incoming Data in the Serial Buffer.
  {
    count = 0; // Reset count to zero
    while(Serial.available()) // Keep reading Byte by Byte from the Buffer till the Buffer is empty
    {
      char input = Serial.read(); // Read 1 Byte of data and store it in a character variable
      readString+=input;
      Serial.print(input); // Print the Byte
      count++; // Increment the Byte count after every Byte Read
      delay(5); // A small delay 
    }
    Serial.println();

    for(int i=0;i<3;i++)
    {
      if(readString==pinCodes[i])
      {
        servo.write(30*(i+1));
      }
    }
    
    readString = "";
    delay(500);
    servo.write(0);
  }
}