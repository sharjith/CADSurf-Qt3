/***************************************************************************
                          mdiwindow.h  -  description
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Sharjith
    email                : sharjith_ssn@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include <qwidget.h>
#include <qmainwindow.h>

#include "CadSurfView.h"

/**
  *@author Sharjith
  */

class MDIWindow : public QMainWindow  {
   Q_OBJECT
public: 
	MDIWindow(QWidget *parent=0, const char *name=0, WFlags = WType_TopLevel);
	~MDIWindow();
	/*CadSurfView* GetView() const { return myView; }
private:
	CadSurfView* myView;*/
};

#endif
