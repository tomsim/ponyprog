//=========================================================================//
//                                                                         //
//  PonyProg - Serial Device Programmer                                    //
//                                                                         //
//  Copyright (C) 1997-2017   Claudio Lanconelli                           //
//                                                                         //
//  http://ponyprog.sourceforge.net                                        //
//                                                                         //
//-------------------------------------------------------------------------//
//                                                                         //
// This program is free software; you can redistribute it and/or           //
// modify it under the terms of the GNU  General Public License            //
// as published by the Free Software Foundation; either version2 of        //
// the License, or (at your option) any later version.                     //
//                                                                         //
// This program is distributed in the hope that it will be useful,         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of          //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       //
// General Public License for more details.                                //
//                                                                         //
// You should have received a copy of the GNU  General Public License      //
// along with this program (see COPYING);     if not, write to the         //
// Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. //
//                                                                         //
//-------------------------------------------------------------------------//
//=========================================================================//

#include <QDebug>
#include <QString>
#include <QSettings>

#include "e2profil.h"
#include "types.h"

#include "errcode.h"

#include "globals.h"



QString E2Profile::filename = "e2p.ini";

QSettings* E2Profile::s = new QSettings("e2p.ini", QSettings::IniFormat);



//=====>>> Costruttore <<<======
#if 0
E2Profile::E2Profile(const QString &nm)  :
	s(0)
{
	if (nm.length())
	{
		filename = nm;
	}
	else
	{
		filename = "e2p.ini";
	}

	s = new QSettings(filename, QSettings::IniFormat);
}


E2Profile::~E2Profile()
{
	s->sync();
}
#endif

