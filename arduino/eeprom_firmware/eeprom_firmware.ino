#define WRITE_DATA 2
#define REG_LATCH 3
#define SHIFT_CLK 4
#define OUT_ENABLE 5
#define WRITE_ENABLE 14

#define DATA_0 6
#define DATA_7 13

unsigned int addr_count = 0x0000;
int mode = 0x03;

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

    if (data == 0x23 || data == 0x13 || data == 0x03)
    {
      if (mode == 0x23)
      {
        write_byte_eeprom(0xFFFC, 0x00);
        write_byte_eeprom(0xFFFD, 0x00);
      }
      
      mode = data;
      addr_count = 0x000;
      Serial.write(0x03);
      return;
    }

    if (mode == 0x23)
    {
      write_byte_eeprom(addr_count, data);
      addr_count ++;
      Serial.write(0x02);
    }

    else if (mode == 0x13 && data == 0x33)
    {
      Serial.write(read_byte_eeprom(addr_count));
      addr_count ++;
    }

    else if (mode == 0x03)
    {
      Serial.write(0x01);
    }

    else 
    {
      Serial.write(0x0);
    }
  }
}
