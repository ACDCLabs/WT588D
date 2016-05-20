// WT588D .bin file transfer.  Runs in conjunction with
// AudioLoader_WT588D.ino sketch on Arduino 
// Processing sketch based off AdaFruit AudioXfer sketch
// Use at your own risk

import processing.serial.*;
Serial  port = null;

int     capacity;
boolean done = false;

// Wait for line from serial port, with timeout
String readLine() {
  String s;
  int    start = millis();
  do {
    s = port.readStringUntil('\n');
  } while((s == null) && ((millis() - start) < 3000));
  return s;
}

void setup() {
  String s;
  background(0);
  size(300, 150);                                 // Serial freaks out without a window :/

  // Locate Arduino running AudioLoader sketch.
  // Try each serial port, checking for ACK after opening.
  
  println("Scanning serial ports...");
   for(String portname : Serial.list()) {
    try {
      portname = "/dev/tty.usbmodem1421";                        // bypass scan
      port = new Serial(this, portname, 57600);
    } catch (Exception e) {         // Port in use, etc.
      print("Error" +e);
      continue;
    }
    print("Trying port " + portname + "... ");
    if(((s = readLine()) != null) && s.contains("HELLO")) {
      println("OK");
      break;
    } else {
      println();
      print("Received: " + s);
      port.stop();
      port = null;
    }
  }

  if(port != null) {                              // Find one?
    if(((s        = readLine())                != null)
      && ((capacity = Integer.parseInt(s.trim())) > 0)) {
      println("Found Arduino w/" + capacity + " byte flash chip."); 
      selectInput("Select a file to process:", "fileSelected");
    } else {
      println("Arduino failed to initialize flash memory.");
      done = true;
    }
  } else {
    println("Could not find connected Arduino running WT588DAudioLoader sketch.");
    done = true;
  } 
}

void fileSelected(File f) {
  if(f == null) {
    println("Cancel selected or window closed.");
  } else {
    println("Selected file: " + f.getAbsolutePath());
    byte input[] = loadBytes(f.getAbsolutePath());

    int filesize = input.length;
    
    
      port.write("ERASE");                          // Issue erase command now, process audio while it works

      println("Processing file...\n");
                 
      print("Erasing flash...");                    // Really just waiting for the erase ACK
      
      String s, cap;
      int c;
      
      cap = "CAPACITY" + filesize;
      port.write(cap);
                  
      while(((s = readLine()) == null) || (s.contains("READY") == false));
      
      print("\nFilesize: ");
      println(cap);
      print("OK\nWriting...\n");
      // Instead of just write(output), it's done per-byte so we
      // can echo progress dots from the Arduino to the console.
      // Eventually may want to make it re-write error sectors.
     
      for(int i=0; i<filesize; i++) {
        //a = input[i]; //& 0xff;
        port.write(input[i]);
              
        if((c = port.read()) >= 0) {print((char)c);}
      }
      println("done!");
     
    } 
 
   done = true;
   
}

void draw() {
  if(done) exit();
}