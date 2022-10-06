#include <GNSS.h>

GNSS gnss;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(" ");

  gnss.init(Serial1, 9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  gnss.serialRead();
  printf("%s,%s,%.7f,%.7f,%.2f,%.2f\n",
         gnss.UTCDate.c_str(),
         gnss.UTCTime.c_str(),
         gnss.lat, gnss.lon, gnss.velocity, gnss.course);
  delay(200);
}