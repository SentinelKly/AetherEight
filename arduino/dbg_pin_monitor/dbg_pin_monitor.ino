#define uchar unsigned char
#define uint unsigned int

#define CLOCK_PIN 2
#define RW_PIN 3

//ARDUINO NANO PINOUTS -> D4 - D13 // A0 - A5
const char ADDRESS_PINS[16] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

void setup() 
{
  for (uchar i = 0; i < 16; i++)
  {
    pinMode(ADDRESS_PINS[i], INPUT);
  }

  pinMode(CLOCK_PIN, INPUT);
  pinMode(RW_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), on_clock, RISING);
  
  Serial.begin(57600);
}

void on_clock()
{
  uint faddr = 0;
  char fstr[8] = {0};

  for (uchar i = 0; i < 16; i++)
  {
    uint bit = digitalRead(ADDRESS_PINS[i]) ? 1 : 0;
    faddr = faddr << 1;
    faddr += bit;
  }
  
  sprintf(fstr, "%04X %c\n", faddr, digitalRead(RW_PIN) ? 'R' : 'W');
  Serial.print(fstr);
}

void loop(){}
