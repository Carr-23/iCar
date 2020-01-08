//By: Mohammad-Ali, Braulio and Matthew
//Course code: TEJ 4M
//Date: Jan 15 2019
//Description: This program will use the LDR values to steer the car on the line

//the following 3 variables are used to determine if the LDR is on black or white
int leftTolerance = 125;
int middleTolerance = 128;
int rightTolerance = 118;
//the following 3 variables are used to hold the current LDR values
int leftLDR = 0;
int middleLDR = 0;
int rightLDR = 0;
//this is used to determine how far to go straight before starting a turn
int millisecondsToGoStraight = 125; 

//drive straight
void straight() {
  digitalWrite(6, HIGH); //left forward
  digitalWrite(9, LOW); //left reverse
  digitalWrite(2, LOW); //right reverse
  digitalWrite(3, HIGH); //right forward
}
//turn car left
void left() {
  digitalWrite(6, LOW); //left forward
  digitalWrite(9, HIGH); //left reverse
  digitalWrite(2, LOW); //right reverse
  digitalWrite(3, HIGH); //right forward
}
//turn car right
void right() {
  digitalWrite(6, HIGH); //left forward
  digitalWrite(9, LOW); //left reverse
  digitalWrite(2, HIGH); //right reverse
  digitalWrite(3, LOW); //right forward
}
//Update the LDR variables with current values
void updateLDR() {
  leftLDR = analogRead(A3);
  middleLDR = analogRead(A5);
  rightLDR = analogRead(A4);
}
//arduino built in function
void setup() {
  pinMode(6, OUTPUT);//left forward
  pinMode(9, OUTPUT);//left reverse
  pinMode(2, OUTPUT);//right reverse
  pinMode(3, OUTPUT);//right forward
  pinMode(A5, INPUT); //middle LDR
  pinMode(A4, INPUT); //right LDR
  pinMode(A3, INPUT); //left LDR
  //stopCar();
  Serial.begin(9600);
}
//clear the serial monitor by printing many new lines
void clearSerial() {
  Serial.println("we will restart in 5 sec");
  delay(5000);
  for (int x = 0; x < 100; x++) {
    Serial.println();
  }
}
//this function will calibrate the ldrs and verify their functionality and is used at the start of every day or when lighting conditions change
int calibrateCar() {
  Serial.println("We will now begin the calibration proccess");
  Serial.println("Please start by placing  ALL three ldrs on black");
  Serial.println("please type \"done\" when you are done");
  while (Serial.available() < 4);
  Serial.readString();
  updateLDR();
  leftTolerance = leftLDR + 20;
  middleTolerance = middleLDR + 20;
  rightTolerance = rightLDR + 20;
  Serial.print("Calibrated Values: ");
  Serial.print(leftTolerance);
  Serial.print(" ");
  Serial.print(middleTolerance);
  Serial.print(" ");
  Serial.println(rightTolerance);
  Serial.println("Now please place all 3 on white");
  Serial.println("please type \"done\" when you are done");
  while (Serial.available() < 4);
  Serial.readString();
  updateLDR();
  if (leftLDR < leftTolerance || middleLDR < middleTolerance || rightLDR < rightTolerance) {
    Serial.println("ERROR PROGRAM ABORTED 1 please try again");
    clearSerial();
    return 0;
  } else {
    Serial.println("Now please place left on black then the middle and right on white");
    Serial.println("please type \"done\" when you are done");
    while (Serial.available() < 4);
    Serial.readString();
    updateLDR();
    if (leftLDR > leftTolerance || middleLDR < middleTolerance || rightLDR < rightTolerance) {
      Serial.println("ERROR PROGRAM ABORTED 2 please try again");
      clearSerial();
      return 0;
    } else {
      Serial.println("Now please place left on WHITE then the middle and right on BLACK");
      Serial.println("please type \"done\" when you are done");
      while (Serial.available() < 4);
      Serial.readString();
      updateLDR();
      if (leftLDR < leftTolerance || middleLDR > middleTolerance || rightLDR > rightTolerance) {
        Serial.println("ERROR PROGRAM ABORTED 3 please try again");
        clearSerial();
        return 0;
      } else {
        Serial.println("Calibration complete the vehicle should now be functional");
        delay(1000);
        return 1;
      }
    }
  }
}

//this function is used to drive the car
void driveCar() {
  while (true) {
    // delay(100);
    updateLDR();
    if ((leftLDR < leftTolerance && middleLDR < middleTolerance && rightLDR > rightTolerance) || (leftLDR > leftTolerance && middleLDR < middleTolerance && rightLDR > rightTolerance)) {
      //go straight
      straight();
    } else if ((leftLDR > leftTolerance && middleLDR < middleTolerance && rightLDR < rightTolerance) || (leftLDR < leftTolerance && middleLDR < middleTolerance && rightLDR < rightTolerance)) {
      //right turn
      straight();
      delay(millisecondsToGoStraight);
      updateLDR();
      //loop untill right LDR sees black
      while (rightLDR > rightTolerance) {
        updateLDR();
        right();
      }
      //loop untill middle LDR sees black
      while (middleLDR < middleTolerance) {
        updateLDR();
        right();
      }
    } else if (leftLDR < leftTolerance && middleLDR > middleTolerance && rightLDR > rightTolerance) {
      //left adjustment
      //loop untill middle LDR sees black
      while ( middleLDR > middleTolerance ) {
        updateLDR();
        left();
      }
    }  else if (leftLDR > leftTolerance && middleLDR > middleTolerance &&  rightLDR < rightTolerance) {
      //right adjustment
      //loop untill middle LDR sees black
      while (middleLDR > middleTolerance ) {
        updateLDR();
        right();
      }
    }  else {
      //all LDRs reading white
      left();
    }
  }
}
//this is an arduino default function
void loop() {
  //CALIBRATION
  //  if (calibrateCar() == 1) {
  driveCar();
  //  }

}
