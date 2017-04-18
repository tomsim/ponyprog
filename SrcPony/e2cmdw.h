//=========================================================================//
//                                                                         //
//  PonyProg - Serial Device Programmer                                    //
//                                                                         //
//  Copyright (C) 1997-2017   Claudio Lanconelli                           //
//                                                                         //
//  http://ponyprog.sourceforge.net                                        //
//                                                                         //
//-------------------------------------------------------------------------//
// $Id: e2cmdw.h,v 1.9 2009/07/08 10:44:00 lancos Exp $
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

#ifndef e2CMDW_H
#define e2CMDW_H


#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QCloseEvent>
#include <QComboBox>
#include <QKeyEvent>
#include <QScrollArea>
#include <QImage>
#include <QProgressDialog>
#include <QActionGroup>
#include <QMenu>
#include <QVector>


#include "device.h"

#include "e2app.h"
#include "e2awinfo.h"


#include "ui_mainwindow.h"
#include "qhexedit.h"


typedef enum
{
	verboseNo,
	verboseErr,
	verboseAll
} VerboseType;


class e2App;

class e2AppWinInfo;


class e2CmdWindow : public QMainWindow, public e2App, public Ui::MainWindow
{
	Q_OBJECT

public:               //---------------------------------------- public
	e2CmdWindow(QWidget *parent = 0);
	virtual ~e2CmdWindow();
	virtual int CloseAppWin();

	//      virtual void WindowCommand(ItemVal id, ItemVal val, CmdType cType);
	//      virtual void KeyIn(vKey keysym, unsigned int shift);

	void About();
	void Draw(/*int rows = 0, int cols = 0*/);
	void Print();
	//      int NextLine();
	//      int PrevLine();
	//      void ScrollV(int top);

	void PostInit();
	//      void CharEdit(int row = -1, int col = -1);

	int IsBufChanged() const;

	void SetTitle();

	QString GetFileName();

	QProgressDialog *GetProgressDlg()
	{
		return e2Prg;
	}

	long GetCurrentChipType()
	{
		return e2type_id;
	}


private slots:
	void onNew();
	void onOpen(); //
	void onSave(); //
	void onSaveAs(); //
	void onReload(); //
	void onPrint(); //
	void onAboutQt();

	void onWrite(); //
	void onRead(); //
	void onErase(); //
	void onVerify(); //
	void onRunScript(); //
	void onLoadScript(); //
	//      void onDebugger();  //
	void onReset(); //
	void onInterfSetup();  //
	void onGetInfo(); //
	void onHelp(); //
	void onAbout(); //
	void onEditBufToggle(); //
	void onCalibration(); //
	void onProgram(); //
	void onByteSwap();
	void onLastFile1();
	void onLastFile2();
	void onDoubleSize();
	//     void onOverwriteMode(bool b);
	void onWriteHEndurance();
	void onAskToSave();
	void onProgress(int val);
	void onCloseAllDialog();

	void onSelectEEPType(int val);
	void onEEPSubType(int val);
	void onSetSerialNumber();
	void onClearBuf();
	void onEditNote();
	void onReadCalibration(int idx);
	void onWriteCalibration(int idx);
	void onReadSecurity(bool display_dialog);
	void onWriteSecurity();
	void onFillBuf();
	void onSpecialBits();
	void onProgramOptions(); //
	void onSerialNumberOptions();
	void onOscCalibOption();
	void onClose(); //
	void onExit(); //
	void onDevType(int i);
	void onDevSubType(int i);

	// EK 2017: slots for signals from group actions
	void onSelectI2C8(QAction* a);
	void onSelectI2C16(QAction* a);
	void onSelectI2CAT17(QAction* a);
	void onSelectMW16(QAction* a);
	void onSelectMW8(QAction* a);
	void onSelectSPI(QAction* a);
	void onSelectAVR(QAction* a);
	void onSelectAT89S(QAction* a);
	void onSelectPIC16(QAction* a);
	void onSelectPIC12(QAction* a);
	void onSelectImBus(QAction* a);
	void onSelectSDE2506(QAction* a);
	void onSelectX244(QAction* a);

