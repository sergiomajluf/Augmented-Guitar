import processing.serial.*;

Serial myPort;        // The serial port
float xPos = 1;         // horizontal position of the graph
int e=0;

void setup () {
  // set the window size:
  size(1200, 300); 
  colorMode(HSB); 

  //println(Serial.list());
  myPort = new Serial(this, Serial.list()[4], 115200);
  // don't generate a serialEvent() unless you get a newline character:
  myPort.bufferUntil('\n');
  background(255);
}
void draw () {
}

void serialEvent (Serial myPort) {

  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an int and map to the screen height:
    float inByte = float(inString); 
    
    //inByte = map(inByte, 0, 1023, 20, height); // FSR
    inByte = map(inByte, 0, 127, 20, height);    // Flex sensor
    
    float c = map(inByte, 0, 1023, 0, 360);

    // draw the line:
    //stroke(c, 170, 255);
    stroke(e, 170, 255);
    strokeWeight(2);
    point(xPos, height+10 - inByte);

    //stroke(c, 255, 255, 40);
    //line(xPos, height-10, xPos, height+10 - inByte);

    drawChrome();

    fill(0);
    noStroke();
    text("//01 : FORCE TEST", 20, 40);
    fill(255);
    rect(145, 20, 80, 30);
    fill(c, 255, 255);
    text(nf(inByte, 3, 2), 150, 40);



    // at the edge of the screen, go back to the beginning:
    if (xPos >= width-9) {
      //save("strengthHoldingPick-"+minute()+millis()+".png");
      xPos = 10;
      
      if (e < 200) {
         e += 50;
      } else {
        background(255, 30);
        e =0;
      } 
    } 
    else {
      // increment the horizontal position:
      xPos += 0.25;
    }
  }
}


void drawChrome() {
  noFill();
  stroke(70, 50);
  rectMode(CORNER);
  strokeWeight(1);
  rect(10, 10, width-20, height-20);
  line(10, 190, width-10, 190);
}

