
import processing.serial.*;
Serial myPort;                       
int[] serialInArray = new int[1];   
int serialCount = 0;                
boolean firstContact = false;      
int analogIn;     
void setup() {
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  size(500, 500);
  background(0);
  smooth();
}

void draw() { 
  fill(120, 120, 120);
  ellipse(analogIn, 200, 10, 10);
}


void serialEvent(Serial myPort) {
  int inByte = myPort.read();
  if (firstContact == false) {
    if (inByte == 'A') { 
      myPort.clear();         
      firstContact = true;    
      myPort.write('A');    
    }
  } 
  else {
    serialInArray[serialCount] = inByte;
    serialCount++; 
    if (serialCount > 0 ) {
      analogIn = serialInArray[0];
      myPort.write('A');   
      serialCount = 0;
    }
  }
}