	//      void setOverwriteMode(bool);
	//      void dataChanged();


public:
	int CmdHelp();


protected:
	//      void CmdRemoteMode();


private:
	//All commands
	int CmdOpen(int type = ALL_TYPE, char *file = 0, long relocation = 0, int clear_buffer = -1);
	int CmdSave(int type = ALL_TYPE, char *file = 0, long relocation = 0);
	int CmdSaveAs(int type = ALL_TYPE, long relocation = 0);
	int CmdLastFile1();
	int CmdLastFile2();
	int CmdReload();
	int CmdPrint();
	int CmdRead(int type = ALL_TYPE);
	int CmdWrite(int type = ALL_TYPE, bool verify = true);
	int CmdVerify(int type = ALL_TYPE);
	int CmdErase(int type = ALL_TYPE);
	int CmdGetInfo();
	int CmdReset();
	int CmdReadLock();
	int CmdWrite(int type);
	//      int CmdRunScript();
	int CmdLoadScript();
	int CmdWriteLock();
	int CmdReadSpecial();
	int CmdWriteSpecial();
	int CmdReadSecurity(bool display_dialog);
	int CmdWriteSecurity();
	int CmdDoubleSize();
	int CmdClearBuf(int type = ALL_TYPE);
	int CmdFillBuf();
	int CmdByteSwap();
	int CmdCalibration();

	int CmdEditNote();
	int CmdSelectDevice(long new_type);
	int CmdSetDeviceType(int val); // or const QString* ?
	int CmdSetDeviceSubType(int val);
	int CmdProgram();
	int CmdSetSerialNumber();
	int CmdReadCalibration(int idx);
	int CmdRunScript(bool test_mode = false);
	int SpecialBits();
	int ProgramOptions();
	int SerialNumberOptions();
	int OscCalibOption();

	void createToolBarNotice();
	void createToolBarCbx();
	void reateToolBarNotice();
	void createStatusWidgets();

	int findItemInMenuVector(const QString &n);


private:              //--------------------------------------- private
	void createSignalSlotConnections();
	QString convertFilterListToString(const QStringList &lst);

	//      int NextPage();
	//      int PrevPage();
	//      void FirstPage();
	//      void LastPage();
	void createDeviceMenues();
	int SaveFile(int force_select = 0);
	int OpenFile(const QString &file = 0);
	void UpdateStrFromBuf();
	void UpdateStrFromStr(const QString &s1, const QString &s2 = 0);
	void UpdateStatusBar();
	//      void UpdateChipType(int pritype = -1, int subtype = -1);
	//      void SetChipSubType(int pritype, int subtype = 0);
	void UpdateMenuType(int new_type = 0, int new_subtype = 0);
	void UpdateFileMenu();
	//      int TypeToMenuId(long type);
	//      long MenuIdToType(QAction * id);
	//      void MenuIdToCbxId(int id, int &idx1, int &idx2);
	//      void TypeToCbxId(long type, int &idx1, int &idx2);
	void CbxMenuInit();
	long CbxIdToType(int idx1, int idx2);
	int OpenScript(const QString &file);

	int OnError(int err_no, const QString &msgerr = 0);
	int PlaySoundMsg(bool val);

	long GetDevSize() const;

	//      int CoordToIndex(int row, int col);
	//      void IndexToCoord(int index, int &row, int &col);
	void setMenuIndexes();
	void selectTypeSubtype(const QString &t, const QString &st);
	int ScriptError(int line_number, int arg_index, char *arg, const QString msg = "");

private:
	int idxI2Cbus8;
	int idxI2Cbus16;
	int idxI2CbusAT17;
	int idxMicroWire16;
	int idxMicroWire8;
	int idxSPI;
	int idxAVR;
	int idxAT89S;
	int idxPIC16;
	int idxPIC12;
	int idxImBus;
	int idxSDE2506;
	int idxX244;

	QLabel *txtComment;
	QLabel *txtID;

	QStringList arguments;

	QComboBox* cbxEEPType;
	QComboBox* cbxEEPSubType;
	QLabel *lblEEPInfo;
	QLabel *lblStringID;

	QLineEdit *txtEEPInfo;
	QLineEdit *txtStringID;
	QProgressBar *statusProgress;

	// Standard elements
	QHexEdit* e2HexEdit;             // For the canvas

	QProgressDialog *e2Prg;

	// Info for the TextCanvas
	int first_line;
	int pre_type;
	int pre_subtype;

	long e2type_id;

	// Index for checking type menu
	//      int type_index;

	// Edit buffer enabled (checked the menu)
	int editbuf_enabled;

	//      int curIndex;

	//Verbose mode
	VerboseType verbose;
};


#endif


