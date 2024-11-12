/***************************************************************************
                          QGLView.cpp  -  description
                             -------------------
    begin                : |15 - Feb|
    copyright            : (C) |2003| by |Sharjith N.|
    email                : |sharjith_ssn@hotmail.com|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "QGLView.h"
#include "MMath.h"

#include "CadSurfDoc.h"

QGLView::QGLView(QGLWidget* w, CGLDisplayContext* ctx) : theWidget(w), theContext(ctx)
{
	ldown = mdown = rdown = false;

	myCamera = new CGLCamera;
/* Rotation amounts*/
	xRot = -45.0f;
	zRot = -45.0f;
	yRot = 0;
	xTrans = yTrans = zTrans = 0.0f;

	nRange = 500;
	
	m_bkRed = 0;
	m_bkGreen = 0;
	m_bkBlue = 0;

	displayMode = GLSHADED;

	m_antialias = false;

	ZoomON = false;
	PanON = false;
	RotON = false;

	gridOn = false;

	myXmin=0;
    	myYmin=0;
   	myXmax=0;
    	myYmax=0;

	myTrihedron = new CGLTrihedron;
	myGrid = new CGLGrid;


	/* Light values and coordinates*/
	ambientLight[0] = 0.3f;
	ambientLight[1] = 0.3f;
	ambientLight[2] = 0.3f;
	ambientLight[3] = 1.0f;
	diffuseLight[0] = 0.7f;
	diffuseLight[1] = 0.7f;
	diffuseLight[2] = 0.7f;
	diffuseLight[3] = 1.0f;
	specular[0] = 1.0f;
	specular[1] = 1.0f;
	specular[2] = 1.0f;
	specular[3] = 1.0f;
	specref[0] =  1.0f;
	specref[1] =  1.0f;
	specref[2] =  1.0f;
	specref[3] =  1.0f;
}

QGLView::~QGLView()
{
	delete myTrihedron;
	delete myGrid;
	delete myCamera;
}

void QGLView::InitGL()
{
	// Set up the rendering context, define display lists etc.:
	/* Insert your OpenGL initialization code here */
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);


	// Set drawing color to
	glColor3ub(156, 128, 255);

}

void QGLView::ReSize( int w, int h )
{
	// setup viewport, projection etc.:
	/* Prevent a divide by zero*/
	if(h == 0)
		h = 1;

    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	if (w <= h)
		glOrtho (-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		(nRange*h/w)-yTrans, -(nRange*5000.0f)-zTrans, (nRange*5000.0f)-zTrans);
	else
		glOrtho (-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		nRange-yTrans, -nRange*5000.0f, nRange*5000.0f);
    glMatrixMode (GL_MODELVIEW);
	
	glLoadIdentity ();
}

void QGLView::Draw()
{
	// call RenderScene():
	RenderScene(GL_RENDER);
}

