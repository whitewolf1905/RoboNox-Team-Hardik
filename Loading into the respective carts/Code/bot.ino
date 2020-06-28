// #include <LiquidCrystal.h>
// LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

#define DT A0
#define SCK A1
#define sw 2


int t= 5000;                                                 // This is the calculated time taken by DC motor to lift the platform to the cart level

long sample = 0;
float val = 0;
long count = 0;
long threshold = 123456;                                          // Some threshold weight
int dist = 0;
int distFactor = 123456;                                          // A calculated constant for incrementing value of distance travelled

unsigned long readCount(void)
{
    unsigned long Count;
    unsigned char i;
    pinMode(DT, OUTPUT);
    digitalWrite(DT,HIGH);
    digitalWrite(SCK,LOW);
    Count=0;
    pinMode(DT, INPUT);
    while(digitalRead(DT));
    for (i=0;i<24;i++)
    {
        digitalWrite(SCK,HIGH);
        Count=Count<<1;
        digitalWrite(SCK,LOW);
        if(digitalRead(DT)) 
        Count++;
    }
    digitalWrite(SCK,HIGH);
    Count=Count^0x800000;
    digitalWrite(SCK,LOW);
    return(Count);
}

void setup()
{
    Serial.begin(9600);
    
    // For Weight Sensor
    pinMode(SCK, OUTPUT);
    pinMode(sw, INPUT_PULLUP);
    calibrate();

    // For Motors
    pinMode(13, OUTPUT); //left motors forward
    pinMode(12, OUTPUT); //left motors reverse
    pinMode(11, OUTPUT); //right motors forward
    pinMode(10, OUTPUT); //right motors reverse
    pinMode(9, OUTPUT);  //DC motor to lift upward
    pinMode(8, OUTPUT);  //DC motor to lower downwards
}

void loop()
{
    count = readCount();                                                          // Using a weight sensor to verify that package has been placed on one of the bots
    int w = (((count - sample) / val) - 2 * ((count - sample) / val));
    Serial.print("weight:");
    Serial.print((int)w);
    Serial.println("g");

    if(w>threshold){                                                              // If weight is more than threshold, bot will start moving towards the respective Cart destination.
        fd50();
        unload();
        bk50();
        delay(200);
    }

    if (digitalRead(sw) == 0)
    {
        val = 0;
        sample = 0;
        w = 0;
        count = 0;
        calibrate();
    }
}

void calibrate()
{
    for (int i = 0; i < 100; i++)
    {
        count = readCount();
        sample += count;
        Serial.println(count);
    }
    sample /= 100;
    Serial.print("Avg:");
    Serial.println(sample);
    // "Put 100g & wait"
    count = 0;
    while (count < 1000)
    {
        count = readCount();
        count = sample - count;
        Serial.println(count);
    }
    delay(2000);
    for (int i = 0; i < 100; i++)
    {
        count = readCount();
        val += sample - count;
        Serial.println(sample - count);
    }
    val = val / 100.0;
    val = val / 100.0; // put here your calibrating weight
}

void fd50()
{
    dist = 0;
    while(dist<5000)
    {
        digitalWrite(13, HIGH);
        digitalWrite(11, HIGH);
        dist+=distFactor;
    }
    delay(200);
}

void unload()
{
    int time_up = t;
    while(time_up--)                              // Lifting the platform upwards to reach the level of Cart
    {
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
    }
    delay(2000);
    int time_down = t;
    while(time_down--)                           // Lowering the platform downwards to reach the level of bot
    {
      digitalWrite(9, LOW);
      digitalWrite(8, HIGH);
    }
    delay(2000);
}

void bk50()
{
    dist = 0;
    while (dist < 5000)
    {
        digitalWrite(12, HIGH);
        digitalWrite(10, HIGH);
        dist += distFactor;
    }
}
