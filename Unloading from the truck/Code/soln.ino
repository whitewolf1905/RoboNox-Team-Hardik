#include <Servo.h>

String readString = "";
int count = 0;
int pinCodes[3] = {"...", "...", "..."};        // 3 pincodes saved in the database
void setup()
{
  servo.attach(7);
  Serial.begin(9600);
}

void loop()
{
  
  if(Serial.available())                      // Check if there is Incoming Data in the Serial Buffer.
  {
    count = 0;                                // Reset count to zero
    while(Serial.available())                 // Keep reading Byte by Byte from the Buffer till the Buffer is empty
    {
      char input = Serial.read();             // Read 1 Byte of data and store it in a character variable
      readString +=input;
      Serial.print(input);                    // Print the Byte
      count++;                                // Increment the Byte count after every Byte Read
      delay(5);                               // A small delay 
    }
    Serial.println();

    for(int i=0;i<3;i++)
    {
      if(readString == pinCodes[0])           // Rotate the Left servo to move in the leftmost direction
      {
        for(int i=0; i<=30; i++)
        {
            left_servo.write(i);
            delay(2);
        }
        delay(200);
        for(int i=30; i>=0; i--)              // Coming back to original position
        {
            left_servo.write(i);
            delay(2);
        }
        
      }
      else if(readString == pinCodes[1])      // No change required because the package has to go in the middle section
      {
                                              // No movement required;
          delay(200);
      }
      else                                    // Rotate the right servo motor in the rightmost direction
      {
        for(int i=0; i<=30; i++)
        {
            right_servo.write(i);
            delay(2);
        }
        delay(200);
        for(int i=30; i>=0; i--)            // Coming back to original position
        {
            right_servo.write(i);
            delay(2);
        }
      }
      
    }
    readString = ""
    delay(500);
  }
}
