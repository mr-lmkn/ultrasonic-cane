  uint8_t pinVib = 2;          // вибромодуль
  int sonarPingPin = 5;
  int sonarEchoPin = 6;
  int del = 100;               // задержка между измерениями в мкс //delay(500)-0,5 секунд
  int mesureCnt = 10;          // кол-во изменрений для вычисления среднего значения
  int avgMesureDel = 0;        // задержка между измерениями в мкс для среднего
  int activateDistance = 100;  // расстояние в см при котором сработает мотор
//-----------------------------
  int debugMode = 1;
  int beeperPort = 4;          // динамик

void setup(){ 
 pinMode(pinVib, OUTPUT);   // Переводим вывод pinVib в режим выхода
 digitalWrite(pinVib, LOW); // Устанавливаем уровень логического «0» на выводе pinVib
  if (debugMode==1) {
    pinMode(beeperPort, OUTPUT);
    Serial.begin(9600);
    //delay(3000);
    tone(4,200); delay(130); tone(4,250); delay(130); tone(4,300); delay(130); noTone(4); delay(150);
    tone(4,400); delay(200); noTone(4);
    tone(4,400); delay(150); noTone(4); 
    Serial.println("Start");
  }
}

void loop(){ 
  float frequency;
  long prntOfRange;
  int avgDistande;
  int prcntOfFrequency;
  
   avgDistande = getDistance(sonarPingPin, sonarEchoPin);
  //avgDistande = getAvgDistande (sonarPingPin, sonarEchoPin, avgMesureDel);

  if (avgDistande < activateDistance && avgDistande > 0) {
    prntOfRange = round(avgDistande * 100 / activateDistance);    
    vibrateDistance(prntOfRange);
    if (debugMode==1) {
      frequency = (1 + log10(prntOfRange) * 200) ;
      tone(4,frequency );
    }
  } 
  else {
    if (debugMode==1) {
      prntOfRange = 0;
      frequency = 0;
      noTone(4);
    }
    
  }
  delay(del);
  if (debugMode==1) {
    Serial.println(", mesureCnt:" + String(mesureCnt) + ", avgDistande: " + String(avgDistande) + " prntOfRange: " + prntOfRange + ", calced frequency:" + String(frequency));
  }
  
}

int getAvgDistande (int pingPin, int echoPin, int avgMesureDel) {
  long colectedSensorData = 0;
  int avgDistande = 0;
  int distance = 0;
  for (int i = 0; i < mesureCnt; i++) {
    distance = getDistance (pingPin, echoPin);
    colectedSensorData = colectedSensorData + distance;
    if (debugMode==1) {
      Serial.println(" distance " + String(distance));
    }
    delay(avgMesureDel); 
  }
    // AVG distance
  avgDistande = round(colectedSensorData / mesureCnt);
  if (debugMode==1) {
    Serial.println(" colectedSensorData " + String(colectedSensorData) + ", avgDistande " + String(avgDistande));
  }
  return avgDistande;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return round(microseconds / 29 / 2);
}

long getDistance(int pingPin, int echoPin) {
  // establish variables for duration of the ping, and the distance result
  // in centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  // Serial.println("distance " + String(cm));
  return cm;
}

void vibrate (int count, int del, int pause) {
  for (int i = 0; i < count; i++) {
    digitalWrite(pinVib, HIGH);
    delay(del);
    digitalWrite(pinVib, LOW);
    delay(pause);
  }
}

void vibrateDistance (int distance) {
  if (distance > 80) {
    if (debugMode==1) {
      Serial.println("Bzzz step 01 distance: " + String(distance));
    }
    vibrate(3,15, 25);
  } else if(distance >= 60) {
    if (debugMode==1) {
      Serial.println("Bzzz step 02 distance: " + String(distance));
    }
    vibrate(3,20, 25);
  } else if(distance >= 40) {
    if (debugMode==1) {
      Serial.println("Bzzz step 04 distance: " + String(distance));
    }
    vibrate(3,25, 20);
  } else if(distance >= 20) {
    if (debugMode==1) {
      Serial.println("Bzzz step 05 distance: " + String(distance));
    }
    vibrate(3,30, 15);
  } else if(distance > 10) {
    if (debugMode==1) {
      Serial.println("Bzzz step 06 distance: " + String(distance));
    }
    vibrate(3,35, 10);
  } else if(distance <= 10) {
    if (debugMode==1) {
      Serial.println("Bzzz step 07 distance: " + String(distance));
    }
    vibrate(4,45, 5);
  } else if(distance <= 5) {
    if (debugMode==1) {
      Serial.println("Bzzz step 07 distance: " + String(distance));
    }
    vibrate(4,50, 0);
  }
}
