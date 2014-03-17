
import processing.serial.*;
Serial myPort;                       
int[] serialInArray = new int[8];   
int serialCount = 0;                
boolean firstContact = false; 
//below parameter is not necessary, can be deleted
int[] analogIn=new int[8];     
void setup() {
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 57600);
  size(500, 900);  
  smooth();
}

void draw() { 
  background(0);
  fill(255, 255, 120);
  for(int i=0;i<8;i++){
  ellipse(analogIn[i], 50*i+100, 10, 10);
  }
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
    if (serialCount > 7 ) {
     analogIn[0] = serialInArray[0];
     analogIn[1] = serialInArray[1];
     analogIn[2] = serialInArray[2];
     analogIn[3] = serialInArray[3];
     analogIn[4] = serialInArray[4];
     analogIn[5] = serialInArray[5];
     analogIn[6] = serialInArray[6];
     analogIn[7] = serialInArray[7];
      myPort.write('A');   
      serialCount = 0;
    }
  }
}
