/***************************************************************************
                          cadsurfview.cpp  -  description
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qmainwindow.h>
#include <qvbox.h>

// application specific includes
#include "CadSurfView.h"
#include "CadSurfDoc.h"
#include "CadSurf.h"

CadSurfView::CadSurfView(CadSurfDoc* pDoc, QWidget *parent, const char* name, int /*wflags*/)
 : QGLWidget(parent, name)
{
    doc=pDoc;
    myView = new QGLView(this, doc->GetDisplayContext());
    	ldown = mdown = rdown = false;
    	winZoom = false;
	dShaded = false;
	antialiased = false;
	gridOn = false;
	setMouseTracking(TRUE);
	
	dt = QDate::currentDate();
	QString stdow = dt.dayName(dt.dayOfWeek());
	int dy = dt.day();
	QString st(tr("  %1  ").arg(dy));
	dtStr = stdow + st;
	QString stdom = dt.monthName(dt.month());
	int yr = dt.year();
	st = tr("  %0.2d  ").arg(yr);
	dtStr = dtStr + stdom;
	dtStr = dtStr + st;
	
	startTimer( 0 );                            // run continuous timer
    QTimer * counter = new QTimer( this );
    connect( counter, SIGNAL(timeout()),
             this, SLOT(slotUpdateTime()) );
    counter->start( 1000 );
}

CadSurfView::~CadSurfView()
{
	delete myView;
}

CadSurfDoc *CadSurfView::getDocument() const
{
	return doc;
}

void CadSurfView::update(CadSurfView* pSender){
	if(pSender != this)
	{
		repaint();
		updateGL();
	}
}

void CadSurfView::print(QPrinter *pPrinter)
{
  if (pPrinter->setup(this))
  {
		QPainter p;
		p.begin(pPrinter);
		
		///////////////////////////////
		// TODO: add your printing code here
		///////////////////////////////
		
		p.end();
  }
}

void CadSurfView::closeEvent(QCloseEvent*)
{
  // LEAVE THIS EMPTY: THE EVENT FILTER IN THE CadSurfApp CLASS TAKES CARE FOR CLOSING
  // QWidget closeEvent must be prevented.
}

void CadSurfView::initializeGL()
{
	myView->InitGL();
}

void CadSurfView::resizeGL(int w, int h)
{
	myView->ReSize(w, h);
}

void CadSurfView::paintGL()
{
	myView->Draw();
}
	
void CadSurfView::resizeEvent( QResizeEvent* /*e*/)
{
	QRect aRect = geometry();
	resizeGL(aRect.width(), aRect.height());
}

void CadSurfView::mousePressEvent( QMouseEvent *e)
{
	if(e->button() & QMouseEvent::LeftButton)
	{
		// ask window to give us events even if outside the view
    		grabMouse(ArrowCursor);

		//  save the current mouse coordinate in min
		myXmin=e->x();  myYmin=e->y();
		myXmax=e->x();  myYmax=e->y();

		ldown = true;
		lPoint.setX(e->x());
		lPoint.setY(e->y());
		if(!(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
		{
			myView->Select(e->x(), e->y());
			
		}
		if ((e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
		{
			myView->MultiSelect(e->x(), e->y());
			
		}
	}
	if(e->button() & QMouseEvent::MidButton)
	{
		mdown = true;		
		mPoint.setX(e->x());
		mPoint.setY(e->y());
	}
	if(e->button() & QMouseEvent::RightButton)
	{
		rdown = true;
		rPoint.setX(e->x());
		rPoint.setY(e->y());
	}
}

void CadSurfView::mouseReleaseEvent( QMouseEvent *e)
{
	if(e->button() & QMouseEvent::LeftButton)
	{
		ldown = false;
		myView->StopZooming();
		
		if(!(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
		{
			DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
           	myXmax=e->x();
           	myYmax=e->y();
			QPoint point(e->x(), e->y());
			QRect arect(lPoint, point);
			arect.normalize();
			if(arect.width() && arect.height())
				myView->SweepSelect(arect);
			
		}

		if(winZoom)
		{
			DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
           	myXmax=e->x();
           	myYmax=e->y();
			QRect myZoomRect(myXmin, myYmax, abs(myXmax-myXmin), abs(myYmax-myYmin));
			if ((abs(myXmin-myXmax)>1) || (abs(myYmin-myYmax)>1))
			// Test if the zoom window size is not null
			{
				myView->ZoomWindow(myZoomRect);
			}
			
		}

		winZoom = false;
		releaseMouse();
	}
	if(e->button() & QMouseEvent::MidButton)
	{
		mdown = false;
		myView->StopPanning();
	}
	
	if(e->button() & QMouseEvent::RightButton)
	{
		myView->StopRotation();
		rdown = false;
	}
}

void CadSurfView::mouseMoveEvent( QMouseEvent *e)
{
	QPoint point(e->x(), e->y());
	
	CadSurfApp *top = (CadSurfApp*)topLevelWidget()->topLevelWidget();
	QString str;
	CPoint3D C;
	myView->ScreenToPoint(e->x(), e->y(), C);
	str = str.sprintf("X  %0.3lf  Y  %0.3lf  Z  %0.3lf",C.GetX(), C.GetY(), C.GetZ());
	top->cViewer->setText(str);
	
	if(ldown && !(e->state() & ControlButton) && !(e->state() & ShiftButton))
	{
		myXmax = point.x(); myYmax = point.y();
		DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);	
		DrawRectangle(myXmin,myYmin,myXmax,myYmax,true);
	}
	if(e->state() & ControlButton)
	{
	if(ldown)
	{
		myView->ZoomView(lPoint, point);		
	}
	else if(mdown)
	{
		myView->PanView(mPoint, point);		
	}
	else if(rdown)
	{
		myView->RotateView(rPoint, point);				
	}
	}
}

void CadSurfView::SetDisplayMode(const GLDisplayMode& aMode)
{
	myView->SetDisplayMode(aMode);
}

void CadSurfView::DrawRectangle (const int  MinX  ,
			        const int  MinY  ,
                    const int  MaxX  ,
	    	        const int  MaxY  ,
		            const bool  Draw  )
{
	QPainter thePainter(this);
	thePainter.setRasterOp(Qt::XorROP);
	thePainter.setPen(Qt::white);
	
    static		int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
    static		bool m_IsVisible;

	QRect aRect;
	if ( m_IsVisible && !Draw) // move or up  : erase at the old position
    {
	 aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
	 thePainter.drawRect(aRect);
	 m_IsVisible = FALSE;
    }

    StoredMinX = (MinX < MaxX) ? MinX: MaxX ;
    StoredMinY = (MinY < MaxY) ? MinY: MaxY ;
    StoredMaxX = (MinX > MaxX) ? MinX: MaxX ;
    StoredMaxY = (MinY > MaxY) ? MinY: MaxY ;

    if (Draw) // move : draw
    {
	 aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
	 thePainter.drawRect(aRect);
     m_IsVisible = TRUE;
   }
}

void CadSurfView::slotUpdateTime()
{
	tm = QTime::currentTime();
	CadSurfApp *top = (CadSurfApp*)topLevelWidget()->topLevelWidget();
	int hr, min, sec;
	hr = tm.hour();
	min = tm.minute();
	sec = tm.second();
	tmStr = tr("  %0.2d : %0.2d : %0.2d").arg(hr).arg(min).arg(sec);
	QString str = dtStr + tmStr;
	top->tViewer->setText(str);
	
}
