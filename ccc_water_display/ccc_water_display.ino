#define ON LOW
#define OFF HIGH
#define ENABLE 1
#define DISABLE 0

#define zone1green 5
#define zone1blue  4
#define zone2green 7
#define zone2blue  6
#define zone3green 9 
#define zone3blue  8
#define zone4green 11
#define zone4blue  10
#define zone5green 13
#define zone5blue  12

#define startPump 38
#define PUMP 0
#define ALARM 3

#define pump1 52
#define pump2 50
#define pump3 48
#define pump4 46
#define pump5 44

unsigned int pumpPos = 1;
unsigned short blueWater  = 255;
unsigned short greenWater = 0; 


void setup() {  
  // Set up the fading LEDs
  pinMode(zone1green, OUTPUT);
  pinMode(zone1blue, OUTPUT);  
  pinMode(zone2green, OUTPUT);
  pinMode(zone2blue, OUTPUT);  
  pinMode(zone3green, OUTPUT);
  pinMode(zone3blue, OUTPUT);  
  pinMode(zone4green, OUTPUT);
  pinMode(zone4blue, OUTPUT);  
  pinMode(zone5green, OUTPUT);
  pinMode(zone5blue, OUTPUT);

  // Set up the pump LEDs 
  pinMode(ALARM, OUTPUT);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  pinMode(pump4, OUTPUT);
  pinMode(pump5, OUTPUT);

  // Set up the pump start button
  pinMode(startPump, INPUT);
  
  // Turn on the pump LEDs except one so the timer ISR will work
  digitalWrite(pump1, ON);
  digitalWrite(pump2, ON);
  digitalWrite(pump3, ON);
  digitalWrite(pump4, ON);
  digitalWrite(pump5, ON);
  analogWrite(ALARM, 0);

  // Set up the zones to be blue to start with 
  setZone(1, blueWater);
  setZone(2, blueWater);
  setZone(3, blueWater);
  setZone(4, blueWater);
  setZone(5, blueWater);
}


void loop() {
  int pause = 25; // delay between steps in the pumping process
  while (!digitalRead(startPump)) {} // Wait for pump start button
  
  // Button pressed to start pump so begin the reservoir-filling
  // this causes the aquifer to lose pressure and seawater backfills
  // from the ocean until it reaches the wells
  for (int i = 0; i <= 1055; i++) {
    if (i < 255) {
      analogWrite(zone1blue, 255-i);
      analogWrite(zone1green, i);
    }
    if (i > 200 && i <= 455) {
      analogWrite(zone2blue, 455 - i);
      analogWrite(zone2green, i - 200);
    }
    if (i > 400 && i <= 655) {
      analogWrite(zone3blue, 655 - i);
      analogWrite(zone3green, i - 400);
    }
    if (i > 600 && i <= 855) {
      analogWrite(zone4blue, 855 - i);
      analogWrite(zone4green, i - 600);
    }
    if (i > 800) {
      analogWrite(zone5blue, 1055 - i);
      analogWrite(zone5green, i - 800);
    }
    // Set the pause duration. Allow the operator to slow the pumping process
    // down in order to explain any part of the process in depth
    pause = (digitalRead(startPump))? 100 : 25;  // slow : normal
    if (i == 1000) alarm(ENABLE); // point at which seawater has backfilled to well
    if (i < 1000 && i % 4) runPump(); // pump only runs if seawater is not present
    delay(pause);
  }

  for (int i = 0; i <= 1055; i++) { 
    if (i > 800) {
      analogWrite(zone1blue, i - 800);
      analogWrite(zone1green, 1055 - i);
    }
    if (i > 600 && i <= 855) {
      analogWrite(zone2blue, i - 600);
      analogWrite(zone2green, 855 - i);
    }
    if (i > 400 && i <= 655) {
      analogWrite(zone3blue, i - 400);
      analogWrite(zone3green, 655 - i);
    }
    if (i > 200 && i <= 455) {
      analogWrite(zone4blue, i - 200);
      analogWrite(zone4green, 455 - i);
    }
    if (i < 255) {
      analogWrite(zone5blue, i);
      analogWrite(zone5green, 255-i);
    }
    
    // Set the pause. Allow the operator to slow the pumping process
    // down in order to explain part of the process in depth
    pause = (digitalRead(startPump))? 100 : 25;  // slow : normal
    if (i == 200) alarm(DISABLE);
    delay(pause);
  }
}
 
  
/**
 * This makes the pump LEDs cycle, indicating the pumps are working
 * It works on a timer to cycle the LEDs periodically
 */
void runPump() {
  switch (pumpPos) {
    case 1: // Pump pattern *__*_
      digitalWrite(pump1, ON);
      digitalWrite(pump2, OFF);
      digitalWrite(pump3, OFF);
      digitalWrite(pump4, ON);
      digitalWrite(pump5, OFF);
      break;
      
    case 2: // Pump pattern _*__*
      digitalWrite(pump1, OFF);
      digitalWrite(pump2, ON);
      digitalWrite(pump3, OFF);
      digitalWrite(pump4, OFF);
      digitalWrite(pump5, ON);
      break;
      
    case 3: // Pump pattern __*__
      digitalWrite(pump1, OFF);
      digitalWrite(pump2, OFF);
      digitalWrite(pump3, ON);
      digitalWrite(pump4, OFF);
      digitalWrite(pump5, OFF);
      pumpPos = 1;
      return;
  }
  pumpPos++;
  if (pumpPos > 3) pumpPos = 1; // Just in case it somehow overflows. Should NEVER happen
}


void alarm(unsigned char _alarm) {
  if (_alarm) {
    analogWrite(ALARM, 255);
    digitalWrite(pump1, OFF);
    digitalWrite(pump2, OFF);
    digitalWrite(pump3, OFF);
    digitalWrite(pump4, OFF);
    digitalWrite(pump5, OFF);
  }
  else {
    analogWrite(ALARM, 0);
    digitalWrite(pump1, ON);
    digitalWrite(pump2, ON);
    digitalWrite(pump3, ON);
    digitalWrite(pump4, ON);
    digitalWrite(pump5, ON);
  }
}


void setZone(int led, int colour)
{
  int green = 255 - colour;
  int blue = colour;
  
  //green = green / 3; // Green is too intense at full brightness
  //green = (green < 20)? 20 : blue; // Always have a little green for boosted brightness
  
  switch (led) {
    case 1: // Zone 1
      analogWrite(zone1green, green);
      analogWrite(zone1blue, blue);
      break;
 
    case 2: // Zone 2
      analogWrite(zone2green, green);
      analogWrite(zone2blue, blue);
      break;

    case 3: // Zone 3
      analogWrite(zone3green, green);
      analogWrite(zone3blue, blue);
      break;

    case 4: // Zone 4
      analogWrite(zone4green, green);
      analogWrite(zone4blue, blue);
      break;

    case 5: // Zone 5
      analogWrite(zone5green, green);
      analogWrite(zone5blue, blue);
      break;
  }
}

