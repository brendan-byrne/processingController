//----------------------------------------------------------------------------------------------------------------

// MULTIPLEXER SPECIFIC VARIABLES
const int bitPin[] = {2, 3, 4};             //The multiplexer's three binary control inputs
const int numChannels = 8;                  //The number of channels available on each multiplexer

// BUTTON INPUTS + PREVIOUS STATE AND DEBOUNCING VARIABLES
const int digitalInput = 5;                 //This pin connects to the in/out of multiplexer (on right)
boolean lastState[numChannels];             //Array that saves last state of button press in order to trigger debounce timer         
boolean currentState[numChannels];          //Current state of button press. used to check against previous states in conditionals
boolean formerLastState[numChannels];       //Temp variable to pass between debounce conditional and action conditional
long lastDebounce[numChannels];             //Debounce timer for each button press
int debounceDelay = 50;                     //Debounce Time before actual button reading is used in program

// ANALOG INPUTS + SMOOTHING VARIABLES
const int analogInput = A0;                 //This pin connects to the in/out of the other multiplexer (on left)
const int numReadings = 15;                 //The number of readings taken that are later averaged together
int readings[numChannels][numReadings];     //Two dimensional array for storing the readings to be averager per each analog channel
int index = 0;                              //Index to keep track of number of readings taken
int total[numChannels];                     //Running total to be averaged
byte average[numChannels];                   //Average value that will be used to do things with!
byte lastAverage[numChannels];               //Only send serial if reading is different fromt this value

// LED VARAIBLES + SHIFT REGISTER VARIABLES
int dataPin = 6;                            //Pin sending data to shift reg
int clockPin = 7;                           //Pin shifting data in to registers
int latchPin = 8;                           //Pin setting latch to set all registers to visible
boolean registers[16];                      //Variable used for setting register to HIGH or LOW
boolean ledState[16];                       //LED states to toggle whether LED is on or off

// SERIAL COMMUNICATION VARIABLES
int inByte = 0;
char id[] = {"abcdefgh"};
byte byteSend[numChannels];

//----------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);                                          //Initiate serial communication for debugging/connecting with Processing
  for (int x=0; x<3; x++) pinMode(bitPin[x], OUTPUT);           //Initiate binary controllers as outputs
  pinMode(dataPin, OUTPUT);                                     //Set dataPin to output
  pinMode(clockPin, OUTPUT);                                    //Set clockPin to output
  pinMode(latchPin, OUTPUT);                                    //Set latchPin to output
  pinMode(digitalInput, INPUT);                                 //Initiate output from first multiplexer as digital input
  pinMode(analogInput, INPUT);                                  //Initiate output from second multiplexer as analog input
  for (int x=0; x<numChannels; x++) {                           
    for (int y=0; y<numReadings; y++) readings[x][y] = 0;       //Set all readings to 0. code works fine without it. good practice?
  }
   establishContact();                                          //Open serial connection
}

//----------------------------------------------------------------------------------------------------------------

void loop() {
  for (int x=0; x<numChannels; x++) {
    // DIGITAL READING + DEBOUNCING
    // ----------------------------
    currentState[x] = readMux(digitalInput, x);                 //Read buttons 1-8 using multiplexer function
    if (currentState[x] != lastState[x]) {                      //Is that input different from it's last state?
      lastDebounce[x] = millis();                               //If yes, start the debounce timer in order to bypass fake readings. it waits for a steady reading. resets every bounce
      formerLastState[x] = lastState[x];                        //Save the last state in temporary variable, because lastState will be set to currentState in the next line. we still need this information for the next if statement block so the code knows what to change
      lastState[x] = currentState[x];                           //Set last button state to current button state. this is so the if statement (currentState != lastState) doesn't keep checking as true and resetting the debounce timer
    }
    if ((millis() - lastDebounce[x]) > debounceDelay) {         //Has the debounce timer run for 50ms?
      if (currentState[x] == 1 && formerLastState[x] == 0) {    //Is the button HIGH?
        ledState[x] = !ledState[x];                             //Yes? do this thingy --- PUT SERIAL CODE HERE TO SEND TO PROCESSING
      }
      formerLastState[x] = currentState[x];                     //Set formerLastState to currentState so that the if statement doesn't keep running as true
    }
    registers[x] = ledState[x];                                 //Set LED State to register position (turn LED on/off)

    // ANALOG READING + SMOOTHING
    // --------------------------
    total[x] = total[x] - readings[x][index];                   //Subtract last reading at this index. Otherwise the total would just keep getting bigger!           
    readings[x][index] = readMux(analogInput, x);               //Read analog input 1-8 and assign to specific index
    total[x] += readings[x][index];                             //Add that index to the total
    average[x] = (total[x] / numReadings);                      //Divide total by number of readings taken to obtain the average
    
    if (average[x] > lastAverage[x] + 7 || average[x] < lastAverage[x] - 7) {
      Serial.write(id[x]);
      Serial.write(average[x]);
      delay(10);
      lastAverage[x] = average[x];   
    }  

    if (average[x] > 512) ledState[x+8] = HIGH;                 //Is pot reading higher than 512? turn LED on
    else ledState[x+8] = LOW;                                   //Is pot reading less than 512? turn LED off
    registers[x+8] = ledState[x+8];

    //Set LED state to register position (this is second shift register so it's +8)
  }
  index += 1;                                                   //Advance index to next position
  if (index >= numReadings) index = 0;                          //Reset index once all readings have been taken 
  writeRegisters();
  delay(10);                                                    // delay in between reads for stability 
}

//----------------------------------------------------------------------------------------------------------------

//Multiplexer Read Function
int readMux(int readPin, int channel) {
  for(int x=0; x<3; x++) digitalWrite(bitPin[x], bitRead(channel, x));  //bitRead: my new favorite function
  if (readPin < 11) return digitalRead(readPin);                        //all pins on teensy below 11 are digital. this is a "rough" part of the code. could make it more specific or make separate functions for digi/analog readings.
  if (readPin > 10) return analogRead(readPin);                         //all pins on teensy above 10 are analog.
}

//Shift Register Function
void writeRegisters() {
  digitalWrite(latchPin, LOW);                                          //Set latch pin LOW in prep for data
  for(int i=15; i>=0; i--) {                                            //There are 16 positions. --because of the way i wired. could have been ++
    digitalWrite(clockPin, LOW);                                        //Set clock pin LOW in prep for data
    digitalWrite(dataPin, registers[i]);                                //Input data to shift register
    digitalWrite(clockPin, HIGH);                                       //lock that data in first register and push previous one over
  }
  digitalWrite(latchPin, HIGH);                                         //display all data input
} 
//MaxMaxMax
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write('A');  
    delay(300);
  }
}

