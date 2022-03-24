#define PIN_SIGNAL 14
#define INT_MS 250

void setup() 
{
  pinMode(PIN_SIGNAL, OUTPUT);
}

void loop() 
{
  digitalWrite(PIN_SIGNAL, HIGH);
  delay(INT_MS);
  digitalWrite(PIN_SIGNAL, LOW);
  delay(INT_MS);
}
