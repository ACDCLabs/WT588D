// WT588D .bin file readout.  Runs in conjunction with
// FlashReader.ino sketch on Arduino 


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
  String portname;
  Serial port;
  background(0);
  size(300, 150);                                 // Serial freaks out without a window :/  

  portname = "/dev/tty.usbmodem1421";                        // bypass scan
  port = new Serial(this, portname, 57600);

    print("Trying port " + portname + "... ");
    while(true){
    if((s = readLine()) != null) {
      println(s);
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