#define WRITE_DATA 2
#define REG_LATCH 3
#define SHIFT_CLK 4
#define OUT_ENABLE 5
#define WRITE_ENABLE 14

#define DATA_0 6
#define DATA_7 13

unsigned int count = 0x0000;

//Sets an address (up to 0xFFFF) on the 16bit shift register bus
void set_address(unsigned int addr)
{
  shiftOut(WRITE_DATA, SHIFT_CLK, MSBFIRST, (addr >> 8));
  shiftOut(WRITE_DATA, SHIFT_CLK, MSBFIRST, addr);
  
  digitalWrite(REG_LATCH, LOW);
  digitalWrite(REG_LATCH, HIGH);
  digitalWrite(REG_LATCH, LOW);
}

//Enables/disables EEPROM OE pin
void output_enable(bool enable)
{
  digitalWrite(OUT_ENABLE, (enable) ? LOW : HIGH);
}

//Returns a single 8bit value from an address
byte read_byte_eeprom(unsigned int addr)
{
  byte fdata = 0;
  
  set_address(addr);
  output_enable(true);

  for (int pin = DATA_7; pin >= DATA_0; pin--)
  {
    pinMode(pin, INPUT);
    fdata = (fdata << 1) + digitalRead(pin);
  }

  return fdata;
}

//Writes a single 8bit value to an address
void write_byte_eeprom(unsigned int addr, byte data)
{
  set_address(addr);
  output_enable(false);

  for (int pin = DATA_0; pin <= DATA_7; pin++)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }

  digitalWrite(WRITE_ENABLE, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_ENABLE, HIGH);
  delay(10);
}

void setup() 
{
  digitalWrite(WRITE_ENABLE, HIGH);
    
  //Set D2:D13 and A0 to digital output
  for (int pins = 2; pins < 15; pins++)
  {
    pinMode(pins, OUTPUT);
  }

  //Enable serial interface
  Serial.setTimeout(10);
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    int data = Serial.readString().toInt();

    if (data == 0x03)
    {
      write_byte_eeprom(0x7FFC, 0x00);
      write_byte_eeprom(0x7FFD, 0x30);

      Serial.print("EEPROM PROGRAMMING FINISHED AT: ");
      Serial.println(count - ((count - 0x3000) ? 1 : 0), HEX);
      Serial.end();
    }

    else
    {
      write_byte_eeprom(count, data);
      Serial.print("ADDRESS: ");
      Serial.print(count, HEX);
      Serial.print(" DATA: ");
      Serial.println(data, HEX);
      count ++;
    }
  }
}
