import processing.serial.*;
Serial port;

/*
Steuerung erfolgt über die Pfeiltasten links/rechts
und Push über die Leertaste.
Evtl. Probleme bei anderen Systeme wegen ASCII codierung?
*/

void setup() {
  size(100, 100);
  println("Available serial ports:");
  println(Serial.list());
  // Hier muss der serial port ausgewählt werden an dem der arduino hängt.
  // über Serial.list()[0] <- über eine der Nummern aus der Liste.
  port = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  background(0, 0, 0);
}

void keyPressed() {
  port.write(keyCode);
}
void keyReleased() {
  port.write('0');
}
