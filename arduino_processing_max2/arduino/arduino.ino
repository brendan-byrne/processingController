int analogIn = 0;
int inByte = 0; 
void setup()
{
  Serial.begin(9600);
  establishContact(); 
}

void loop()
{
  if (Serial.available() > 0) {
    inByte = Serial.read();
    analogIn = analogRead(A10);
    Serial.write(analogIn/4);              
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write('A');  
    delay(300);
  }
}