void QGLView::RenderScene(GLenum /*mode*/)
{
	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(m_antialias)
	{
		glEnable (GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	    glLineWidth (1.125);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable (GL_LINE_SMOOTH);
		glLineWidth (1);
		glDisable (GL_BLEND);
	}
    
	glPushMatrix();

	myCamera->Render();
	//View Rotations
	glRotatef(xRot,1,0,0);
	glRotatef(yRot,0,1,0);
	glRotatef(zRot,0,0,1);

	myTrihedron->SetSize(nRange/2);
	myTrihedron->Display();

	if(gridOn)
		myGrid->Display();

	//Do The Rendering
	theContext->Render(displayMode);
    //glutSolidTeapot(500);
	
	glPopMatrix();

	glFlush();
}

void QGLView::ZoomView(QPoint dPoint, QPoint mPoint)
{
	if(!ZoomON)
		DownPoint = dPoint;
	ZoomON = true;

	if(DownPoint.x() > mPoint.x() || DownPoint.y() < mPoint.y())
		nRange *= 1.05;
	else
		nRange /= 1.05;

	DownPoint = mPoint;

	int w = theWidget->width();
	int h = theWidget->height();

	ReSize(w, h);
	theWidget->updateGL();
}

void QGLView::PanView(QPoint dPoint, QPoint mPoint)
{
	if(!PanON)
		DownPoint = dPoint;
	PanON = true;
	QPoint pan = DownPoint - mPoint;
	DownPoint = mPoint;	
	
	myCamera->MoveAcross(0.01*nRange/2*pan.x());
	myCamera->Move(0.0, -0.01*nRange/2*pan.y(), 0.0);

	DownPoint = mPoint;

	int w = theWidget->width();
	int h = theWidget->height();

	ReSize(w, h);
	theWidget->updateGL();
}

void QGLView::RotateView(QPoint dPoint, QPoint mPoint)
{
	if(!RotON)
		DownPoint = dPoint;
	RotON = true;
	QPoint rotate = DownPoint - mPoint;
	//yRot -= rotate.x()/2;
	//xRot -= rotate.y()/2;
	myCamera->RotateX(rotate.y()/2);
	myCamera->RotateY(rotate.x()/2);
	DownPoint = mPoint;
	theWidget->updateGL();
}


void QGLView::TopView()
{
	// TODO: Add your command handler code here
	xRot = 0;
	yRot = 0;
	zRot = 0;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::BottomView()
{
	// TODO: Add your command handler code here
	xRot = -180;
	yRot = 0;
	zRot = 0;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::FrontView()
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = 0;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::BackView()
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = -180;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::LeftView()
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = -90;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::RightView()
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = 90;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::AxonView()
{
	// TODO: Add your command handler code here
	xRot = -45;
	yRot = 0;
	zRot = -45;
	myCamera->ResetAll();
	theWidget->updateGL();	
}

void QGLView::FitAll()
{
	// TODO: Add your command handler code here
	if(theContext->IsEmpty())
		return;
	FitView();

	QRect Rect = theWidget->geometry();
	int w = Rect.width();
	int h = Rect.height();

	ReSize( w, h);
	theWidget->updateGL();	
}

void QGLView::ZoomWindow(QRect zcRect)
{		
	QRect rcRect = theWidget->geometry();

	int rcWidth, rcHeight, zcWidth, zcHeight;
	QPoint rcCentre, zcCentre;
	int dx, dy, dw, dh;
	GLdouble wRatio, hRatio;
	GLdouble dRange;

	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;

	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;

	rcWidth = rcRect.width();
	rcHeight = rcRect.height();

	zcWidth = zcRect.width();
	zcHeight = zcRect.height();

	rcCentre = rcRect.center();
	zcCentre = zcRect.center();

	theWidget->makeCurrent();
	glPushMatrix();
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	realy=viewport[3]-(GLint)zcCentre.y()-1;
	gluUnProject((GLdouble)zcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	realy=viewport[3]-(GLint)rcCentre.y()-1;
	gluUnProject((GLdouble)rcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	dx = (int)(cx - mx);
	dy = (int)(cy - my);

	dw = rcWidth - zcWidth;
	dh = rcHeight - zcHeight;

	wRatio = (GLdouble) zcWidth/rcWidth;
	hRatio = (GLdouble) zcHeight/rcHeight;

	dRange = (wRatio < hRatio) ? hRatio : wRatio;

	nRange *= dRange/0.8;

	myCamera->MoveAcross(-dx);
	myCamera->Move(0.0, -dy, 0.0);

	ReSize(rcWidth, rcHeight);
	theWidget->updateGL();	
}

void QGLView::ResetView()
{
	// TODO: Add your command handler code here
	xRot = -45;
	yRot = 0;
	zRot = -45;
	xTrans = 0;
	yTrans = 0;
	zTrans = 0;
	nRange = 500;
	myCamera->ResetAll();

	QRect rect = theWidget->geometry();

	int w = rect.width();
	int h = rect.height();

	ReSize(w, h);
	theWidget->updateGL();
}

void QGLView::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_bkRed = r;
	m_bkGreen = g;
	m_bkBlue = b;
	theWidget->makeCurrent();
	glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0f);
	theWidget->updateGL();	
}

void QGLView::SetDisplayMode(const GLDisplayMode& aMode)
{
	displayMode = aMode;
	theWidget->updateGL();	
}

void QGLView::SetAntiAliasing(const bool& mode)
{
	m_antialias = mode;
	theWidget->updateGL();		
}

void QGLView::SetGridOn(const bool& mode)
{
	gridOn = mode;
	theWidget->updateGL();	
}

void QGLView::SetGridPara(const CPlane& plane, const double& size, const double& step)
{
	myGrid->SetPlane(plane);
	myGrid->SetSize(size);
	myGrid->SetStep(step);
	theWidget->updateGL();	
}

void QGLView::PointToScreen(const CPoint3D& P, int& x, int& y)
{
	CPoint3D p = P;
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLdouble wx, wy, wz;
	theWidget->makeCurrent();
	glPushMatrix();
	SF3DVector V = myCamera->GetPosition();
	glTranslatef(V.x, V.y, V.z);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	gluProject((GLdouble)p.GetX(), p.GetY(), p.GetZ(), mvmatrix, projmatrix, viewport,
				&wx, &wy, &wz);
	wx = sround(wx);
	wy = sround(wy);
	x = (int)wx; y = (int)wy;
	glPopMatrix();
}

void QGLView::ScreenToPoint(const int& x, const int& y, CPoint3D&  P)
{
	QPoint point(x,y);
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;
	GLdouble wx, wy, wz;
	theWidget->makeCurrent();
	glPushMatrix();
	SF3DVector V = myCamera->GetPosition();
	glTranslatef(-V.x, -V.y, -V.z);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	realy=viewport[3]-(GLint)point.y()-1;
	gluUnProject((GLdouble)point.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&wx, &wy, &wz);
	P.SetParam(wx, wy, wz);
	glPopMatrix();
}

void QGLView::FitView()
{
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;
	GLdouble lx, ly, lz, sx, sy, sz;
	CBoundingBox B = theContext->GetBoundingBox();
	lx = B.XMax(); ly = B.YMax(); lz = B.ZMax();
	sx = B.XMin(); sy = B.YMin(); sz = B.ZMin();

	theWidget->makeCurrent();
	glPushMatrix();

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	gluProject(lx, ly, lz, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	gluProject(sx, sy, sz, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	QPoint topr((int)cx,(int)cy), botl((int)mx,(int)my);
	QRect volRect(topr, botl);;
	volRect.normalize();

	Fit3d(volRect);
	theWidget->updateGL();
}

void QGLView::Fit3d(QRect rect)
{	
	QRect zcRect = rect;
	QRect rcRect = theWidget->geometry();

	int rcWidth, rcHeight, zcWidth, zcHeight;
	QPoint rcCentre, zcCentre;
	int dx, dy, dw, dh;
	GLdouble wRatio, hRatio;
	GLdouble dRange;

	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;

	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;

	rcWidth = rcRect.width();
	rcHeight = rcRect.height();

	zcWidth = zcRect.width();
	zcHeight = zcRect.height();

	rcCentre = rcRect.center();
	zcCentre = zcRect.center();

	theWidget->makeCurrent();

	glPushMatrix();

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	realy=viewport[3]-(GLint)zcCentre.y()-1;
	gluUnProject((GLdouble)zcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	realy=viewport[3]-(GLint)rcCentre.y()-1;
	gluUnProject((GLdouble)rcCentre.x(), (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	dx = (int)(cx - mx);
	dy = (int)(cy - my);

	dw = rcWidth - zcWidth;
	dh = rcHeight - zcHeight;

	wRatio = (GLdouble) zcWidth/rcWidth;
	hRatio = (GLdouble) zcHeight/rcHeight;

	dRange = (wRatio < hRatio) ? hRatio : wRatio;

	nRange *= dRange/(double)0.8;

	myCamera->SetPosition(0,0,0);
	xTrans += dx;
	yTrans -= dy;
	
	theWidget->updateGL();	
}

void QGLView::Select(const int &x, const int &y)
{
	theContext->Select(this, x, y);
	theWidget->updateGL();	
}

void QGLView::MultiSelect(const int &x, const int &y)
{
	theContext->MultiSelect(this, x, y);
	theWidget->updateGL();		
}

void QGLView::SweepSelect(const QRect& swRect)
{
	theContext->SweepSelect(this, swRect);
	theWidget->updateGL();		
}

int QGLView::ProcessSelection(const int& xPos, const int& yPos , const int& sensitivity)
{
	int id = 0;

	theWidget->makeCurrent();

	// Space for selection buffer
	GLuint selectBuff[512];

	// Hit counter and viewport storeage
	GLint hits, viewport[4];

	// Setup selection buffer
	glSelectBuffer(512, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending n pixels
	// in the vertical and horzontal direction
	int n = sensitivity;
	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3] - yPos, n, n, viewport);

	QRect rect = theWidget->geometry();

	int w = rect.width();
	int h = rect.height();

	// Ortho Viewing
    if (w <= h)
		glOrtho (-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		(nRange*h/w)-yTrans, -(nRange*5000.0f)-zTrans, (nRange*5000.0f)-zTrans);
	else
		glOrtho (-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		nRange-yTrans, -nRange*5000.0f, nRange*5000.0f);

	// Draw the scene
	RenderScene(GL_SELECT);

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

	// If a single hit occured, display the info.
	if(hits)
	{
		id =  selectBuff[3];		
	}

	return id;

}

int QGLView::ProcessSweepSelection(const QRect& swRect, unsigned long int* id)
{
	GLint i;
	GLuint j;
	theWidget->makeCurrent();

	// Space for selection buffer
	GLuint selectBuff[512];

	// Hit counter and viewport storeage
	GLint hits = 0, viewport[4];

	// Setup selection buffer
	glSelectBuffer(512, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be cube around
	// center point of sweeping rectangle and extending pixels
	// falling within the height and width of the sweep rectangle
	// in the vertical and horzontal direction
	glLoadIdentity();
	gluPickMatrix(swRect.center().x(), viewport[3] - swRect.center().y(), swRect.width(), swRect.height(), viewport);

	QRect trect = theWidget->geometry();

	int w = trect.width();
	int h = trect.height();

	// Ortho Viewing
    if (w <= h)
		glOrtho (-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		(nRange*h/w)-yTrans, -(nRange*5000.0f)-zTrans, (nRange*5000.0f)-zTrans);
	else
		glOrtho (-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		nRange-yTrans, -nRange*5000.0f, nRange*5000.0f);

	// Draw the scene
	RenderScene(GL_SELECT);

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

	// If hits occured, display the info.
	if(hits)
	{
		GLuint *ptr, names;
		ptr = (GLuint*) selectBuff;
		for(i=0; i < hits; i++)
		{
			names = *ptr;
			ptr++; ptr++; ptr++;
			for(j = 0; j < names; j++)
			{
				id[i] = *ptr;
				ptr++;
			}
		}
	}

	return hits;
}

