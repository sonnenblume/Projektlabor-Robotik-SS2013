int lft = 8;
int rght = 9;
int gas = 10;
long time = 0;
int push = 2;

int hits;

int cond = 1;
const int left = 1;
const int right = 2;
const int strike = 3;

char key = 0;

void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT); //Laser
  pinMode(3, OUTPUT); //Laser
  pinMode(5, INPUT_PULLUP); //Taster
  pinMode(6, INPUT_PULLUP); //Taster
  pinMode(2, OUTPUT); //Magnet-Pusher
  
  setupMotor(); 
}



void setupMotor() {
  pinMode(lft, OUTPUT);
  pinMode(rght, OUTPUT);
  pinMode(gas, OUTPUT);
}

void goLeft() {
  digitalWrite(lft, HIGH);
  digitalWrite(rght, LOW);
  digitalWrite(gas, HIGH);
}

void goRight() {
  digitalWrite(lft, LOW);
  digitalWrite(rght, HIGH);
  digitalWrite(gas, HIGH);
}

void pause() {
  digitalWrite(lft, LOW);
  digitalWrite(rght, LOW);
  digitalWrite(gas, LOW);
}

void pow() {
  digitalWrite(push, HIGH);
  delay(200);
  digitalWrite(push, LOW);
  Serial.println("Pow");
}


void loop() {
  int input1 = msrDiff(2, A0, 4);
  int input2 = msrDiff(2, A1, 3);
  
  pongKI(input1, input2);
  
  //humanPlayer();
}


int msrDiff(int delaytime, int msrPin, int ledPin) {
  long woLaser = 0;
  long laser = 0;
  for (int i = 0; i <= 5; i++) {
    digitalWrite(ledPin, LOW);
    delay(delaytime);
    woLaser += analogRead(msrPin);
    digitalWrite(ledPin, HIGH);
    delay(delaytime);
    laser += analogRead(msrPin);
  }
  return(laser - woLaser);
}
 
/*
- Funktionierende KI -
TO-DO:
  - !Wichtig! Magnetpusher mit Schutz vor Dauerspannung ausstatten.
    z.B. Push-Dauer oder die Anzahl der Aufrufe pro Zeitintervall messen 
    und entsprechend begrenzen.
  - Schlitten soll in die Mitte des Spielfelds zurückkehren, nachdem der Ball zurück
    geschossen wurde.
  - Schlittenbewegung stoppen wenn das linke/rechte ende erreicht wird. 
    Momentan knallt der Schlitten ungebremst in die Blechaufhängung
  - Erkennen das der Ball ins Tor gegangen ist.
  - System ist nach wie vor empfindlich gegen starkes Sonnenlich/Tageslicht.
  - Zeitpunkt des "zurück-schießens" kalibrieren. 
    Man kann hier sicherlich erreichen das der Impuls auf den Ball größer wird!
  - Die Konstruktion des "Schlägers" leidet doch sehr unter dem Pusher. 
    Vielleicht kann man etwas aus Metall basteln? 
    Mit abgeflachten Seiten könnte ein "anschnibbeln" des Balles erreicht werden?!
  - Diverse Verbesserungen der Gesamtkonstruktion sind denkbar. 
    Insbesondere solle der KingPong an das Spielfeld angebracht werden. 
    Dabei ist darauf zu achten das der Laser in der richtigen Höhe und möglichst 
    waagerecht über das Spielfeld strahlt.
*/
  
void pongKI(int input1, int input2) {
  if (input1 >= 25) {
    if (input2 >= 25) {
      pow();
      hits += 1;
    }
    else {
      goLeft();
      Serial.println("left");
    }
  }
  else {
    if (input2 >= 25) {
      goRight();
    }
    else {
      pause();
    }
  }
}
 
  
/*
Zusammen mit dem Processing Programm "HumanPong" 
lässt sich so der Pong-Schläger steuern.
*/
void humanPlayer() {
  if (Serial.available()) {
     key = Serial.read();
     if (key == '0') {
       pause();
     }
     if (key == byte(37)) { // left
       goLeft();
     }
     if (key == byte(39)) { // right
       goRight();
     }
     else if (key == byte(32)) {
       pow();
     }
  }
}
  
  
/*
Dieser Code ist der letzte Stand der KingPong Gruppe.
Funktioniert leider nicht, obwohl eine Umsetzung mit switch-block ganz schön ist.
*/
void brokeKI(int input1, int input2) {
  switch(cond) {
    case left: {
      goLeft();
      int leftButton = digitalRead(5);
      if (input2 >= 25 || leftButton == 0) {
        Serial.print("now right");
        Serial.print("\t");
        cond = right;
      }
      if (input1 >= 25 && input2 >= 25) {
        cond = strike;
      }
      break;
    }
    
    case right: {
      goRight();
      int rightButton = digitalRead(6);
      if (input1 >= 25 || rightButton == 0) {
        Serial.print("now left");
        Serial.print("\t");
        cond = left;
      }
      if (input1 >= 25 && input2 >= 25) {
        cond = strike;
      }
      break;
    }
    
    case strike: {
      Serial.print("Pow");
      digitalWrite(push, HIGH);
      delay(200);
      digitalWrite(push, LOW);
      cond = left;
    }
  }
}
