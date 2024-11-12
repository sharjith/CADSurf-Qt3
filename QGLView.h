/***************************************************************************
                          qglview.h  -  description
                             -------------------
    begin                : |15- Feb|
    copyright            : (C) |2003| by |Sharjith N.|
    email                : |sharjith_ssn@hormail.com|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGLVIEW_H
#define QGLVIEW_H

//Geometry
#include "Geometry.h"
#include "GeomException.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "OneAxis.h"
#include "AxisSystem.h"
#include "Line3D.h"
#include "Circle3D.h"
#include "Ellipse3D.h"
#include "Hyperbola3D.h"
#include "Parabola3D.h"
#include "BezierCurve.h"
#include "BSplineCurve.h"
#include "Plane.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Torus.h"
#include "ExtrudedSurface.h"
#include "RevolvedSurface.h"
#include "RuledSurface.h"
#include "PipeSurface.h"

//Graphics
#include "GLObject.h"
#include "GLOneAxis.h"
#include "GLAxisSystem.h"
#include "GLPoint.h"
#include "GLPlane.h"
#include "GLCurve.h"
#include "GLSurface.h"
#include "GLTrihedron.h"
#include "GLGrid.h"
#include "GLDisplayContext.h"
#include "GLCamera.h"

#include <qwidget.h>
#include <qrect.h>
#include <qpoint.h>
#include <qgl.h>
#include <GL/glut.h>

/**
  *@author Sharjith
  */

class CadSurfDoc;
class CGLDisplayContext;

class QGLView
{
public:
	QGLView(QGLWidget* w, CGLDisplayContext* ctx);
	virtual ~QGLView();
	void InitGL();
	void ReSize( int w, int h );
	void Draw();
private:
	bool ldown;
	bool mdown;
	bool rdown;
				
	QPoint  lPoint;
	QPoint  mPoint;
	QPoint  rPoint;

     /* Light values and coordinates*/
	GLfloat  ambientLight[4];
	GLfloat  diffuseLight[4];
	GLfloat  specular[4];
	GLfloat  specref[4];
     /*Viewing Volume Range*/
	
	int     myXmin;
	int     myYmin;
	int     myXmax;
	int     myYmax;

	GLdouble nRange;
	GLdouble xRot;
	GLdouble yRot;
	GLdouble zRot;
	GLdouble xTrans;
	GLdouble yTrans;
	GLdouble zTrans;
	GLfloat m_bkRed;
	GLfloat m_bkGreen;
	GLfloat m_bkBlue;
	GLDisplayMode displayMode;
	bool m_antialias;

	QPoint DownPoint;
	bool ZoomON;
	bool PanON;
	bool RotON;

	bool gridOn;
	
	QGLWidget* theWidget;
	CGLCamera* myCamera;
	CGLDisplayContext* theContext;
	CGLTrihedron* myTrihedron;
	CGLGrid* myGrid;	
// Operations
public:
	void RenderScene(GLenum mode);

	void ZoomView(QPoint, QPoint);
	void PanView(QPoint, QPoint);
	void RotateView(QPoint, QPoint);

	void StopZooming(){ ZoomON = false; }
	void StopPanning(){ PanON = false; }
	void StopRotation(){ RotON = false; }

	void TopView();
	void BottomView();
	void FrontView();
	void BackView();
	void LeftView();
	void RightView();
	void AxonView();

	void FitAll();
	void ZoomWindow(QRect zcRect);
	void ResetView();

	void PointToScreen(const CPoint3D& P, int& x, int& y);
	void ScreenToPoint(const int& x, const int& y, CPoint3D&  P);
	void FitView();
	void Fit3d(QRect zcRect);

	void SetBackgroundColor(GLfloat, GLfloat, GLfloat);
	void SetDisplayMode(const GLDisplayMode& aMode);
	GLDisplayMode GetDisplayMode() const { return displayMode; }

	void SetAntiAliasing(const bool& mode);

	void SetGridOn(const bool& mode);
	void SetGridPara(const CPlane& plane, const double& size, const double& step);

	void Select(const int& x, const int& y);
	void MultiSelect(const int& x, const int& y);
	void SweepSelect(const QRect& swRect);

	int  ProcessSelection(const int& x, const int& y, const int& sensitivity = 5);
	int  ProcessSweepSelection(const QRect& rect, unsigned long int* id);

};

#endif