int E2Profile::GetBogoMips()
{
	QString sp = s->value("BogoMipsX1000", "0").toString();
	int rval = 0;           //Defaultvalue

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetBogoMips(int value)
{
	//      QString str;
	s->setValue ("BogoMipsX1000", QString::number(value));
	//      if ( decnum2str(value, str, MAXNUMDIGIT) == OK )
	//      {
	//              return s->setValue("BogoMipsX1000", str);
	//      }
	//      else
	//      {
	//              return BADPARAM;
	//      }
}


#include "eeptypes.h"


long E2Profile::GetLastDevType()
{
	QString sp = s->value("DeviceType", "24XX Auto").toString();

	if (sp.length())
	{
		return GetEEPTypeFromString(sp);
	}
	else
	{
		return E2400;        //Default device type
	}
}


void E2Profile::SetConfigFile(const QString &nm)
{
	if (nm.length())
	{
		filename = nm;
	}
}


void E2Profile::SetLastDevType(long devtype)
{
	QString sp = GetEEPTypeString(devtype);

	if (sp.length())
	{
		s->setValue("DeviceType", sp.remove(QChar(' ')));
	}
}


HInterfaceType E2Profile::GetParInterfType()
{
	extern HInterfaceType NameToInterfType(const QString & name);

	QString v = s->value("InterfaceType", "SI-ProgAPI").toString();

	return NameToInterfType(v);
}


void E2Profile::SetParInterfType(HInterfaceType type)
{
	QString TypeToInterfName(HInterfaceType type);

	s->setValue("InterfaceType", TypeToInterfName(type));
}


int E2Profile::GetParPortNo()
{
	QString sp = s->value("PortNumber", "0").toString();
	int rval = -1;

	if (sp.length() > 0)
	{
		bool ok;
		rval = sp.toInt(&ok);

		if (ok == false )
		{
			rval = -1;
		}
	}

	return rval;
}


void E2Profile::SetParPortNo(int port)
{
	//      int rval = BADPARAM;

	if (port >= 1 && port <= 4)
	{
		s->setValue("PortNumber", QString::number(port));
	}
}


/**
void E2Profile::SetLastFile(QString &name)
{
        int rval = -1;
        if (name && strlen(name))
        {
                char *sp = (char *)GetLastFile();
                if (sp && strcasecmp(name, sp) != 0)
                        s->setValue("PreviousFile", sp);
                s->setValue("LastFile", (char *)name);
        }
        return rval;
}


QString &E2Profile::GetLastFile()
{
        return s->value("LastFile");
}


QString &E2Profile::GetPrevFile()
{
        return s->value("PreviousFile");

**/


QString E2Profile::GetLastScript()
{
	return s->value("LastScript", "").toString();
}


void E2Profile::SetLastScript(const QString &name)
{
	s->setValue("LastScript", name);
}


void E2Profile::SetLastFile(const QString &name, int data)
{
	if (name.length())
	{
		QString str;
		int n;
		QString sp = GetLastFile(n);

		if (sp.length() && (name != sp))
		{
			str = sp;

			if (n == PROG_TYPE)
			{
				str += "?PROG";
			}
			else if (n == DATA_TYPE)
			{
				str += "?DATA";
			}
			else
			{
				str += "?ALL";
			}

			s->setValue("PreviousFile", str.toLatin1());
		}

		str = name;

		if (data == PROG_TYPE)
		{
			str += "?PROG";
		}
		else if (data == DATA_TYPE)
		{
			str += "?DATA";
		}
		else
		{
			str += "?ALL";
		}

		s->setValue("LastFile", str.toLatin1());
	}
}

QString param_copy;


QString E2Profile::GetLastFile(int &data)
{
	QString sp = s->value("LastFile", "").toString();

	data = ALL_TYPE;

	if (sp.length())
	{
		param_copy = sp;

		int p = sp.indexOf('?');

		if (p >= 0)
		{
			if (sp.mid(p + 1) == "DATA")
			{
				data = DATA_TYPE;
			}
			else if (sp.mid(p + 1) == "PROG")
			{
				data = PROG_TYPE;
			}
		}
	}

	return sp;
}


QString E2Profile::GetPrevFile(int &data)
{
	QString sp = s->value("PreviousFile", "").toString();

	data = ALL_TYPE;

	if (sp.length())
	{
		param_copy = sp;

		int p = sp.indexOf('?');

		if (p >= 0)
		{
			if (sp.mid(p + 1) == "DATA")
			{
				data = DATA_TYPE;
			}
			else if (sp.mid(p + 1) == "PROG")
			{
				data = PROG_TYPE;
			}
		}
	}

	return sp;
}


uint8_t E2Profile::GetPolarityControl()
{
	uint8_t res;
	QString rval;

	res = 0;
	rval = s->value("ClockPolarity").toString();

	if (rval.length())
		if (rval != "INV")
		{
			res |= CLOCKINV;
		}

	rval = s->value("ResetPolarity").toString();

	if (rval.length())
		if (rval != "INV")
		{
			res |= RESETINV;
		}

	rval = s->value("DOutPolarity").toString();

	if (rval.length())
		if (rval != "INV")
		{
			res |= DOUTINV;
		}

	rval = s->value("DInPolarity").toString();

	if (rval.length())
		if (rval != "INV")
		{
			res |= DININV;
		}

	return res;
}


void E2Profile::SetPolarityControl(uint8_t polarity_control)
{
	int rval = OK;

	if (rval == OK)
		s->setValue("ResetPolarity",
		            (polarity_control & RESETINV) ? "INV" : "TRUE");

	if (rval == OK)
		s->setValue("ClockPolarity",
		            (polarity_control & CLOCKINV) ? "INV" : "TRUE");

	if (rval == OK)
		s->setValue("DOutPolarity",
		            (polarity_control & DOUTINV) ? "INV" : "TRUE");

	if (rval == OK)
		s->setValue("DInPolarity",
		            (polarity_control & DININV) ? "INV" : "TRUE");
}


int E2Profile::GetI2CPageWrite()
{
	QString sp = s->value("I2CBusPageWrite", "16").toString();
	int rval = 16;          //Default: 16 bytes page write (only for 16-bit I2C Bus eeprom)

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetI2CPageWrite(int page_write)
{
	if (page_write > 0)
	{
		s->setValue("I2CBusPageWrite", QString::number(page_write));
	}
}


int E2Profile::GetSPIPageWrite()
{
	QString sp = s->value("BigSPIPageWrite", "16").toString();
	int rval = 16;          //Default: 16 bytes page write

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetSPIPageWrite(int page_write)
{
	if (page_write > 0)
	{
		s->setValue("BigSPIPageWrite", QString::number(page_write));
	}
}


int E2Profile::GetI2CBaseAddr()
{
	QString sp = s->value("I2CBaseAddress", "0xa0").toString();
	int rval = 0xA0;                //Default base address

	if (sp.length())
	{
		bool ok;
		rval = sp.toLong(&ok, 16);

		if (ok == false)
		{
			rval = 0xA0;
		}
	}

	return rval;
}


void E2Profile::SetI2CBaseAddr(int base_addr)
{
	if (base_addr >= 0x00 && base_addr < 0x100)
	{
		s->setValue("I2CBaseAddress", QString::number(base_addr));
	}
}


int E2Profile::GetI2CSpeed()
{
	QString sp = s->value("I2CBusSpeed", "NORMAL").toString();
	int rval = NORMAL;              //Default speed

	if (sp.length())
	{
		if ( sp == "TURBO")
		{
			rval = TURBO;
		}
		else if ( sp == "FAST")
		{
			rval = FAST;
		}
		else if ( sp == "SLOW")
		{
			rval = SLOW;
		}
		else if ( sp == "VERYSLOW")
		{
			rval = VERYSLOW;
		}
		else if ( sp == "ULTRASLOW")
		{
			rval = ULTRASLOW;
		}
	}

	return rval;
}


void E2Profile::SetI2CSpeed(int speed)
{
	if (speed == TURBO)
	{
		s->setValue("I2CBusSpeed", "TURBO");
	}
	else if (speed == FAST)
	{
		s->setValue("I2CBusSpeed", "FAST");
	}
	else if (speed == NORMAL)
	{
		s->setValue("I2CBusSpeed", "NORMAL");
	}
	else if (speed == SLOW)
	{
		s->setValue("I2CBusSpeed", "SLOW");
	}
	else if (speed == VERYSLOW)
	{
		s->setValue("I2CBusSpeed", "VERYSLOW");
	}
	else if (speed == ULTRASLOW)
	{
		s->setValue("I2CBusSpeed", "ULTRASLOW");
	}
}


int E2Profile::GetSPIResetPulse()
{
	QString sp = s->value("SPIResetPulse", "100").toString();
	int rval = 100;         //Default: 100 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetSPIResetPulse(int delay)
{
	if (delay > 0)
	{
		s->setValue("SPIResetPulse", QString::number(delay));
	}
}


int E2Profile::GetSPIDelayAfterReset()
{
	QString sp = s->value("SPIDelayAfterReset", "50").toString();
	int rval = 50;          //Default: 50 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetSPIDelayAfterReset(int delay)
{
	if (delay > 0)
	{
		s->setValue("SPIDelayAfterReset", QString::number(delay));
	}
}


int E2Profile::GetAT89DelayAfterReset()
{
	QString sp = s->value("AT89DelayAfterReset", "50").toString();
	int rval = 50;          //Default: 50 msec

	if (sp.length())
	{
		rval = sp.length();
	}

	return rval;
}


void E2Profile::SetAT89DelayAfterReset(int delay)
{
	if (delay > 0)
	{
		s->setValue("AT89DelayAfterReset", QString::number(delay));
	}
}


int E2Profile::GetAVRDelayAfterReset()
{
	QString sp = s->value("AVRDelayAfterReset", "50").toString();
	int rval = 50;          //Default: 50 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetAVRDelayAfterReset(int delay)
{
	if (delay > 0)
	{
		s->setValue("AVRDelayAfterReset", QString::number(delay));
	}
}


int E2Profile::GetSPISpeed()
{
	QString sp = s->value("SPIBusSpeed", "NORMAL").toString();
	int rval = NORMAL;              //Default speed

	if (sp.length())
	{
		if ( sp == "TURBO")
		{
			rval = TURBO;
		}
		else if ( sp == "FAST")
		{
			rval = FAST;
		}
		else if ( sp == "SLOW")
		{
			rval = SLOW;
		}
		else if ( sp == "VERYSLOW")
		{
			rval = VERYSLOW;
		}
		else if ( sp == "ULTRASLOW")
		{
			rval = ULTRASLOW;
		}
	}

	return rval;
}


void E2Profile::SetSPISpeed(int speed)
{
	if (speed == TURBO)
	{
		s->setValue("SPIBusSpeed", "TURBO");
	}
	else if (speed == FAST)
	{
		s->setValue("SPIBusSpeed", "FAST");
	}
	else if (speed == NORMAL)
	{
		s->setValue("SPIBusSpeed", "NORMAL");
	}
	else if (speed == SLOW)
	{
		s->setValue("SPIBusSpeed", "SLOW");
	}
	else if (speed == VERYSLOW)
	{
		s->setValue("SPIBusSpeed", "VERYSLOW");
	}
	else if (speed == ULTRASLOW)
	{
		s->setValue("SPIBusSpeed", "ULTRASLOW");
	}
}


int E2Profile::GetMegaPageDelay()
{
	QString sp = s->value("ATMegaPageWriteDelay", "50").toString();
	int rval = 50;          //Default: 50 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetMegaPageDelay(int delay)
{
	if (delay > 0)
	{
		s->setValue("ATMegaPageWriteDelay", QString::number(delay));
	}
}


int E2Profile::GetMicroWireSpeed()
{
	QString sp = s->value("MicroWireBusSpeed", "NORMAL").toString();
	int rval = NORMAL;              //Default speed

	if (sp.length())
	{
		if ( sp == "TURBO")
		{
			rval = TURBO;
		}
		else if ( sp == "FAST")
		{
			rval = FAST;
		}
		else if ( sp == "SLOW")
		{
			rval = SLOW;
		}
		else if ( sp == "VERYSLOW")
		{
			rval = VERYSLOW;
		}
		else if ( sp == "ULTRASLOW")
		{
			rval = ULTRASLOW;
		}
	}

	return rval;
}


void E2Profile::SetMicroWireSpeed(int speed)
{
	if (speed == TURBO)
	{
		s->setValue("MicroWireBusSpeed", "TURBO");
	}
	else if (speed == FAST)
	{
		s->setValue("MicroWireBusSpeed", "FAST");
	}
	else if (speed == NORMAL)
	{
		s->setValue("MicroWireBusSpeed", "NORMAL");
	}
	else if (speed == SLOW)
	{
		s->setValue("MicroWireBusSpeed", "SLOW");
	}
	else if (speed == VERYSLOW)
	{
		s->setValue("MicroWireBusSpeed", "VERYSLOW");
	}
	else if (speed == ULTRASLOW)
	{
		s->setValue("MicroWireBusSpeed", "ULTRASLOW");
	}
}


int E2Profile::GetPICSpeed()
{
	QString sp = s->value("PICBusSpeed", "NORMAL").toString();
	int rval = NORMAL;              //Default speed

	if (sp.length())
	{
		if ( sp == "TURBO")
		{
			rval = TURBO;
		}
		else if ( sp == "FAST")
		{
			rval = FAST;
		}
		else if ( sp == "SLOW")
		{
			rval = SLOW;
		}
		else if ( sp == "VERYSLOW")
		{
			rval = VERYSLOW;
		}
		else if ( sp == "ULTRASLOW")
		{
			rval = ULTRASLOW;
		}
	}

	return rval;
}


void E2Profile::SetPICSpeed(int speed)
{
	if (speed == TURBO)
	{
		s->setValue("PICBusSpeed", "TURBO");
	}
	else if (speed == FAST)
	{
		s->setValue("PICBusSpeed", "FAST");
	}
	else if (speed == NORMAL)
	{
		s->setValue("PICBusSpeed", "NORMAL");
	}
	else if (speed == SLOW)
	{
		s->setValue("PICBusSpeed", "SLOW");
	}
	else if (speed == VERYSLOW)
	{
		s->setValue("PICBusSpeed", "VERYSLOW");
	}
	else if (speed == ULTRASLOW)
	{
		s->setValue("PICBusSpeed", "ULTRASLOW");
	}
}


int E2Profile::GetSDESpeed()
{
	QString sp = s->value("SDEBusSpeed", "NORMAL").toString();
	int rval = NORMAL;              //Default speed

	if (sp.length())
	{
		if ( sp == "TURBO")
		{
			rval = TURBO;
		}
		else if ( sp == "FAST")
		{
			rval = FAST;
		}
		else if ( sp == "SLOW")
		{
			rval = SLOW;
		}
		else if ( sp == "VERYSLOW")
		{
			rval = VERYSLOW;
		}
		else if ( sp == "ULTRASLOW")
		{
			rval = ULTRASLOW;
		}
	}

	return rval;
}


void E2Profile::SetSDESpeed(int speed)
{
	if (speed == TURBO)
	{
		s->setValue("SDEBusSpeed", "TURBO");
	}
	else if (speed == FAST)
	{
		s->setValue("SDEBusSpeed", "FAST");
	}
	else if (speed == NORMAL)
	{
		s->setValue("SDEBusSpeed", "NORMAL");
	}
	else if (speed == SLOW)
	{
		s->setValue("SDEBusSpeed", "SLOW");
	}
	else if (speed == VERYSLOW)
	{
		s->setValue("SDEBusSpeed", "VERYSLOW");
	}
	else if (speed == ULTRASLOW)
	{
		s->setValue("SDEBusSpeed", "ULTRASLOW");
	}
}


int E2Profile::GetIMBusSpeed()
{
	QString sp = s->value("IMBusSpeed", "NORMAL").toString();
	int rval = NORMAL;              //Default speed

	if (sp.length())
	{
		if ( sp == "TURBO")
		{
			rval = TURBO;
		}
		else if ( sp == "FAST")
		{
			rval = FAST;
		}
		else if ( sp == "SLOW")
		{
			rval = SLOW;
		}
		else if ( sp == "VERYSLOW")
		{
			rval = VERYSLOW;
		}
		else if ( sp == "ULTRASLOW")
		{
			rval = ULTRASLOW;
		}
	}

	return rval;
}


void E2Profile::SetIMBusSpeed(int speed)
{
	if (speed == TURBO)
	{
		s->setValue("IMBusSpeed", "TURBO");
	}
	else if (speed == FAST)
	{
		s->setValue("IMBusSpeed", "FAST");
	}
	else if (speed == NORMAL)
	{
		s->setValue("IMBusSpeed", "NORMAL");
	}
	else if (speed == SLOW)
	{
		s->setValue("IMBusSpeed", "SLOW");
	}
	else if (speed == VERYSLOW)
	{
		s->setValue("IMBusSpeed", "VERYSLOW");
	}
	else if (speed == ULTRASLOW)
	{
		s->setValue("IMBusSpeed", "ULTRASLOW");
	}
}


int E2Profile::GetPowerUpDelay()
{
	QString sp = s->value("PowerUpDelay", "200").toString();
	int rval = 200;         //Default: 200 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetPowerUpDelay(int delay)
{
	if (delay > 0)
	{
		s->setValue("PowerUpDelay", QString::number(delay));
	}
}


int E2Profile::GetAVRProgDelay()
{
	QString sp = s->value("AVRByteWriteDelay", "20").toString();
	int rval = 20;          //Default: 20 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetAVRProgDelay(int delay)
{
	if (delay > 0)
	{
		s->setValue("AVRByteWriteDelay", QString::number(delay));
	}
}


int E2Profile::GetFontSize()
{
	QString sp = s->value("FontSize", "9").toString();
	int rval = 9;          //Default: 20 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetFontSize(int sz)
{
	if (sz > 0)
	{
		s->setValue("FontSize", QString::number(sz));
	}
}


int E2Profile::GetAVREraseDelay()
{
	QString sp = s->value("AVREraseDelay", "50").toString();
	int rval = 50;          //Default: 50 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetAVREraseDelay(int delay)
{
	if (delay > 0)
	{
		s->setValue("AVREraseDelay", QString::number(delay));
	}
}


int E2Profile::GetMDAProgDelay()
{
	QString sp = s->value("MDAWriteDelay", "30").toString();
	int rval = 30;          //Default: 30 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetMDAProgDelay(int delay)
{
	return s->setValue("MDAWriteDelay", QString::number(delay));
}


int E2Profile::GetNVMProgDelay()
{
	QString sp = s->value("NVMWriteDelay", "30").toString();
	int rval = 30;          //Default: 30 msec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetNVMProgDelay(int delay)
{
	return s->setValue("NVMWriteDelay", QString::number(delay));
}


unsigned long E2Profile::GetSerialNumVal()
{
	QString sp = s->value("SerialNumberVal", "0").toString();
	unsigned long rval = 0;         //Default 0

	if (sp.length())
	{
		rval = sp.toULong();
	}

	return rval;
}


void E2Profile::SetSerialNumVal(unsigned long val)
{
	if (val > 0)
	{
		s->setValue("SerialNumberVal", QString::number(val));
	}
}


int E2Profile::GetSerialNumAddress(long &start, int &size, bool &mtype)
{
	QString sp;

	start = 0;
	size = 1;
	mtype = false;

	if ( (sp = s->value("SerialNumberAddr", "0").toString()).length() )
	{
		start = sp.toLong();
	}

	if ( (sp = s->value("SerialNumberSize", "1").toString()).length() )
	{
		size = sp.toInt();
	}

	if ( (sp = s->value("SerialNumberType", "PROG").toString()).length() )
	{
		if (sp == "DATA")
		{
			mtype = true;
		}
	}

	return OK;
}


void E2Profile::SetSerialNumAddress(unsigned long start, int size, bool mtype)
{
//	if (start >= 0)
	{
		s->setValue("SerialNumberAddr", QString::number(start));
	}

	if (size >= 1)
	{
		s->setValue("SerialNumberSize", QString::number(size));
	}

	s->setValue("SerialNumberType", mtype ? "DATA" : "PROG");
}


FmtEndian E2Profile::GetSerialNumFormat()
{
	QString sp = s->value("SerialNumberFormat", "BIGENDIAN").toString();

	if ( sp.length() && ("LITTLEENDIAN" == sp))
	{
		return FMT_LITTLE_ENDIAN;
	}
	else
	{
		return FMT_BIG_ENDIAN;
	}
}


void E2Profile::SetSerialNumFormat(FmtEndian fmt)
{
	if (fmt == FMT_BIG_ENDIAN)
	{
		s->setValue("SerialNumberFormat", "BIGENDIAN");
	}
	else
	{
		s->setValue("SerialNumberFormat", "LITTLEENDIAN");
	}
}


bool E2Profile::GetSerialNumAutoInc()
{
	QString sp = s->value("SerialNumAutoIncrement", "YES").toString();

	if (sp.length() && (sp == "NO"))
	{
		return false;
	}
	else
	{
		return true;
	}
}


void E2Profile::SetSerialNumAutoInc(bool val)
{
	if (val)
	{
		s->setValue("SerialNumAutoIncrement", "YES");
	}
	else
	{
		s->setValue("SerialNumAutoIncrement", "NO");
	}
}


long E2Profile::GetProgramOptions()
{
	long res;
	QString rval;

	res = 0;
	rval = s->value("ReloadOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= RELOAD_YES;
		}

	rval = s->value("ReadFlashOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= READFLASH_YES;
		}

	rval = s->value("ReadEEpromOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= READEEP_YES;
		}

	rval = s->value("ByteSwapOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= BYTESWAP_YES;
		}

	rval = s->value("SetIDkeyOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= SETID_YES;
		}

	rval = s->value("ReadOscCalibration", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= READOSCAL_YES;
		}

	rval = s->value("EraseOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= ERASE_YES;
		}

	rval = s->value("WriteFlashOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= FLASH_YES;
		}

	rval = s->value("WriteEEpromOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= EEPROM_YES;
		}

	rval = s->value("WriteSecurityOption", "").toString();

	if (rval.length())
		if (rval != "NO")
		{
			res |= LOCK_YES;
		}

	return res;
}


void E2Profile::SetProgramOptions(long prog_option)
{
	s->setValue("ReloadOption",
	            (prog_option & RELOAD_YES) ? "YES" : "NO");
	s->setValue("ReadFlashOption",
	            (prog_option & READFLASH_YES) ? "YES" : "NO");
	s->setValue("ReadEEpromOption",
	            (prog_option & READEEP_YES) ? "YES" : "NO");
	s->setValue("ByteSwapOption",
	            (prog_option & BYTESWAP_YES) ? "YES" : "NO");
	s->setValue("SetIDkeyOption",
	            (prog_option & SETID_YES) ? "YES" : "NO");
	s->setValue("ReadOscCalibration",
	            (prog_option & READOSCAL_YES) ? "YES" : "NO");
	s->setValue("EraseOption",
	            (prog_option & ERASE_YES) ? "YES" : "NO");
	s->setValue("WriteFlashOption",
	            (prog_option & FLASH_YES) ? "YES" : "NO");
	s->setValue("WriteEEpromOption",
	            (prog_option & EEPROM_YES) ? "YES" : "NO");
	s->setValue("WriteSecurityOption",
	            (prog_option & LOCK_YES) ? "YES" : "NO");
}


QString E2Profile::GetLogFileName()
{
	QString sp = s->value("LogFileName", "PonyProg.log").toString();

	return sp;
}


void E2Profile::SetLogFileName(const QString &name)
{
	if (name.length())
	{
		s->setValue("LogFileName", name);
	}
}


QString E2Profile::GetLockDir()
{
	QString sp = s->value("ttyLockDir",  "/var/lock").toString();

	return sp;
}


void E2Profile::SetLockDir(const QString &name)
{
	if (name.length())
	{
		s->setValue("ttyLockDir", name);
	}
}


QString E2Profile::GetDevDir()
{
	QString sp = s->value("ttyDevDir", "/dev").toString();

	return sp;
}


void E2Profile::SetDevDir(const QString &name)
{
	if (name.length())
	{
		s->setValue("ttyDevDir", name);
	}
}


QString E2Profile::GetDevName()
{
	QString sp = s->value("ttyDevName", "").toString();

	if (sp.length() == 0)
#ifdef  __linux__
		sp = "ttyS";

#else
		sp = "COM";
#endif
	return sp;
}


void E2Profile::SetDevName(const QString &name)
{
	if (name.length())
	{
		s->setValue("ttyDevName", name);
	}
}


QString E2Profile::GetHtmlBrowseApp()
{
	QString sp = s->value("HtmlBrowseApp", "").toString();

	if (sp.length() == 0)
	{
		sp = "konqueror";
	}

	return sp;
}


void E2Profile::SetHtmlBrowseApp(const QString &name)
{
	if (name.length())
	{
		s->setValue("HtmlBrowseApp", name);
	}
}


QString E2Profile::GetLanguageCode()
{
	QString sp = s->value("LanguageCode", "C").toString();

	if (sp.length() == 0)
	{
		sp = "C";
	}

	return sp;
}


void E2Profile::SetLanguageCode(const QString &name)
{
	if (name.length())
	{
		s->setValue("LanguageCode", name);
	}
}


bool E2Profile::GetLogEnabled()
{
	QString sp = s->value("LogEnabled", "").toString();

	if (sp.length() && (sp == "YES"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void E2Profile::SetLogEnabled(bool enabled)
{
	if (enabled)
	{
		s->setValue("LogEnabled", "YES");
	}
	else
	{
		s->setValue("LogEnabled", "NO");
	}
}


bool E2Profile::GetSkipStartupDialog()
{
	QString sp = s->value("SkipStartupDialog", "").toString();

	if (sp.length() && (sp == "YES"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void E2Profile::SetSkipStartupDialog(bool skip)
{
	if (skip)
	{
		s->setValue("SkipStartupDialog", "YES");
	}
	else
	{
		s->setValue("SkipStartupDialog", "NO");
	}
}


bool E2Profile::GetClearBufBeforeLoad()
{
	QString sp = s->value("ClearBufferBeforeLoad").toString();

	if (sp.length() && (sp == "YES"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void E2Profile::SetClearBufBeforeLoad(bool enabled)
{
	if (enabled)
	{
		s->setValue("ClearBufferBeforeLoad", "YES");
	}
	else
	{
		s->setValue("ClearBufferBeforeLoad", "NO");
	}
}


bool E2Profile::GetClearBufBeforeRead()
{
	QString sp = s->value("ClearBufferBeforeRead", "").toString();

	if (sp.length() && (sp == "YES"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void E2Profile::SetClearBufBeforeRead(bool enabled)
{
	if (enabled)
	{
		s->setValue("ClearBufferBeforeRead", "YES");
	}
	else
	{
		s->setValue("ClearBufferBeforeRead", "NO");
	}
}


bool E2Profile::GetAt89PageOp()
{
	QString sp = s->value("AT89SPageOpEnabled", "").toString();

	if (sp.length() && (sp == "NO"))
	{
		return false;
	}
	else
	{
		return true;
	}
}


void E2Profile::SetAt89PageOp(bool enabled)
{
	if (enabled)
	{
		s->setValue("AT89SPageOpEnabled", "YES");
	}
	else
	{
		s->setValue("AT89SPageOpEnabled", "NO");
	}
}


bool E2Profile::Get8253FallEdge()
{
	QString sp = s->value("AT89S8253FallEdgeSampling", "").toString();

	if (sp.length() && (sp == "NO"))
	{
		return false;
	}
	else
	{
		return true;
	}
}


void E2Profile::Set8253FallEdge(bool enabled)
{
	if (enabled)
	{
		s->setValue("AT89S8253FallEdgeSampling", "YES");
	}
	else
	{
		s->setValue("AT89S8253FallEdgeSampling", "NO");
	}
}


bool E2Profile::GetSoundEnabled()
{
	QString sp = s->value("SoundEnabled", "").toString();

	if (sp.length() && (sp == "YES"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void E2Profile::SetSoundEnabled(bool enabled)
{
	if (enabled)
	{
		s->setValue("SoundEnabled", "YES");
	}
	else
	{
		s->setValue("SoundEnabled", "NO");
	}
}


int E2Profile::GetCalibrationAddress(long &start, int &size, bool &mtype)
{
	QString sp;

	start = 0;
	size = 1;
	mtype = false;

	if ( (sp = s->value("OscCalibrationAddr", "0").toString()).length() )
	{
		start = sp.toLong();
	}

	if ( (sp = s->value("OscCalibrationSize", "1").toString()).length() )
	{
		size = sp.toInt();
	}

	if ( (sp = s->value("OscCalibrationMemType", "PROG").toString()).length() )
	{
		if (sp == "DATA")
		{
			mtype = true;
		}
	}

	return OK;
}


void E2Profile::SetCalibrationAddress(unsigned long start, int size, bool mtype)
{
	//      if (start >= 0)
	{
		s->setValue("OscCalibrationAddr", QString::number(start));
	}

	if (size >= 1)
	{
		s->setValue("OscCalibrationSize", QString::number(size));
	}

	s->setValue("OscCalibrationType", mtype ? "DATA" : "PROG");
}


int E2Profile::GetJDMCmd2CmdDelay()
{
	QString sp = s->value("JDM-CmdToCmdDelay", "4000").toString();
	int rval = 4000;                //Default: 4000 usec

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetJDMCmd2CmdDelay(int delay)
{
	if (delay >= 0)
	{
		s->setValue("JDM-CmdToCmdDelay", QString::number(delay));
	}
}


bool E2Profile::GetVerifyAfterWrite()
{
	QString sp = s->value("VerifyAfterWrite", "").toString();

	if (sp.length() && (sp == "NO"))
	{
		return false;
	}
	else
	{
		return true;
	}
}


void E2Profile::SetVerifyAfterWrite(bool enabled)
{
	if (enabled)
	{
		s->setValue("VerifyAfterWrite", "YES");
	}
	else
	{
		s->setValue("VerifyAfterWrite", "NO");
	}
}


bool E2Profile::GetAutoDetectPorts()
{
	QString sp = s->value("AutoDetectPorts", "").toString();

	if (sp.length() && (sp == "NO"))
	{
		return false;
	}
	else
	{
		return true;
	}
}


void E2Profile::SetAutoDetectPorts(bool enabled)
{
	if (enabled)
	{
		s->setValue("AutoDetectPorts", "YES");
	}
	else
	{
		s->setValue("AutoDetectPorts", "NO");
	}
}


int E2Profile::GetCOMAddress(int &com1, int &com2, int &com3, int &com4)
{
	QString sp = s->value("COMPorts").toString();

	com1 = 0x3F8;
	com2 = 0x2F8;
	com3 = 0x3E8;
	com4 = 0x2E8;

	if (sp.length())
	{
		sscanf(sp.toLatin1(), "%x,%x,%x,%x", &com1, &com2, &com3, &com4);
	}

	return OK;
}


void E2Profile::SetCOMAddress(int com1, int com2, int com3, int com4)
{
	QString str;//[STRBUFSIZE];

	if (com1 > 0)
	{
		if (com2 > 0)
		{
			if (com3 > 0)
			{
				if (com4 > 0)
				{
					str.sprintf("%X,%X,%X,%X", com1, com2, com3, com4);
				}
				else
				{
					str.sprintf("%X,%X,%X", com1, com2, com3);
				}
			}
			else
			{
				str.sprintf( "%X,%X", com1, com2);
			}
		}
		else
		{
			str.sprintf( "%X", com1);
		}

		s->setValue("COMPorts", str);
	}
}


int E2Profile::GetLPTAddress(int &lpt1, int &lpt2, int &lpt3)
{
	QString sp = s->value("LPTPorts").toString();

	lpt1 = 0x378;
	lpt2 = 0x278;
	lpt3 = 0x3BC;

	if (sp.length())
	{
		sscanf(sp.toLatin1(), "%x,%x,%x", &lpt1, &lpt2, &lpt3);
	}

	return OK;
}


void E2Profile::SetLPTAddress(int lpt1, int lpt2, int lpt3)
{
	QString str;

	if (lpt1 > 0)
	{
		if (lpt2 > 0)
		{
			if (lpt3 > 0)
			{
				str.sprintf("%X,%X,%X", lpt1, lpt2, lpt3);
			}
			else
			{
				str.sprintf("%X,%X", lpt1, lpt2);
			}
		}
		else
		{
			str.sprintf("%X", lpt1);
		}

		s->setValue("LPTPorts", str);
	}
}


FileType E2Profile::GetDefaultFileType()
{
	QString sp = s->value("DefaultFileType", "E2P").toString();
	FileType ft = E2P;

	if (sp.length())
	{
		if ( sp == "e2p" )
		{
			ft = E2P;
		}
		else if ( sp == "bin" )
		{
			ft = BIN;
		}
		else if ( sp == "csm" )
		{
			ft = CSM;
		}
		else if ( sp == "intel-hex")
		{
			ft = INTEL;
		}
		else if ( sp == "mot-srec")
		{
			ft = MOTOS;
		}
	}

	return ft;
}


void E2Profile::SetDefaultFileType(FileType ft)
{
	QString str;

	if (ft == E2P)
	{
		str = "E2P";
	}
	else if (ft == BIN)
	{
		str = "BIN";
	}
	else if (ft == CSM)
	{
		str = "CSM";
	}
	else if (ft == INTEL)
	{
		str = "INTEL-HEX";
	}
	else if (ft == MOTOS)
	{
		str = "MOT-SREC";
	}

	if (str.length())
	{
		s->setValue("DefaultFileType", str);
	}
}


//RaspberryPi default pins
#define DEF_GPIO_CTRL                   23  //Rst pin 16
#define DEF_GPIO_DATAIN                 27  //Miso pin 13
#define DEF_GPIO_DATAOUT                17  //Mosi pin 11
#define DEF_GPIO_CLOCK                  24  //Clock pin 18

int E2Profile::GetGpioPinCtrl()
{
	QString sp;
	int rval = DEF_GPIO_CTRL;               //Default pin number

	sp = s->value("GpioPinCtrl", "").toString();

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetGpioPinCtrl(int pin)
{
	s->setValue("GpioPinCtrl", QString::number(pin));
}


int E2Profile::GetGpioPinClock()
{
	QString sp;
	int rval = DEF_GPIO_CLOCK;              //Default pin number

	sp = s->value("GpioPinClock").toString();

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetGpioPinClock(int pin)
{
	s->setValue("GpioPinClock", QString::number(pin));
}


int E2Profile::GetGpioPinDataIn()
{
	QString sp;
	int rval = DEF_GPIO_DATAIN;             //Default pin number

	sp = s->value("GpioPinDataIn").toString();

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetGpioPinDataIn(int pin)
{
	s->setValue("GpioPinDataIn", QString::number(pin));
}


int E2Profile::GetGpioPinDataOut()
{
	QString sp;
	int rval = DEF_GPIO_DATAOUT;    //Default pin number

	sp = s->value("GpioPinDataOut").toString();

	if (sp.length())
	{
		rval = sp.toInt();
	}

	return rval;
}


void E2Profile::SetGpioPinDataOut(int pin)
{
	s->setValue("GpioPinDataOut", QString::number(pin));
}

bool E2Profile::GetEditBufferEnabled()
{
	return !(s->value("Editor/ReadOnlyMode", false).toBool());
}

void E2Profile::SetEditBufferEnabled(bool enable)
{
	s->setValue("Editor/ReadOnlyMode", !enable);
}
