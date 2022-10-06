#include "GNSS.h" 

//#define GNSSDEBUG

unsigned char hexToDec(unsigned char hex)
{
	if (hex >= 'A')
		return (hex - 'A') + 10;
	else
		return hex - '0';
}

GNSS::GNSS()
{
	status = false;
}

GNSS::~GNSS()
{
	_serial->end();
}

void GNSS::init(HardwareSerial& serial, int baud)
{
	_serial = &serial;
	_serial->begin(baud);
}

void GNSS::serialRead()
{
	int len = _serial->available();
	if(len>0)
	{
		String recvStr = _serial->readString();
		parseNAME(recvStr);
	}
}

void GNSS::parseNAME(String parseStr)
{
	int parseLength = 1;
	int len = static_cast<int>(parseStr.length());
	
	while (len > parseLength)
	{
		String str = parseStr.substring(parseLength - 1);
#ifdef GNSSDEBUG
		Serial.println(str);
#endif
		if (parse(str, parseLength))
		{
			if (checkCRC())
			{
				int nPos = payload.indexOf(',');
				if(nPos != -1)
				{
					String nmeaHeader = payload.substring(0,nPos);
#ifdef GNSSDEBUG
					Serial.print("Get");
					Serial.println(nmeaHeader);
#endif
					if(nmeaHeader == "$GNRMC")
						parseRMC();
					else if(nmeaHeader == "$GNGGA")
						parseGGA();
				}
			}
		}
	}
}

bool GNSS::parse(String str, int &parseLen)
{
#ifdef GNSSDEBUG
	Serial.println("parsing...");
#endif
	static bool get_header = false;
	if (get_header)
	{
		int pos_footer = str.indexOf(footer);
		if (pos_footer != -1)
		{
#ifdef GNSSDEBUG
			Serial.println("gotFooter");
#endif
			payload += str.substring(0, pos_footer);
			get_header = false;
			parseLen += pos_footer + 2;
			return true;
		}
		else
		{
			payload += str;
			if (payload.length() >= 100)
			{
				payload = "";
				get_header = false;
			}
			parseLen += str.length();
			return false;
		}
	}
	else
	{
		int pos_header = str.indexOf(header);
		if (pos_header != -1)
		{
#ifdef GNSSDEBUG
			Serial.println("gotHeader");
#endif
			payload = "";
			get_header = true;
			int pos_footer = str.indexOf(footer, pos_header);
			if (pos_footer != -1)
			{
#ifdef GNSSDEBUG
				Serial.println("gotFooter");
#endif
				payload = str.substring(pos_header, pos_footer);
				get_header = false;
				parseLen += pos_footer-pos_header + 2;
				return true;
			}
			else
			{
				payload += str.substring(pos_header);
				parseLen += str.length();
				return false;
			}
		}
		else
		{
			parseLen += str.length();
			return false;
		}
	}
}

bool GNSS::checkCRC()
{
	int pos = payload.indexOf('*');
	
	if(pos != -1)
	{
		unsigned char checkSum = 0;
		unsigned char CRC = (hexToDec(payload[pos + 1]) << 4) + hexToDec(payload[pos + 2]);
		for (int i = 1; i < pos; i++)
		{
			checkSum ^= payload[i];
		}
		if (checkSum == CRC)
			return true;
		else
			return false;
	}
	else
		return false;
}

void GNSS::parseRMC()
{
	/**UTC**/
	int headPos = payload.indexOf(',');
	int footPos =  payload.indexOf(',',headPos+1);
	if(footPos == -1) return;
	UTCTime = payload.substring(headPos+1,footPos);
	
	/**Status**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	String subStr = payload.substring(headPos+1,footPos);
	if(subStr == "A") status == true;
	
	/**Lat**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	subStr = payload.substring(headPos+1,footPos);
	int pos = subStr.indexOf('.');
	if(pos == -1) return;
	lat = subStr.substring(0,pos - 2).toDouble() + subStr.substring(pos - 2).toDouble()/60;
	
	/**Northing Indicator**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	
	/**Lon**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	subStr = payload.substring(headPos+1,footPos);
	pos = subStr.indexOf('.');
	if(pos == -1) return;
	lon = subStr.substring(0,pos - 2).toDouble() + subStr.substring(pos - 2).toDouble()/60;
	
	/**Easting Indicator**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	
	/**SOG**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	velocity = payload.substring(headPos+1,footPos).toFloat();
	
	/**COG**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	course = payload.substring(headPos+1,footPos).toFloat();
	
	/**Date**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	UTCDate = payload.substring(headPos+1,footPos);
}

void GNSS::parseGGA()
{
	/**UTC**/
	int headPos = payload.indexOf(',');
	int footPos =  payload.indexOf(',',headPos+1);
	if(footPos == -1) return;
	UTCTime = payload.substring(headPos+1,footPos);
	
	/**Lat**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	String subStr = payload.substring(headPos+1,footPos);
	int pos = subStr.indexOf('.');
	if(pos == -1) return;
	lat = subStr.substring(0,pos - 2).toDouble() + subStr.substring(pos - 2).toDouble()/60;
	
	/**Northing Indicator**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	
	/**Lon**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	subStr = payload.substring(headPos+1,footPos);
	pos = subStr.indexOf('.');
	if(pos == -1) return;
	lon = subStr.substring(0,pos - 2).toDouble() + subStr.substring(pos - 2).toDouble()/60;
	
	/**Easting Indicator**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	
	/**Status**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	
	/**SVs Used**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	SVs = static_cast<uint8_t>(payload.substring(headPos+1,footPos).toInt());
	
	/**HDOP**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	HDOP = payload.substring(headPos+1,footPos).toFloat();
	
	/**Alt**/
	headPos = footPos;
	footPos =  payload.indexOf(',',headPos+1);
	altitude = payload.substring(headPos+1,footPos).toFloat();
}