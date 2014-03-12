
import processing.serial.*;
Serial myPort;                    
int[] serialInArray = new int[16];  
int serialCount = 0;              
boolean firstContact = false;  
int[] average=new int[8];
int[] ledState=new int[8];


void setup() {
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  size(500, 500); 
  background(0);
  smooth();
}

void draw() { 

  fill(0);
  for(int i=0;i<8;i++){
  ellipse(average[i], 150, 20, 20);
  ellipse(ledState[i], 300, 20, 20);  
  println("dial: " + average[i] + "\t" + "led: " + ledState[i] + "\t");
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
    if (serialCount > 2 ) {
      for(int i=0;i<8;i++){
      average[i] = serialInArray[i];
      }
      for(int i=8;i<16;i++){
      ledState[i-8] = serialInArray[i];
      }    
      myPort.write('A');
      serialCount = 0;
    }
  }
}

