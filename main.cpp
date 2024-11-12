/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Mon Feb 17 13:05:59 IST 2003
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

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>

#include <GL/glut.h>

#include "CadSurf.h"

int main(int argc, char *argv[])
{
//  QApplication::setStyle(QStyleFactory::create("Motif"));
  glutInit(&argc, argv);
  QApplication a(argc, argv);
  QTranslator tor( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
  tor.load( QString("cadsurf.") + QTextCodec::locale(), "." );
  a.installTranslator( &tor );

  CadSurfApp *cadsurf=new CadSurfApp();
  a.setMainWidget(cadsurf);

  cadsurf->show();

  if(argc>1)
    cadsurf->openDocumentFile(argv[1]);
	else
	  cadsurf->openDocumentFile();
	
  return a.exec();
}
