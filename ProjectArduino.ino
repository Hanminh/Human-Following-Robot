// Ultrasonic sensor pins


#define S1Trig 2
#define S2Trig 4
#define S3Trig 8
#define S1Echo 3
#define S2Echo 5
#define S3Echo 7

// Button control pin
#define PIN_BTN 12
int countPush = 0;
int curState = 0;
int lastState = 0;
bool BocDauHayTest = true;

// Motor control pins
#define LEFT_MOTOR_PIN1 6
#define LEFT_MOTOR_PIN2 9
#define RIGHT_MOTOR_PIN1 10
#define RIGHT_MOTOR_PIN2 11


// Distance thresholds for obstacle detection
#define MAX_DISTANCE 40
#define MIN_DISTANCE_BACK 10


// Maximum and minimum motor speeds
#define MAX_SPEED 150
#define MIN_SPEED 0

int dis[3];

void setup() {


  // Set motor control pins as outputs
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);


  //Set the Trig pins as output pins
  pinMode(S1Trig, OUTPUT);
  pinMode(S2Trig, OUTPUT);
  pinMode(S3Trig, OUTPUT);


  //Set the Echo pins as input pins
  pinMode(S1Echo, INPUT);
  pinMode(S2Echo, INPUT);
  pinMode(S3Echo, INPUT);

  //Set the button as input pins
  pinMode(PIN_BTN, INPUT);

  // Initialize the serial communication for debugging
  Serial.begin(9600);
}

void selfTest() {
  moveBackward();
  delay(1500);
  moveForward();
  delay(1500);
  circular();
}


int findMinDistance() {
  int min = 999;
  int pos = 0;
  for (int i = 0; i <= 2; i++) {
    if (dis[i] <= min) {
      min = dis[i];
      pos = i;
    }
  }
  return pos;
}

void loop() {

  curState = digitalRead(PIN_BTN);
  if (curState != lastState) {
    if (curState == HIGH) {
      countPush++;
      BocDauHayTest = !BocDauHayTest;
      Serial.print(countPush);
    } else {
      // do something
    }
  }	
  lastState = curState;
  if (countPush % 2 == 0) {
    //Front Distance
    dis[0] = sensorOne();

    //Left Distance
    dis[1] = sensorTwo();

    //Right Distance
    dis[2] = sensorThree();

    // Find the sensor with the smallest distance
    int pos = findMinDistance();

    if (dis[0] < MIN_DISTANCE_BACK  || dis[1] < MIN_DISTANCE_BACK || dis[2] < MIN_DISTANCE_BACK) {
      stop();
    } else if (dis[pos] < MAX_DISTANCE) {
      if (pos == 0) {
        moveForward();
      } else if (pos == 1) {
        turnLeft();
      } else if (pos == 2) {
        turnRight();
      } else {
        stop();
      }
    } else {
      stop();
    }
  } else {
    if(BocDauHayTest) {
      BocDau();
      BocDau();
      BocDau();
    } else {
      selfTest();
    }
    countPush++;
  }
}


// Function to measure the distance using an ultrasonic sensor


int sensorOne() {
  //pulse output
  digitalWrite(S1Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(S1Trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(S1Trig, LOW);
  long t = pulseIn(S1Echo, HIGH);  //Get the pulse
  int cm = t / 29 / 2;             //Convert time to the distance
  return cm;                       // Return the values from the sensor
}


//Get the sensor values


int sensorTwo() {
  //pulse output
  digitalWrite(S2Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(S2Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S2Trig, LOW);
  long t = pulseIn(S2Echo, HIGH);  //Get the pulse
  int cm = t / 29 / 2;             //Convert time to the distance
  return cm;                       // Return the values from the sensor
}


//Get the sensor values


int sensorThree() {
  //pulse output
  digitalWrite(S3Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(S3Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S3Trig, LOW);
  long t = pulseIn(S3Echo, HIGH);  //Get the pulse
  int cm = t / 29 / 2;             //Convert time to the distance
  return cm;                       // Return the values from the sensor
}


// Motor control functions


void moveForward() {
  analogWrite(LEFT_MOTOR_PIN1, MAX_SPEED);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(RIGHT_MOTOR_PIN1, MAX_SPEED);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
}


void moveBackward() {
  analogWrite(LEFT_MOTOR_PIN1, 0);
  analogWrite(LEFT_MOTOR_PIN2, MAX_SPEED);
  analogWrite(RIGHT_MOTOR_PIN1, 0);
  analogWrite(RIGHT_MOTOR_PIN2, MAX_SPEED);
}


void turnRight() {
  analogWrite(LEFT_MOTOR_PIN1, 0);
  analogWrite(LEFT_MOTOR_PIN2, MAX_SPEED);
  analogWrite(RIGHT_MOTOR_PIN1, MAX_SPEED);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
}


void turnLeft() {
  analogWrite(LEFT_MOTOR_PIN1, MAX_SPEED);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(RIGHT_MOTOR_PIN1, 0);
  analogWrite(RIGHT_MOTOR_PIN2, MAX_SPEED);
}

void circular() {
  analogWrite(LEFT_MOTOR_PIN1, MAX_SPEED);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(RIGHT_MOTOR_PIN1, 0);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
  delay(3000);
  analogWrite(LEFT_MOTOR_PIN1, 0);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(RIGHT_MOTOR_PIN1, MAX_SPEED);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
  delay(3000);
  stop();
}

void stop() {
  analogWrite(LEFT_MOTOR_PIN1, 0);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(RIGHT_MOTOR_PIN1, 0);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
}

void BocDau() {
  moveBackward();
  delay(1000);
  analogWrite(LEFT_MOTOR_PIN1, 255);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(RIGHT_MOTOR_PIN1, 255);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
  delay(1000);
  stop();
}
