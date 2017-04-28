//=========================================================================//
//                                                                         //
//  PonyProg - Serial Device Programmer                                    //
//                                                                         //
//  Copyright (C) 1997-2017   Claudio Lanconelli                           //
//                                                                         //
//  http://ponyprog.sourceforge.net                                        //
//                                                                         //
//-------------------------------------------------------------------------//
// $Id: sernumdlg.h,v 1.4 2009/11/16 23:40:43 lancos Exp $
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

#ifndef SerNumDIALOG_H
#define SerNumDIALOG_H

#include "ui_osccalibr.h"
#include "ui_sernumcfg.h"

#include <QString>
#include <QDialog>


#include "types.h"
#include "e2profil.h"
#include "e2awinfo.h"

#include "string_table.h"

using namespace Translator;

class SerNumDialog : public QDialog, public Ui::SNCfgDialog
{
	Q_OBJECT
  public:               //---------------------------------------- public
	SerNumDialog(QWidget *bw, const QString title = STR_MSGSERNUMCFG);
	virtual ~SerNumDialog();                // Destructor

  private slots:
	void onOk();

  protected:    //--------------------------------------- protected

  private:              //--------------------------------------- private
	void setTextWidgets();
  private:
	long loc;
	uint32_t val;
	bool memtype;
	bool autoinc;
	int size;
	FmtEndian fmt;
};


class OscCalibDialog : public QDialog, public Ui::OscCalibrDialog
{
	Q_OBJECT
  public:               //---------------------------------------- public
	OscCalibDialog(QWidget *bw, e2AppWinInfo *aw, const QString title = STR_MSGOSCCALIBCFG);
	virtual ~OscCalibDialog();              // Destructor

  private slots:
	void onOk();
	void onRead();

  protected:    //--------------------------------------- protected

  private:              //--------------------------------------- private
	long loc;
	uint8_t val;
	bool memtype;
	int size;

	e2AppWinInfo *awip;
};

#endif
