/***************************************************************************
  cadsurf.cpp  -  description
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

// Qt includes
#include <qvbox.h>
#include <qaccel.h>
#include <qdesktopwidget.h>

// application specific includes
#include "CadSurfView.h"
#include "CadSurfDoc.h"
#include "CadSurf.h"
#include "MDIWindow.h"

#include "res/appicon.xpm"

#include "res/filenew.xpm"
#include "res/fileopen.xpm"
#include "res/filesave.xpm"

#include "res/top.xpm"
#include "res/bottom.xpm"
#include "res/front.xpm"
#include "res/back.xpm"
#include "res/left.xpm"
#include "res/right.xpm"
#include "res/axo.xpm"
#include "res/reset.xpm"
#include "res/fitall.xpm"
#include "res/zoomwin.xpm"
#include "res/dmode.xpm"
#include "res/hlr.xpm"
#include "res/antial.xpm"

#include "res/points.xpm"
#include "res/line.xpm"
#include "res/circle.xpm"
#include "res/ellipse.xpm"
#include "res/parabola.xpm"
#include "res/hyperbola.xpm"
#include "res/bezier.xpm"
#include "res/bspline.xpm"
#include "res/plane.xpm"
#include "res/cylinder.xpm"
#include "res/cone.xpm"
#include "res/sphere.xpm"
#include "res/torus.xpm"
#include "res/extruded.xpm"
#include "res/revolved.xpm"
#include "res/ruled.xpm"
#include "res/pipe.xpm"
#include "res/translate.xpm"
#include "res/rotate.xpm"
#include "res/scale.xpm"
#include "res/reflect.xpm"

CadSurfApp::CadSurfApp()
{
    setCaption(tr("CadSurf " VERSION ) );

    printer = new QPrinter;
    untitledCount=0;
    pDocList = new QList<CadSurfDoc>();
    pDocList->setAutoDelete(true);

    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts
    initView();
    initActions();
    initMenuBar();
    initToolBar();
    initStatusBar();

    QDesktopWidget *d = QApplication::desktop();
    int w = d->width();     // returns desktop width
    int h = d->height();    // returns desktop height
    resize( w, h );

    viewToolBar->setOn(true);
    viewStatusBar->setOn(true);
    hb = new QHBox(statusBar());
    hb->setSpacing(2);
    cViewer = new QLabel(hb);
    tViewer = new QLabel(hb);
    statusBar()->addWidget(hb,0,TRUE);
}

CadSurfApp::~CadSurfApp()
{
    delete printer;
}

void CadSurfApp::initActions()
{
    QPixmap openIcon, saveIcon, newIcon;
    newIcon = QPixmap(filenew);
    openIcon = QPixmap(fileopen);
    saveIcon = QPixmap(filesave);

    //viewer toolbar
    QPixmap topIcon, bottomIcon, frontIcon, backIcon, leftIcon, rightIcon, axoIcon,
	    resetIcon, fitallIcon, zoomwinIcon, dmodeIcon, hlrIcon, antialIcon;
    topIcon = QPixmap(top);
    bottomIcon = QPixmap(bottom);
    frontIcon = QPixmap(front);
    backIcon = QPixmap(back);
    leftIcon = QPixmap(left_xpm);
    rightIcon = QPixmap(right_xpm);
    axoIcon = QPixmap(axo);
    resetIcon = QPixmap(reset_xpm);
    fitallIcon = QPixmap(fitall);
    zoomwinIcon = QPixmap(zoomwin);
    dmodeIcon = QPixmap(dmode);
    hlrIcon = QPixmap(hlr);
    antialIcon = QPixmap(antial);

    //object toolbar
    QPixmap pntIcon, linIcon, cirIcon, elpIcon, parIcon,
	    hypIcon, bezIcon, bspIcon, plnIcon, cylIcon, conIcon, sphIcon, torIcon,
	    extIcon, revIcon, rulIcon, pipIcon, traIcon, rotIcon, sclIcon, refIcon;

    pntIcon= QPixmap(points);
    linIcon= QPixmap(line);
    cirIcon= QPixmap(circle);
    elpIcon= QPixmap(ellipse);
    parIcon= QPixmap(parabola);
    hypIcon= QPixmap(hyperbola);
    bezIcon= QPixmap(bezier);
    bspIcon= QPixmap(bspline);

    plnIcon= QPixmap(plane);
    cylIcon= QPixmap(cylinder);
    conIcon= QPixmap(cone);
    sphIcon= QPixmap(sphere);
    torIcon= QPixmap(torus);
    extIcon= QPixmap(extruded);
    revIcon= QPixmap(revolved);
    rulIcon= QPixmap(ruled);
    pipIcon= QPixmap(pipe_xpm);
    traIcon= QPixmap(translate);
    rotIcon= QPixmap(rotate_xpm);
    sclIcon= QPixmap(scale);
    refIcon= QPixmap(reflect);


    fileNew = new QAction(tr("New File"), newIcon, tr("&New"), QAccel::stringToKey(tr("Ctrl+N")), this);
    fileNew->setStatusTip(tr("Creates a new document"));
    fileNew->setWhatsThis(tr("New File\n\nCreates a new document"));
    connect(fileNew, SIGNAL(activated()), this, SLOT(slotFileNew()));

    fileOpen = new QAction(tr("Open File"), openIcon, tr("&Open..."), 0, this);
    fileOpen->setStatusTip(tr("Opens an existing document"));
    fileOpen->setWhatsThis(tr("Open File\n\nOpens an existing document"));
    connect(fileOpen, SIGNAL(activated()), this, SLOT(slotFileOpen()));

    fileSave = new QAction(tr("Save File"), saveIcon, tr("&Save"), QAccel::stringToKey(tr("Ctrl+S")), this);
    fileSave->setStatusTip(tr("Saves the actual document"));
    fileSave->setWhatsThis(tr("Save File.\n\nSaves the actual document"));
    connect(fileSave, SIGNAL(activated()), this, SLOT(slotFileSave()));

    fileSaveAs = new QAction(tr("Save File As"), tr("Save &as..."), 0, this);
    fileSaveAs->setStatusTip(tr("Saves the actual document under a new filename"));
    fileSaveAs->setWhatsThis(tr("Save As\n\nSaves the actual document under a new filename"));
    connect(fileSaveAs, SIGNAL(activated()), this, SLOT(slotFileSave()));

    fileClose = new QAction(tr("Close File"), tr("&Close"), QAccel::stringToKey(tr("Ctrl+W")), this);
    fileClose->setStatusTip(tr("Closes the actual document"));
    fileClose->setWhatsThis(tr("Close File\n\nCloses the actual document"));
    connect(fileClose, SIGNAL(activated()), this, SLOT(slotFileClose()));

    filePrint = new QAction(tr("Print File"), tr("&Print"), QAccel::stringToKey(tr("Ctrl+P")), this);
    filePrint->setStatusTip(tr("Prints out the actual document"));
    filePrint->setWhatsThis(tr("Print File\n\nPrints out the actual document"));
    connect(filePrint, SIGNAL(activated()), this, SLOT(slotFilePrint()));

    fileQuit = new QAction(tr("Exit"), tr("E&xit"), QAccel::stringToKey(tr("Ctrl+Q")), this);
    fileQuit->setStatusTip(tr("Quits the application"));
    fileQuit->setWhatsThis(tr("Exit\n\nQuits the application"));
    connect(fileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

    editDeleteAll = new QAction(tr("Delete All"), tr("&Delete All"), 0, this);
    editDeleteAll->setStatusTip(tr("Delete All Objects In Context"));
    editDeleteAll->setWhatsThis(tr("Delete All\n\nDelete All Objects In Context"));
    connect(editDeleteAll, SIGNAL(activated()), this, SLOT(slotEditDeleteAll()));

    editDeleteSelected = new QAction(tr("Delete Selected"), tr("Delete &Selected"), 0, this);
    editDeleteSelected->setStatusTip(tr("Delete Selected Objects In Context"));
    editDeleteSelected->setWhatsThis(tr("Delete Selected\n\nDelete Selected Objects In Context"));
    connect(editDeleteSelected, SIGNAL(activated()), this, SLOT(slotEditDeleteSelected()));

    editCut = new QAction(tr("Cut"), tr("Cu&t"), QAccel::stringToKey(tr("Ctrl+X")), this);
    editCut->setStatusTip(tr("Cuts the selected section and puts it to the clipboard"));
    editCut->setWhatsThis(tr("Cut\n\nCuts the selected section and puts it to the clipboard"));
    connect(editCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

    editCopy = new QAction(tr("Copy"), tr("&Copy"), QAccel::stringToKey(tr("Ctrl+C")), this);
    editCopy->setStatusTip(tr("Copies the selected section to the clipboard"));
    editCopy->setWhatsThis(tr("Copy\n\nCopies the selected section to the clipboard"));
    connect(editCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

    editUndo = new QAction(tr("Undo"), tr("&Undo"), QAccel::stringToKey(tr("Ctrl+Z")), this);
    editUndo->setStatusTip(tr("Reverts the last editing action"));
    editUndo->setWhatsThis(tr("Undo\n\nReverts the last editing action"));
    connect(editUndo, SIGNAL(activated()), this, SLOT(slotEditUndo()));

    editPaste = new QAction(tr("Paste"), tr("&Paste"), QAccel::stringToKey(tr("Ctrl+V")), this);
    editPaste->setStatusTip(tr("Pastes the clipboard contents to actual position"));
    editPaste->setWhatsThis(tr("Paste\n\nPastes the clipboard contents to actual position"));
    connect(editPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

    viewDisplayAll = new QAction(tr("Display All"), tr("Display All"), 0, this, 0, false);
    viewDisplayAll->setStatusTip(tr("Display All/Display All Objects In Context"));
    viewDisplayAll->setWhatsThis(tr("Display All\n\nDisplay All Objects In Context"));
    connect(viewDisplayAll, SIGNAL(activated()), this, SLOT(slotViewDisplayAll()));

    viewDisplaySelected = new QAction(tr("Display Selected"), tr("Display Selected"), 0, this, 0, false);
    viewDisplaySelected->setStatusTip(tr("Display Selected/Display Selected Objects In Context"));
    viewDisplaySelected->setWhatsThis(tr("Display Selected\n\nDisplay Selected Objects In Context"));
    connect(viewDisplaySelected, SIGNAL(activated()), this, SLOT(slotViewDisplaySelected()));

    viewEraseAll = new QAction(tr("Erase All"), tr("Erase All"), 0, this, 0, false);
    viewEraseAll->setStatusTip(tr("Erase All/Erase All Objects In Context"));
    viewEraseAll->setWhatsThis(tr("Erase All\n\nErase All Objects In Context"));
    connect(viewEraseAll, SIGNAL(activated()), this, SLOT(slotViewEraseAll()));

    viewEraseSelected = new QAction(tr("Erase Selected"), tr("Erase Selected"), 0, this, 0, false);
    viewEraseSelected->setStatusTip(tr("Erase Selected/Erase Selected Objects In Context"));
    viewEraseSelected->setWhatsThis(tr("Erase Selected\n\nErase Selected Objects In Context"));
    connect(viewEraseSelected, SIGNAL(activated()), this, SLOT(slotViewEraseSelected()));

    viewToolBar = new QAction(tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
    viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
    viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
    connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

    viewStatusBar = new QAction(tr("Statusbar"), tr("&Statusbar"), 0, this, 0, true);
    viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
    viewStatusBar->setWhatsThis(tr("Statusbar\n\nEnables/disables the statusbar"));
    connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

    Shaded = new QAction(tr("Shaded"), tr("&Shaded"), 0, this, 0, false);
    Shaded->setStatusTip(tr("Sets Shaded Display Mode"));
    Shaded->setWhatsThis(tr("Sets Shaded Display Mode"));
    connect(Shaded, SIGNAL(activated()), this, SLOT(slotSetShaded()));

    Wireframe = new QAction(tr("Wireframe"), tr("&Wireframe"), 0, this, 0, false);
    Wireframe->setStatusTip(tr("Sets Wireframe Display Mode"));
    Wireframe->setWhatsThis(tr("Sets Wireframe Display Mode"));
    connect(Wireframe, SIGNAL(activated()), this, SLOT(slotSetWireframe()));

    pColor = new QAction(tr("Color"), tr("&Color"), 0, this, 0, false);
    pColor->setStatusTip(tr("Sets Color Of Selected Object"));
    pColor->setWhatsThis(tr("Sets Color Of Selected Object"));
    connect(pColor, SIGNAL(activated()), this, SLOT(slotSetColor()));

    pMaterial = new QAction(tr("Material"), tr("&Material"), 0, this, 0, false);
    pMaterial->setStatusTip(tr("Sets Material Of Selected Object"));
    pMaterial->setWhatsThis(tr("Sets Material Of Selected Object"));
    connect(pMaterial, SIGNAL(activated()), this, SLOT(slotSetMaterial()));

    gridToggle = new QAction(tr("On"), tr("On"), 0, this, 0, false);
    gridToggle->setStatusTip(tr("Toggles Grid On And Off"));
    gridToggle->setWhatsThis(tr("Toggles Grid On And Off"));
    connect(gridToggle, SIGNAL(activated()), this, SLOT(slotToggleGrid()));

    gridPara = new QAction(tr("Settings"), tr("Settings"), 0, this, 0, false);
    gridPara->setStatusTip(tr("Sets Parameters Of Grid"));
    gridPara->setWhatsThis(tr("Sets Parameters Of Grid"));
    connect(gridPara, SIGNAL(activated()), this, SLOT(slotGridPara()));

    lineSolid = new QAction(tr("Solid Line Style"), tr("Solid Line Style"), 0, this, 0, false);
    lineSolid->setStatusTip(tr("Sets The Line Style To Solid"));
    lineSolid->setStatusTip(tr("Sets The Line Style Of Selected Curve Entities To Solid"));
    connect(lineSolid, SIGNAL(activated()), this, SLOT(slotLineSolid()));

    lineDashed = new QAction(tr("Dashed Line Style"), tr("Dashed Line Style"), 0, this, 0, false);
    lineDashed->setStatusTip(tr("Sets The Line Style To Dashed"));
    lineDashed->setStatusTip(tr("Sets The Line Style Of Selected Curve Entities To Dashed"));
    connect(lineDashed, SIGNAL(activated()), this, SLOT(slotLineDashed()));

    lineDotted = new QAction(tr("Dotted Line Style"), tr("Dotted Line Style"), 0, this, 0, false);
    lineDotted->setStatusTip(tr("Sets The Line Style To Dotted"));
    lineDotted->setStatusTip(tr("Sets The Line Style Of Selected Curve Entities To Dotted"));
    connect(lineDotted, SIGNAL(activated()), this, SLOT(slotLineDotted()));

    lineDotDashed = new QAction(tr("DotDashed Line Style"), tr("DotDashed Line Style"), 0, this, 0, false);
    lineDotDashed->setStatusTip(tr("Sets The Line Style To DotDashed"));
    lineDotDashed->setStatusTip(tr("Sets The Line Style Of Selected Curve Entities To DotDashed"));
    connect(lineDotDashed, SIGNAL(activated()), this, SLOT(slotLineDotDashed()));

    backColor = new QAction(tr("Background Color"), tr("Background Color"), 0, this, 0, false);
    backColor->setStatusTip(tr("Sets Background Color"));
    backColor->setWhatsThis(tr("Sets Background Color"));
    connect(backColor, SIGNAL(activated()), this, SLOT(slotBackColor()));

    windowNewWindow = new QAction(tr("New Window"), tr("&New Window"), 0, this);
    windowNewWindow->setStatusTip(tr("Opens a new view for the current document"));
    windowNewWindow->setWhatsThis(tr("New Window\n\nOpens a new view for the current document"));
    connect(windowNewWindow, SIGNAL(activated()), this, SLOT(slotWindowNewWindow()));

    windowCascade = new QAction(tr("Cascade"), tr("&Cascade"), 0, this);
    windowCascade->setStatusTip(tr("Cascades all windows"));
    windowCascade->setWhatsThis(tr("Cascade\n\nCascades all windows"));
    connect(windowCascade, SIGNAL(activated()), pWorkspace, SLOT(cascade()));

    windowTile = new QAction(tr("Tile"), tr("&Tile"), 0, this);
    windowTile->setStatusTip(tr("Tiles all windows"));
    windowTile->setWhatsThis(tr("Tile\n\nTiles all windows"));
    connect(windowTile, SIGNAL(activated()), pWorkspace, SLOT(tile()));


    windowAction = new QActionGroup(this, 0, false);
    windowAction->insert(windowNewWindow);
    windowAction->insert(windowCascade);
    windowAction->insert(windowTile);

    helpAboutApp = new QAction(tr("About"), tr("&About..."), 0, this);
    helpAboutApp->setStatusTip(tr("About the application"));
    helpAboutApp->setWhatsThis(tr("About\n\nAbout the application"));
    connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

    //viewer actions
    viewTop = new QAction(tr("Top View"), topIcon, tr("Top"), QAccel::stringToKey(tr("Ctrl+T")), this);
    viewTop->setStatusTip(tr("Displays The Top View"));
    viewTop->setWhatsThis(tr("Top View\n\nDisplays The Top View"));
    connect(viewTop, SIGNAL(activated()), this, SLOT(slotTopView()));

    viewBottom = new QAction(tr("Bottom View"), bottomIcon, tr("Bottom"), QAccel::stringToKey(tr("Ctrl+B")), this);
    viewBottom->setStatusTip(tr("Displays The Bottom View"));
    viewBottom->setWhatsThis(tr("Bottom View\n\nDisplays The Bottom View"));
    connect(viewBottom, SIGNAL(activated()), this, SLOT(slotBottomView()));

    viewFront = new QAction(tr("Front View"), frontIcon, tr("Front"), QAccel::stringToKey(tr("Ctrl+F")), this);
    viewFront->setStatusTip(tr("Displays The Front View"));
    viewFront->setWhatsThis(tr("Front View\n\nDisplays The Front View"));
    connect(viewFront, SIGNAL(activated()), this, SLOT(slotFrontView()));

    viewBack = new QAction(tr("Back View"), backIcon, tr("Back"), QAccel::stringToKey(tr("Shift+B")), this);
    viewBack->setStatusTip(tr("Displays The Back View"));
    viewBack->setWhatsThis(tr("Back View\n\nDisplays The Back View"));
    connect(viewBack, SIGNAL(activated()), this, SLOT(slotBackView()));

    viewLeft = new QAction(tr("Left View"), leftIcon, tr("Left"), QAccel::stringToKey(tr("Ctrl+L")), this);
    viewLeft->setStatusTip(tr("Displays The Left View"));
    viewLeft->setWhatsThis(tr("Left View\n\nDisplays The Left View"));
    connect(viewLeft, SIGNAL(activated()), this, SLOT(slotLeftView()));

    viewRight = new QAction(tr("Right View"), rightIcon, tr("Right"), QAccel::stringToKey(tr("Ctrl+R")), this);
    viewRight->setStatusTip(tr("Displays The Right View"));
    viewRight->setWhatsThis(tr("Right View\n\nDisplays The Right View"));
    connect(viewRight, SIGNAL(activated()), this, SLOT(slotRightView()));

    viewAxo = new QAction(tr("Axo View"), axoIcon, tr("Axo"), QAccel::stringToKey(tr("Ctrl+A")), this);
    viewAxo->setStatusTip(tr("Displays The Axonometric View"));
    viewAxo->setWhatsThis(tr("Axo View\n\nDisplays The Axonometric View"));
    connect(viewAxo, SIGNAL(activated()), this, SLOT(slotAxoView()));

    viewFitall = new QAction(tr("Fit All"), fitallIcon, tr("Fit All"), QAccel::stringToKey(tr("F")), this);
    viewFitall->setStatusTip(tr("Fits The View To Screen"));
    viewFitall->setWhatsThis(tr("Fit All\n\nFits The View To Screen"));
    connect(viewFitall, SIGNAL(activated()), this, SLOT(slotFitall()));

    viewZoomwin = new QAction(tr("Zoom Win"), zoomwinIcon, tr("Zoom Win"), QAccel::stringToKey(tr("Z")), this);
    viewZoomwin->setStatusTip(tr("Zooms The View Defined By A Window"));
    viewZoomwin->setWhatsThis(tr("Zoom Win\n\nZooms The View Defined By A Window"));
    connect(viewZoomwin, SIGNAL(activated()), this, SLOT(slotZoomwin()));

    viewReset = new QAction(tr("Reset View"), resetIcon, tr("Axo"), 0, this);
    viewReset->setStatusTip(tr("Resets The View"));
    viewReset->setWhatsThis(tr("Reset View\n\nResets The View"));
    connect(viewReset, SIGNAL(activated()), this, SLOT(slotReset()));

    viewDmode = new QAction(tr("Display Mode"), dmodeIcon, tr("Display Mode"), 0, this);
    viewDmode->setStatusTip(tr("Toggles The Display Mode To Shaded And Wireframe"));
    viewDmode->setWhatsThis(tr("Display Mode\n\nToggles The Display Mode To Shaded And Wireframe"));
    connect(viewDmode, SIGNAL(activated()), this, SLOT(slotDmode()));

    viewHLR = new QAction(tr("HLR View"), hlrIcon, tr("HLR"), 0, this);
    viewHLR->setStatusTip(tr("Hidden Line Removal"));
    viewHLR->setWhatsThis(tr("HLR\n\nDisplays Hidden Lines Removed"));
    connect(viewHLR, SIGNAL(activated()), this, SLOT(slotHLR()));

    viewAntial = new QAction(tr("Antialias View"), antialIcon, tr("HLR"), 0, this, 0, false);
    viewAntial->setStatusTip(tr("Toggle Antialias"));
    viewAntial->setWhatsThis(tr("Toggle Antialias\n\nToggles Antialiasing On And Off"));
    connect(viewAntial, SIGNAL(activated()), this, SLOT(slotAntial()));

    mkPoint= new QAction(tr("Create Points"), pntIcon, tr("PNT"), 0, this);;
    connect(mkPoint, SIGNAL(activated()), this, SLOT(slotmkPoint()));

    mkLine= new QAction(tr("Create Line"), linIcon, tr("LIN"), 0, this);;
    connect(mkLine, SIGNAL(activated()), this, SLOT(slotmkLine()));

    mkCircle= new QAction(tr("Create Circle"), cirIcon, tr("CIR"), 0, this);;
    connect(mkCircle, SIGNAL(activated()), this, SLOT(slotmkCircle()));

    mkEllipse= new QAction(tr("Create Ellipse"), elpIcon, tr("ELP"), 0, this);
    connect(mkEllipse, SIGNAL(activated()), this, SLOT(slotmkEllipse()));

    mkParabola= new QAction(tr("Create Parabola"), parIcon, tr("PAR"), 0, this);;
    connect(mkParabola, SIGNAL(activated()), this, SLOT(slotmkParabola()));

    mkHyperbola= new QAction(tr("Create Hyperbola"), hypIcon, tr("HYP"), 0, this);;
    connect(mkHyperbola, SIGNAL(activated()), this, SLOT(slotmkHyperbola()));

    mkBezier= new QAction(tr("Create Bezier Curve"), bezIcon, tr("BEZ"), 0, this);;
    connect(mkBezier, SIGNAL(activated()), this, SLOT(slotmkBezier()));

    mkBSpline= new QAction(tr("Create B-Spline Curve"), bspIcon, tr("BSP"), 0, this);;
    connect(mkBSpline, SIGNAL(activated()), this, SLOT(slotmkBSpline()));

    mkPlane= new QAction(tr("Create Plane"), plnIcon, tr("PLN"), 0, this);;
    connect(mkPlane, SIGNAL(activated()), this, SLOT(slotmkPlane()));

    mkCylinder= new QAction(tr("Create Cylinder"), cylIcon, tr("CYL"), 0, this);;
    connect(mkCylinder, SIGNAL(activated()), this, SLOT(slotmkCylinder()));

    mkCone= new QAction(tr("Create Cone"), conIcon, tr("CON"), 0, this);;
    connect(mkCone, SIGNAL(activated()), this, SLOT(slotmkCone()));

    mkSphere= new QAction(tr("Create Sphere"), sphIcon, tr("SPH"), 0, this);;
    connect(mkSphere, SIGNAL(activated()), this, SLOT(slotmkSphere()));

    mkTorus= new QAction(tr("Create Torus"), torIcon, tr("TOR"), 0, this);;
    connect(mkTorus, SIGNAL(activated()), this, SLOT(slotmkTorus()));

    mkExtruded= new QAction(tr("Create Extruded Surface"), extIcon, tr("EXT"), 0, this);;
    connect(mkExtruded, SIGNAL(activated()), this, SLOT(slotmkExtruded()));

    mkRevolved= new QAction(tr("Create Revolved Surface"), revIcon, tr("REV"), 0, this);;
    connect(mkRevolved, SIGNAL(activated()), this, SLOT(slotmkRevolved()));

    mkRuled= new QAction(tr("Create Ruled Surface"), rulIcon, tr("RUL"), 0, this);;
    connect(mkRuled, SIGNAL(activated()), this, SLOT(slotmkRuled()));

    mkPipe= new QAction(tr("Create Pipe"), pipIcon, tr("PIP"), 0, this);;
    connect(mkPipe, SIGNAL(activated()), this, SLOT(slotmkPipe()));

    mkUserSurf= new QAction(tr("Create User Defined Surface Surface"), rulIcon, tr("USR"), 0, this);;
    connect(mkUserSurf, SIGNAL(activated()), this, SLOT(slotmkUserSurf()));

    mkTranslate= new QAction(tr("Translation"), traIcon, tr("TRA"), 0, this);;
    connect(mkTranslate, SIGNAL(activated()), this, SLOT(slotmkTranslate()));

    mkRotate= new QAction(tr("Rotation"), rotIcon, tr("ROT"), 0, this);;
    connect(mkRotate, SIGNAL(activated()), this, SLOT(slotmkRotate()));

    mkScale= new QAction(tr("Scaling"), sclIcon, tr("SCL"), 0, this);;
    connect(mkScale, SIGNAL(activated()), this, SLOT(slotmkScale()));

    mkReflect= new QAction(tr("Reflection"), refIcon, tr("REF"), 0, this);;
    connect(mkReflect, SIGNAL(activated()), this, SLOT(slotmkReflect()));

}

void CadSurfApp::initMenuBar()
{
    ///////////////////////////////////////////////////////////////////
    // MENUBAR

    ///////////////////////////////////////////////////////////////////
    // menuBar entry pFileMenu
    pFileMenu=new QPopupMenu();
    fileNew->addTo(pFileMenu);
    fileOpen->addTo(pFileMenu);
    fileClose->addTo(pFileMenu);
    pFileMenu->insertSeparator();
    fileSave->addTo(pFileMenu);
    fileSaveAs->addTo(pFileMenu);
    pFileMenu->insertSeparator();
    filePrint->addTo(pFileMenu);
    pFileMenu->insertSeparator();
    fileQuit->addTo(pFileMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry editMenu
    pEditMenu=new QPopupMenu();
    editDeleteAll->addTo(pEditMenu);
    editDeleteSelected->addTo(pEditMenu);
    pEditMenu->insertSeparator();
    editUndo->addTo(pEditMenu);
    pEditMenu->insertSeparator();
    editCut->addTo(pEditMenu);
    editCopy->addTo(pEditMenu);
    editPaste->addTo(pEditMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry viewMenu
    pViewMenu=new QPopupMenu();
    viewDisplayAll->addTo(pViewMenu);
    viewDisplaySelected->addTo(pViewMenu);
    viewEraseAll->addTo(pViewMenu);
    viewEraseSelected->addTo(pViewMenu);
    pViewMenu->insertSeparator();
    pViewMenu->setCheckable(true);
    viewToolBar->addTo(pViewMenu);
    viewStatusBar->addTo(pViewMenu);
    ///////////////////////////////////////////////////////////////////
    // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE
    ///////////////////////////////////////////////////////////////////
    // menuBar entry Preference
    pPreferenceMenu=new QPopupMenu();
    pDisplayMenu = new QPopupMenu(pPreferenceMenu);
    pPreferenceMenu->insertItem(tr("&Display Mode"), pDisplayMenu);
    Shaded->addTo(pDisplayMenu);
    Wireframe->addTo(pDisplayMenu);

    pColor->addTo(pPreferenceMenu);
    pMaterial->addTo(pPreferenceMenu);

    pLineStyleMenu = new QPopupMenu(pPreferenceMenu);
    pPreferenceMenu->insertItem(tr("&Line Style"), pLineStyleMenu);
    lineSolid->addTo(pLineStyleMenu);
    lineDashed->addTo(pLineStyleMenu);
    lineDotted->addTo(pLineStyleMenu);
    lineDotDashed->addTo(pLineStyleMenu);
    pPreferenceMenu->insertSeparator();
    pGridMenu = new QPopupMenu(pPreferenceMenu);
    pPreferenceMenu->insertItem(tr("&Grid"), pGridMenu);
    gridToggle->addTo(pGridMenu);
    gridPara->addTo(pGridMenu);

    pPreferenceMenu->insertSeparator();
    backColor->addTo(pPreferenceMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry windowMenu
    pWindowMenu = new QPopupMenu(this);
    pWindowMenu->setCheckable(true);
    connect(pWindowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

    ///////////////////////////////////////////////////////////////////
    // menuBar entry helpMenu
    pHelpMenu=new QPopupMenu();
    helpAboutApp->addTo(pHelpMenu);
    pHelpMenu->insertSeparator();
    pHelpMenu->insertItem(tr("What's &This"), this, SLOT(whatsThis()), SHIFT+Key_F1);


    menuBar()->insertItem(tr("&File"), pFileMenu);
    menuBar()->insertItem(tr("&Edit"), pEditMenu);
    menuBar()->insertItem(tr("&View"), pViewMenu);
    menuBar()->insertItem(tr("&Preferences"), pPreferenceMenu);
    menuBar()->insertItem(tr("&Window"), pWindowMenu);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("&Help"), pHelpMenu);

}

void CadSurfApp::initToolBar()
{
    ///////////////////////////////////////////////////////////////////
    // TOOLBAR
    fileToolbar = new QToolBar(this, "file operations");
    fileNew->addTo(fileToolbar);
    fileOpen->addTo(fileToolbar);
    fileSave->addTo(fileToolbar);
    fileToolbar->addSeparator();
    QWhatsThis::whatsThisButton(fileToolbar);

    objToolbar = new QToolBar(this, "Geometry");
    mkPoint->addTo(objToolbar);
    mkLine->addTo(objToolbar);
    mkCircle->addTo(objToolbar);
    mkEllipse->addTo(objToolbar);
    mkParabola->addTo(objToolbar);
    mkHyperbola->addTo(objToolbar);
    mkBezier->addTo(objToolbar);
    mkBSpline->addTo(objToolbar);
    objToolbar->addSeparator();
    mkPlane->addTo(objToolbar);
    mkCylinder->addTo(objToolbar);
    mkCone->addTo(objToolbar);
    mkSphere->addTo(objToolbar);
    mkTorus->addTo(objToolbar);
    mkExtruded->addTo(objToolbar);
    mkRevolved->addTo(objToolbar);
    mkRuled->addTo(objToolbar);
    mkPipe->addTo(objToolbar);	
    mkUserSurf->addTo(objToolbar);	
    objToolbar->addSeparator();
    mkTranslate->addTo(objToolbar);
    mkRotate->addTo(objToolbar);
    mkScale->addTo(objToolbar);
    mkReflect->addTo(objToolbar);

}

void CadSurfApp::initStatusBar()
{
    ///////////////////////////////////////////////////////////////////
    //STATUSBAR
    statusBar()->message(tr("Ready."));
}

void CadSurfApp::initView()
{
    ////////////////////////////////////////////////////////////////////
    // set the main widget here
    QVBox* view_back = new QVBox( this );
    view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    pWorkspace = new QWorkspace( view_back );
    pWorkspace->setScrollBarsEnabled(true);
    setCentralWidget(view_back);
}

void CadSurfApp::createClient(CadSurfDoc* doc)
{
    QPixmap appIcon = QPixmap(appicon);
    MDIWindow* mw = new MDIWindow(pWorkspace, 0, 0);
    mw->setIcon(appIcon);
    CadSurfView* w = new CadSurfView(doc, mw ,0,WDestructiveClose);
    mw->setCentralWidget(w);
    viewToolbar = new QToolBar(mw, " view Tool Bar");
    viewTop->addTo(viewToolbar);
    viewBottom->addTo(viewToolbar);
    viewFront->addTo(viewToolbar);
    viewBack->addTo(viewToolbar);
    viewLeft->addTo(viewToolbar);
    viewRight->addTo(viewToolbar);
    viewAxo->addTo(viewToolbar);
    viewToolbar->addSeparator();
    viewFitall->addTo(viewToolbar);
    viewZoomwin->addTo(viewToolbar);
    viewReset->addTo(viewToolbar);
    viewDmode->addTo(viewToolbar);
    viewHLR->addTo(viewToolbar);
    viewAntial->addTo(viewToolbar);
    w->installEventFilter(this);
    doc->addView(w);
    mw->showMaximized();
    cViewer->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
    cViewer->setMaximumSize(300,30);
    cViewer->setMinimumSize(300,30);
    cViewer->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tViewer->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
    tViewer->setMaximumSize(200,30);
    tViewer->setMinimumSize(200,30);
    tViewer->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //pWorkspace->tile();
}

void CadSurfApp::openDocumentFile(const char* file)
{
    statusBar()->message(tr("Opening file..."));
    CadSurfDoc* doc;
    // check, if document already open. If yes, set the focus to the first view
    for(doc=pDocList->first(); doc != 0; doc=pDocList->next())
    {
	if(doc->pathName()==file)
	{
	    CadSurfView* view=doc->firstView();	
	    view->setFocus();
	    return;
	}
    }
    doc = new CadSurfDoc();
    pDocList->append(doc);
    doc->newDocument();
    // Creates an untitled window if file is 0	
    if(!file)
    {
	untitledCount+=1;
	QString fileName=QString(tr("Untitled%1")).arg(untitledCount);
	doc->setPathName(fileName);
	doc->setTitle(fileName);
    }
    // Open the file
    else
    {
	if(!doc->openDocument(file))
	{
	    QMessageBox::critical(this, tr("Error !"),tr("Could not open document !"));
	    delete doc;
	    return;
	}
    }
    // create the window
    createClient(doc);

    statusBar()->message(tr("Ready."));
}

bool CadSurfApp::queryExit()
{
    int exit=QMessageBox::information(this, tr("Quit..."),
	    tr("Do your really want to quit?"),
	    QMessageBox::Ok, QMessageBox::Cancel);

    if (exit==1)
    {

    }
    else
    {

    };

    return (exit==1);
}

bool CadSurfApp::eventFilter(QObject* object, QEvent* event)
{
    if((event->type() == QEvent::Close)&&((CadSurfApp*)object!=this))
    {
	QCloseEvent* e=(QCloseEvent*)event;
	CadSurfView* pView=(CadSurfView*)object;
	CadSurfDoc* pDoc=pView->getDocument();
	if(pDoc->canCloseFrame(pView))
	{
	    pDoc->removeView(pView);
	    if(!pDoc->firstView())
		pDocList->remove(pDoc);

	    e->accept();
	}
	else
	    e->ignore();
    }
    return QWidget::eventFilter( object, event );    // standard event processing
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void CadSurfApp::slotFileNew()
{
    statusBar()->message(tr("Creating new file..."));

    openDocumentFile();		

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotFileOpen()
{
    statusBar()->message(tr("Opening file..."));

    QString fileName = QFileDialog::getOpenFileName(0,0,this);
    if (!fileName.isEmpty())
    {
	openDocumentFile(fileName);		
    }
    statusBar()->message(tr("Ready."));
}


void CadSurfApp::slotFileSave()
{
    statusBar()->message(tr("Saving file..."));

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    if(doc->title().contains(tr("Untitled")))
		slotFileSaveAs();
	    else
		if(!doc->saveDocument(doc->pathName()))
		    QMessageBox::critical (this, tr("I/O Error !"), tr("Could not save the current document !"));
	}
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotFileSaveAs()
{
    statusBar()->message(tr("Saving file under new filename..."));
    QString fn = QFileDialog::getSaveFileName(0, 0, this);
    if (!fn.isEmpty())
    {
	MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
	if(mw)
	{
	    CadSurfView* view = (CadSurfView*)mw->centralWidget();
	    if( view )
	    {	
		CadSurfDoc* doc = view->getDocument();
		if(!doc->saveDocument(fn))
		{
		    QMessageBox::critical (this, tr("I/O Error !"), tr("Could not save the current document !"));
		    return;
		}
		doc->changedViewList();
	    }
	}
    }
    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotFileClose()
{
    statusBar()->message(tr("Closing file..."));
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    CadSurfDoc* doc=view->getDocument();
	    doc->closeDocument();
	}
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotFilePrint()
{
    statusBar()->message(tr("Printing..."));
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	    view->print( printer );
    }
    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotFileQuit()
{ 
    statusBar()->message(tr("Exiting application..."));
    ///////////////////////////////////////////////////////////////////
    // exits the Application
    //  if(doc->isModified())
    //  {
    //    if(queryExit())
    //    {
    //      qApp->quit();
    //    }
    //    else
    //    {
    //
    //    };
    //  }
    //  else
    //  {
    qApp->quit();
    //  };

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotEditDeleteAll()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->dContext->DeleteAll();
	}
    }
}

void CadSurfApp::slotEditDeleteSelected()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->dContext->DeleteSelected();
	}
    }
}

void CadSurfApp::slotEditUndo()
{
    statusBar()->message(tr("Reverting last action..."));

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	    cout << "";
	//	view->undo();
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotEditCut()
{
    statusBar()->message(tr("Cutting selection..."));

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	    cout << "";
	//	view->cut();	
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotEditCopy()
{
    statusBar()->message(tr("Copying selection to clipboard..."));

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	    cout << "";
	//	view->copy();
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotEditPaste()
{
    statusBar()->message(tr("Inserting clipboard contents..."));

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	    cout << "";
	//	view->paste();
    }

    statusBar()->message(tr("Ready."));
}


void CadSurfApp::slotViewDisplayAll()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->dContext->DisplayAll();
	}
    }
}

void CadSurfApp::slotViewDisplaySelected()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->dContext->DisplaySelected();
	}
    }
}

void CadSurfApp::slotViewEraseAll()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->dContext->EraseAll();
	}
    }
}

void CadSurfApp::slotViewEraseSelected()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->dContext->EraseSelected();
	}
    }
}

void CadSurfApp::slotViewToolBar(bool toggle)
{
    statusBar()->message(tr("Toggle toolbar..."));
    ///////////////////////////////////////////////////////////////////
    // turn Toolbar on or off
    if (toggle== false)
    {
	fileToolbar->hide();
    }
    else
    {
	fileToolbar->show();
    };

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotViewStatusBar(bool toggle)
{
    statusBar()->message(tr("Toggle statusbar..."));
    ///////////////////////////////////////////////////////////////////
    //turn Statusbar on or off

    if (toggle == false)
    {
	statusBar()->hide();
    }
    else
    {
	statusBar()->show();
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotSetShaded()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->SetDisplayMode(GLSHADED);
	    view->dShaded = true;
	}
    }
}

void CadSurfApp::slotSetWireframe()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    view->GetView()->SetDisplayMode(GLWIREFRAME);
	    view->dShaded = false;
	}
    }
}

void CadSurfApp::slotToggleGrid()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	 	
	    view->gridOn = !view->gridOn;
	    view->setFocus();
	    if(!view->gridOn)
	    {
		view->GetView()->SetGridOn(false);
		gridToggle->setMenuText(tr("On"));
	    }
	    else
	    {
		view->GetView()->SetGridOn(true);
		gridToggle->setMenuText(tr("Off"));
	    }
	}
    }
}

#include "GridSettings.h"
void CadSurfApp::slotGridPara()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	 	
	    view->setFocus();
	    GridSettings dlg(0,0,TRUE);
	    if(dlg.exec()==QDialog::Accepted)
	    {
		double dx, dy, dz, size, step;
		dx = dlg.spinBox3->value();
		dy = dlg.spinBox4->value();
		dz = dlg.spinBox5->value();
		size = dlg.spinBox1->value();
		step = dlg.spinBox2->value();
		if(dx == 0 && dy == 0 && dz == 0)
		    dz=1;
		CPlane plane(dx, dy, dz, 0);
		view->GetView()->SetGridPara(plane, size, step);
	    }
	}
    }
}

void CadSurfApp::slotSetColor()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    QColor initial(255,0,0);
	    initial = QColorDialog::getColor ( initial, this, 0 );
	    if(!initial.isValid())
		return;
	    CGLDisplayContext* ctx = view->getDocument()->dContext;
	    for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	    {
		CGLObject* O = ctx->CurrentSelected();
		O->SetColor(initial.red(), initial.green(), initial.blue());
	    }
	}
    }
}

void CadSurfApp::slotBackColor()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    view->GetView()->SetBackgroundColor((GLfloat)col.red()/255.0, (GLfloat)col.green()/255.0, (GLfloat)col.blue()/255.0);
	}
    }
}

#include "MaterialDlg.h"
#include <GLMaterial.h>
void CadSurfApp::slotSetMaterial()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();

	    MatlEditor e;	    
	    if(e.exec()==QDialog::Accepted)
	    {
		CGLMaterial mat = e.GetMaterial();
		CGLDisplayContext* ctx = view->getDocument()->dContext;
		for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
		{
		    CGLObject* O = ctx->CurrentSelected();
		    O->SetMaterial(mat);
		}
	    }
	}
    }
}

//line style
void CadSurfApp::slotLineSolid()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    CGLDisplayContext* ctx = view->getDocument()->dContext;
	    for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	    {
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
		    CGLCurve* crv = static_cast<CGLCurve*>(O);
		    crv->SetLineStyle(LINE_SOLID);
		}
	    }
	}
    }
}

void CadSurfApp::slotLineDashed()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    CGLDisplayContext* ctx = view->getDocument()->dContext;
	    for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	    {
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
		    CGLCurve* crv = static_cast<CGLCurve*>(O);
		    crv->SetLineStyle(LINE_DASHED);
		}
	    }
	}
    }
}

void CadSurfApp::slotLineDotted()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    CGLDisplayContext* ctx = view->getDocument()->dContext;
	    for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	    {
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
		    CGLCurve* crv = static_cast<CGLCurve*>(O);
		    crv->SetLineStyle(LINE_DOTTED);
		}
	    }
	}
    }
}

void CadSurfApp::slotLineDotDashed()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    CGLDisplayContext* ctx = view->getDocument()->dContext;
	    for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	    {
		CGLObject* O = ctx->CurrentSelected();
		if(O->IsOfType(GLCURVE))
		{
		    CGLCurve* crv = static_cast<CGLCurve*>(O);
		    crv->SetLineStyle(LINE_DASHDOTTED);
		}
	    }
	}
    }
}

void CadSurfApp::slotWindowNewWindow()
{
    statusBar()->message(tr("Opening new document view..."));

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    createClient(doc);
	}
    }

    statusBar()->message(tr("Ready."));
}

void CadSurfApp::slotHelpAbout()
{
    QMessageBox::about(this,tr("About..."),
	    tr(" CadSurf Version " VERSION
		"\n##################################################################################\n\
		Author:	Sharjith Nair. Email:	sharjith_ssn@hotmail.com\n \
		Program:	OpenGL, C++ QT Surface Modeler Demo Application\n\
		Important Notice: \n\
		This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.\n\
		You can freely use it as long as you credit me for it. No guarantee/warantee\n\
		is given on this app and I will not be responsible for any damage	to you, your\n\
		property or any other person from using it.	USE IT ON YOUR OWN RISK.\n\
		Thankyou -\n\
		Sharjith Nair.\n\
#################################################################################\n\
		This Software Is Intended For Demonstrating The Use	OF OpenGL And Geometry For\n\
		Cad Type Applications. Almost All The Coding Is Done By me And The Respective\n\
		Names Of The Programmers Of Whom The Codes Are Used Are	Mentioned. Most Codes\n\
		Or Ideas That Are Not Programmed By me Are Taken From Sites Like www.codeguru.com,\n\
		www.codeproject.com, www.programmersheaven.com etc. I Sincierly Thank All Of Them.\n\
		Following Are Some Guidlines For Using The Software:\n\
		(1)Use Mouse Button 1 For Zooming With Control Key Pressed\n\
		(2)Use Mouse Button 2 For Panning With Control Key Pressed\n\
		(3)Use Mouse Button 3 For Rotating With Control Key Pressed\n\
		(5)Use Mouse Button 1 For Selection Without Any Key Pressed\n\
		(6)Use Mouse Button 1 For Multi Selection With Shift Key Pressed\n\
		(7)Press Mouse Button 1 And Drag For Sweep Selection Without Any Key Pressed\n\
		(8)Actions Like Window Zooming, Best Fit, Different Views, Display Modes In\n\
		Wireframe, Shading And Hidden Line Removed Mode Are Provided In The Child\n\
		Frame ToolBar\n\
		(9)The ToolBar In The MainFrame Has Buttons For Creating Geometry Implemented\n\
		Currently In Non Interactive Mode. You Can Freely Modify The Code To Make\n\
		Interactive Applications."));
}

void CadSurfApp::slotStatusHelpMsg(const QString &text)
{
    ///////////////////////////////////////////////////////////////////
    // change status message of whole statusbar temporary (text, msec)
    statusBar()->message(text, 2000);
}

void CadSurfApp::windowMenuAboutToShow()
{
    pWindowMenu->clear();	
    windowNewWindow->addTo(pWindowMenu);
    windowCascade->addTo(pWindowMenu);
    windowTile->addTo(pWindowMenu);

    if ( pWorkspace->windowList().isEmpty() )
    {
	windowAction->setEnabled(false);
    }
    else
    {
	windowAction->setEnabled(true);
    }

    pWindowMenu->insertSeparator();

    QWidgetList windows = pWorkspace->windowList();
    for ( int i = 0; i < int(windows.count()); ++i )
    {
	int id = pWindowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT( windowMenuActivated( int ) ) );
	pWindowMenu->setItemParameter( id, i );
	pWindowMenu->setItemChecked( id, pWorkspace->activeWindow() == windows.at(i) );
    }
}

void CadSurfApp::windowMenuActivated( int id )
{
    QWidget* w = pWorkspace->windowList().at( id );
    if ( w )
	w->setFocus();
}

//viewer slots
void CadSurfApp::slotTopView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->TopView();
	}
    }	
}

void CadSurfApp::slotBottomView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->BottomView();
	}
    }
}

void CadSurfApp::slotFrontView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->FrontView();
	}
    }
}

void CadSurfApp::slotBackView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->BackView();
	}
    }
}

void CadSurfApp::slotLeftView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->LeftView();
	}
    }
}

void CadSurfApp::slotRightView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->RightView();
	}
    }
}

void CadSurfApp::slotAxoView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->AxonView();
	}
    }
}

void CadSurfApp::slotFitall()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->FitAll();
	}
    }
}

void CadSurfApp::slotZoomwin()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->winZoom = true;
	}
    }
}

void CadSurfApp::slotReset()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->ResetView();
	}
    }
}

void CadSurfApp::slotDmode()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{    	
	    view->setFocus();
	    view->dShaded = ! view->dShaded;
	    if(!view->dShaded)
		view->GetView()->SetDisplayMode(GLSHADED);
	    else
		view->GetView()->SetDisplayMode(GLWIREFRAME);
	}
    }
}

void CadSurfApp::slotHLR()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->SetDisplayMode(GLHLREMOVED);
	}
    }
}

void CadSurfApp::slotAntial()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->antialiased = !view->antialiased;
	    if(!view->antialiased)
		view->GetView()->SetAntiAliasing(false);
	    else
		view->GetView()->SetAntiAliasing(true);
	}
    }
}

//object slots
void CadSurfApp::slotmkPoint()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnPoints();
	}
    }
}

void CadSurfApp::slotmkLine()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnLines();
	}
    }
}

void CadSurfApp::slotmkCircle()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnCircles();
	}
    }
}

void CadSurfApp::slotmkEllipse()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnEllipse();
	}
    }
}

void CadSurfApp::slotmkParabola()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnParabola();
	}
    }
}

void CadSurfApp::slotmkHyperbola()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnHyperbola();
	}
    }
}

void CadSurfApp::slotmkBezier()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnBezier();
	}
    }
}

void CadSurfApp::slotmkBSpline()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnBspline();
	}
    }
}

void CadSurfApp::slotmkPlane()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnPlane();
	}
    }
}

void CadSurfApp::slotmkCylinder()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnCylinder();
	}
    }
}

void CadSurfApp::slotmkCone()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnCone();
	}
    }
}

void CadSurfApp::slotmkSphere()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnSphere();
	}
    }
}

void CadSurfApp::slotmkTorus()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnTorus();
	}
    }
}

void CadSurfApp::slotmkExtruded()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnExtsurf();
	}
    }
}

void CadSurfApp::slotmkRevolved()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnRevsurf();
	}
    }
}

void CadSurfApp::slotmkRuled()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnRuledsurf();
	}
    }
}

void CadSurfApp::slotmkPipe()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnPipe();
	}
    }
}

void CadSurfApp::slotmkUserSurf()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnUserSurface();
	}
    }
}


void CadSurfApp::slotmkTranslate()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnTranslate();
	}
    }
}

void CadSurfApp::slotmkRotate()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnRotate();
	}
    }
}

void CadSurfApp::slotmkScale()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnScale();
	}
    }
}

void CadSurfApp::slotmkReflect()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	CadSurfView* view = (CadSurfView*)mw->centralWidget();
	if ( view )
	{
	    CadSurfDoc* doc = view->getDocument();
	    doc->OnReflect();
	}
    }
}

