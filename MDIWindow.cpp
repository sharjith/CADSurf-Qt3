/***************************************************************************
                          mdiwindow.cpp  -  description
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

#include "MDIWindow.h"

MDIWindow::MDIWindow(QWidget *parent, const char *name , WFlags flags) : QMainWindow(parent,name, flags)
{
    setDockEnabled(Qt::DockTornOff, true);
}

MDIWindow::~MDIWindow()
{
}
