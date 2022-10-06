#ifndef __GNSS_H_
#define __GNSS_H_

#include "Arduino.h"

class GNSS
{
public:
	GNSS();
	~GNSS();
	void init(HardwareSerial& serial, int baud);
	void serialRead();
	
	bool status; //true-valid;false-invalid
	String UTCTime;
	String UTCDate;
	double lat, lon;
	float velocity;  //knot
	float course;  //deg 0-360
	float altitude;  //m
	uint8_t SVs;
	float HDOP;
	
	void parseNAME(String);
	
private:
	const String header = "$GN";  
	const String footer = "\r\n";
	String payload;
	
	HardwareSerial* _serial;
	
    bool parse(String str, int &parseLen);
   
	bool checkCRC();
	void parseRMC();
	void parseGGA();
};

#endif
