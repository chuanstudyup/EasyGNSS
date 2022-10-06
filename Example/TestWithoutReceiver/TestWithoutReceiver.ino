#include <GNSS.h>

GNSS gnss;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(" ");

  String str0 = "$GNRMC,083712.40,A,3030.83159,N,11424.56558,E,";
  String str1 = "0.150,,291221,,,A*65\r\n$GNRMC,083712.80,A,3030.83158,N,11424.56558,E,0.033,,291221,,,A*6C\r\n";

  gnss.parseNAME(str0);
  gnss.parseNAME(str1);
  Serial.printf("%s,%s,%.7f,%.7f,%.3f",gnss.UTCTime.c_str(),gnss.UTCDate.c_str(),gnss.lat,gnss.lon,gnss.velocity);
}

void loop() {
  // put your main code here, to run repeatedly:
}