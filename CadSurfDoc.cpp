/***************************************************************************
  cadsurfdoc.cpp  -  description
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
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qmsgbox.h>
#include <qfiledialog.h>


// application specific includes
#include "CadSurfDoc.h"
#include "CadSurf.h"
#include "CadSurfView.h"   

#include "GLFont.h"


CadSurfDoc::CadSurfDoc()
{
    pViewList = new QList<CadSurfView>;
    pViewList->setAutoDelete(false);
    dContext = new CGLDisplayContext(this);	
    /*
       cout<<"\nSize Of int On Your Machine Is "<<sizeof(int)<<" bytes"<<endl;
       cout<<"Size of long int "<<sizeof(long int)<<endl;
       cout<<"Size of float "<<sizeof(float)<<endl;
       cout<<"Size of double "<<sizeof(double)<<endl;
       cout<<"Size of long double "<<sizeof(long double)<<endl;
       cout<<"Size of Point3D "<<sizeof(CPoint3D)<<endl;
       cout<<"Size of Vector3D "<<sizeof(CVector3D)<<endl;
       cout<<"Size of OneAxis "<<sizeof(COneAxis)<<endl;
       cout<<"Size of AxisSystem "<<sizeof(CAxisSystem)<<endl;
       cout<<"Size of Cylinder "<<sizeof(CCylinder)<<endl;
       cout<<"Size of Cone "<<sizeof(CCone)<<endl;
       cout<<"Size of Sphere "<<sizeof(CSphere)<<endl;
       cout<<"Size Of Torus "<<sizeof(CTorus)<<" bytes"<<endl;
       cout<<"Size of Ext Surface "<<sizeof(CExtrudedSurface)<<endl;
       cout<<"Size of Rev Surface "<<sizeof(CRevolvedSurface)<<endl;
       cout<<"Size of Rul Surface "<<sizeof(CRuledSurface)<<endl;
       cout<<"Size of Pipe Surface "<<sizeof(CPipeSurface)<<endl;
       */	
}

CadSurfDoc::~CadSurfDoc()
{
    delete pViewList;
    delete dContext;
}

void CadSurfDoc::addView(CadSurfView *view)
{
    pViewList->append(view);
    changedViewList();
}

void CadSurfDoc::removeView(CadSurfView *view)
{
    pViewList->remove(view);
    if(!pViewList->isEmpty())
	changedViewList();
    else
	deleteContents();
}

void CadSurfDoc::changedViewList(){	

    CadSurfView *w;
    if((int)pViewList->count() == 1){
	w=pViewList->first();
	w->setCaption(m_title);
    }
    else{
	int i;
	for( i=1,w=pViewList->first(); w!=0; i++, w=pViewList->next())
	    w->setCaption(QString(m_title+":%1").arg(i));	
    }
}

bool CadSurfDoc::isLastView() {
    return ((int) pViewList->count() == 1);
}


void CadSurfDoc::updateAllViews(CadSurfView *sender)
{
    CadSurfView *w;
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
	w->update(sender);
    }

}

void CadSurfDoc::update()
{
    CadSurfView *w;
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
	w->update(0);
    }

}

void CadSurfDoc::setPathName(const QString &name)
{
    m_filename=name;
    m_title=QFileInfo(name).fileName();
}

const QString& CadSurfDoc::pathName() const
{
    return m_filename;
}

void CadSurfDoc::setTitle(const QString &title)
{
    m_title=title;
}

const QString &CadSurfDoc::title() const
{
    return m_title;
}


void CadSurfDoc::closeDocument()
{
    CadSurfView *w;
    if(!isLastView())
    {
	for(w=pViewList->first(); w!=0; w=pViewList->next())
	{
	    if(!w->close())
		break;
	}
    }
    if(isLastView())
    {
	w=pViewList->first();
	w->close();
    }
}

bool CadSurfDoc::newDocument()
{
    /////////////////////////////////////////////////
    // TODO: Add your document initialization code here
    /////////////////////////////////////////////////
    modified=false;
    return true;
}

bool CadSurfDoc::openDocument(const QString &filename, const char * /* format =0*/)
{

    QFile f( filename );
    if ( !f.open( IO_ReadOnly ) )
	return false;
    /////////////////////////////////////////////////
    // TODO: Add your document opening code here
    /////////////////////////////////////////////////
    f.close();

    modified=false;
    m_filename=filename;
    m_title=QFileInfo(f).fileName();
    return true;
}

bool CadSurfDoc::saveDocument(const QString &filename, const char* /*format =0*/)
{
    QFile f( filename );
    if ( !f.open( IO_WriteOnly ) )
	return false;

    /////////////////////////////////////////////////
    // TODO: Add your document saving code here
    /////////////////////////////////////////////////

    f.close();

    modified=false;
    m_filename=filename;
    m_title=QFileInfo(f).fileName();
    return true;
}

void CadSurfDoc::deleteContents()
{
    /////////////////////////////////////////////////
    // TODO: Add implementation to delete the document contents
    /////////////////////////////////////////////////

}

bool CadSurfDoc::canCloseFrame(CadSurfView* pFrame)
{
    if(!isLastView())
	return true;

    bool ret=false;
    if(isModified())
    {
	QString saveName;
	switch(QMessageBox::information(pFrame, title(), tr("The current file has been modified.\n"
			"Do you want to save it?"),QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel ))
	{
	    case QMessageBox::Yes:
		if(title().contains(tr("Untitled")))
		{
		    saveName=QFileDialog::getSaveFileName(0, 0, pFrame);
		    if(saveName.isEmpty())
			return false;
		}
		else
		    saveName=pathName();

		if(!saveDocument(saveName))
		{
		    switch(QMessageBox::critical(pFrame, tr("I/O Error !"), tr("Could not save the current document !\n"
				    "Close anyway ?"),QMessageBox::Yes ,QMessageBox::No))

		    {
			case QMessageBox::Yes:
			    ret=true;
			case QMessageBox::No:
			    ret=false;
		    }
		}
		else
		    ret=true;
		break;
	    case QMessageBox::No:
		ret=true;
		break;
	    case QMessageBox::Cancel:
	    default:
		ret=false;
		break;
	}
    }
    else
	ret=true;

    return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CadSurfDoc commands

void CadSurfDoc::OnPoints()
{

    dContext->DeleteAll();


    CPoint3D P1(100,100,100);
    CPoint3D P3(-200,200,100);
    CPoint3D P2(200,-200,-100);

    CGLPoint *gP1 = new CGLPoint(P1);
    CGLPoint *gP2 = new CGLPoint(P2);
    CGLPoint *gP3 = new CGLPoint(P3);

    dContext->Display(gP1);
    dContext->Display(gP2);
    dContext->Display(gP3);

    QString str;

    str.sprintf("P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
    CGLFont *myFont1 = new CGLFont(str, P1+1);
    str.sprintf("P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P2+1);
    str.sprintf("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
    CGLFont *myFont3 = new CGLFont(str, P3+1);

    dContext->Display(myFont1);
    dContext->Display(myFont2);
    dContext->Display(myFont3);

    delete gP1;
    delete gP2;
    delete gP3;

    delete myFont1;
    delete myFont2;
    delete myFont3;
}

void CadSurfDoc::OnLines()
{

    dContext->DeleteAll();


    CPoint3D P1(-100, 100, -100);
    CPoint3D P2(100, 300, 100);
    CLine3D L1(P1, P2);
    CVector3D V(1,0,1);

    CPoint3D P3(100, -200, 100);
    CLine3D L2(P3, V);

    CGLCurve* lin1 = new CGLCurve(&L1);
    CGLCurve* lin2 = new CGLCurve(&L2);

    dContext->Display(lin1);
    dContext->Display(lin2);	

    CGLPoint* gP1 = new CGLPoint(P1);
    CGLPoint* gP2 = new CGLPoint(P2);
    CGLPoint* gP3 = new CGLPoint(P3);

    dContext->Display(gP1);
    dContext->Display(gP2);
    dContext->Display(gP3);

    QString str;

    str.sprintf("From P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
    CGLFont *myFont1 = new CGLFont(str, P1);
    str.sprintf("To P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P2);
    P1.Translate(-100,-100,-100);
    CVector3D D = L2.Direction();
    str.sprintf("Infinite Line Thru P3 Along Direction(%0.3lf,%0.3lf,%0.3lf)",D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont3 = new CGLFont(str, L2.PointAtPara(0.499));
    str.sprintf("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
    CGLFont *myFont4 = new CGLFont(str, P3);

    dContext->Display(myFont1);
    dContext->Display(myFont2);
    dContext->Display(myFont3);
    dContext->Display(myFont4);

    delete lin1;
    delete lin2;

    delete gP1;
    delete gP2;
    delete gP3;

    delete myFont1;
    delete myFont2;
    delete myFont3;
    delete myFont4;
}

void CadSurfDoc::OnCircles()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double Rad = 100;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CCircle3D C(P, Rad);
    CGLCurve* gC = new CGLCurve(&C);
    dContext->Display(gC);
    QString str;
    str.sprintf("Circle Of Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetRadius(),P.GetX(), P.GetY(), P.GetZ());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CPoint3D P1(-200, 100, 0);
    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);
    CAxisSystem Ax(P1, CVector3D(1,0,1));
    CCircle3D C1(Ax, Rad);
    CGLCurve* gC1 = new CGLCurve(&C1);
    dContext->Display(gC1);

    str.sprintf("Radius %0.3lf ",C1.GetRadius());
    CPoint3D R = P1;
    R.Translate(R, C1.PointAtPara(0));
    CGLFont *myFont1 = new CGLFont(str, R);
    dContext->Display(myFont1);
    CVector3D D = Ax.GetDirection();
    str.sprintf("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P1);
    dContext->Display(myFont2);

    CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
    dContext->Display(gAx);

    delete gP;
    delete gC;
    delete gP1;
    delete gC1;
    delete gAx;

    delete myFont;
    delete myFont1;
    delete myFont2;
}

void CadSurfDoc::OnEllipse()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double maj = 100, min = 50;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CEllipse3D C(P, maj, min);
    CGLCurve* gC = new CGLCurve(&C);
    dContext->Display(gC);

    QString str;
    str.sprintf("Ellipse Of Maj Radius %0.3lf And Min Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetMajorRadius(), C.GetMinorRadius(),P.GetX(), P.GetY(), P.GetZ());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CPoint3D P1(-200, 100, 0);
    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);

    CAxisSystem Ax(P1, CVector3D(1,1,0));
    CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
    dContext->Display(gAx);

    CEllipse3D C1(Ax, maj, min);
    CGLCurve* gC1 = new CGLCurve(&C1);
    dContext->Display(gC1);

    str.sprintf("Maj Radius %0.3lf",C1.GetMajorRadius());
    CPoint3D R1 = P1;
    R1.Translate(R1, C1.PointAtPara(0));
    CGLFont *myFont1 = new CGLFont(str, R1);
    dContext->Display(myFont1);
    str.sprintf("Min Radius %0.3lf ",C1.GetMinorRadius());
    CPoint3D R2 = P1;
    R2.Translate(R2, C1.PointAtPara(PI/2));
    CGLFont *myFont2 = new CGLFont(str, R2);
    dContext->Display(myFont2);
    CVector3D D = Ax.GetDirection();
    str.sprintf("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont3 = new CGLFont(str, P1);
    dContext->Display(myFont3);

    delete gP;
    delete gC;
    delete gP1;
    delete gC1;
    delete gAx;

    delete myFont;
    delete myFont1;
    delete myFont2;
    delete myFont3;
}

void CadSurfDoc::OnParabola()
{

    dContext->DeleteAll();


    CAxisSystem Ax(CPoint3D(200,0,0), CVector3D(0,0,1), CVector3D(1,0,0));
    CParabola3D P(Ax, 50);
    P.SetYRange(-10, 10);

    CAxisSystem Ax1(CPoint3D(-100,0,0), CVector3D(0,0,1), CVector3D(-1,0,0));
    CParabola3D P1(Ax1, 60);
    P1.SetYRange(-10, 10);

    CGLCurve* gC = new CGLCurve(&P);
    dContext->Display(gC);

    CGLCurve* gC1 = new CGLCurve(&P1);
    dContext->Display(gC1);

    QString str;
    str.sprintf("Parabola Of Focal Length %0.3lf",P.GetFocalLength());
    CGLFont *myFont1 = new CGLFont(str, CPoint3D(200,0,0));
    dContext->Display(myFont1);

    str.sprintf("Parabola Of Focal Length %0.3lf",P1.GetFocalLength());
    CGLFont *myFont2 = new CGLFont(str, P1.GetLocation().GetPosition());
    dContext->Display(myFont2);

    delete gC;
    delete gC1;

    delete myFont1;
    delete myFont2;
}

void CadSurfDoc::OnHyperbola()
{

    dContext->DeleteAll();


    CAxisSystem Ax(CPoint3D(0,0,0), CVector3D(0,0,1), CVector3D(1,0,0));
    CHyperbola3D H(Ax, 100, 50);

    CHyperbola3D H1 = H.GetConjugateBranch1();
    CHyperbola3D H2 = H.GetConjugateBranch2();
    CHyperbola3D H3 = H.GetOtherBranch();

    CGLCurve* gC = new CGLCurve(&H);
    dContext->Display(gC);

    CGLCurve* gC1 = new CGLCurve(&H1);
    dContext->Display(gC1);

    CGLCurve* gC2 = new CGLCurve(&H2);
    dContext->Display(gC2);

    CGLCurve* gC3 = new CGLCurve(&H3);
    dContext->Display(gC3);

    QString str;
    CPoint3D P = H1.GetLocation().GetPosition();
    P.Translate(0,-20,0);
    str.sprintf("Hyperbola Of Maj Radius %0.3lf And Min Radius %0.3lf",H.GetMajorRadius(), H.GetMinorRadius());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    str.sprintf("First Conjugate Branch");
    CPoint3D P1 = H1.GetLocation().GetPosition();
    P1.Translate(0,200,0);
    CGLFont *myFont1 = new CGLFont(str, P1);
    dContext->Display(myFont1);

    str.sprintf("Second Conjugate Branch");
    CPoint3D P2 = H2.GetLocation().GetPosition();
    P2.Translate(0,-200,0);
    CGLFont *myFont2 = new CGLFont(str, P2);
    dContext->Display(myFont2);

    str.sprintf("Other Branch");
    CPoint3D P3 = H3.GetLocation().GetPosition();
    P3.Translate(-300,0,0);
    CGLFont *myFont3 = new CGLFont(str, P3);
    dContext->Display(myFont3);

    delete gC;
    delete gC1;
    delete gC2;
    delete gC3;

    delete myFont;
    delete myFont1;
    delete myFont2;
    delete myFont3;
}

void CadSurfDoc::OnBezier()
{

    dContext->DeleteAll();


    CListOfCPoint3D *myCPList = new CListOfCPoint3D;
    CPoint3D P1(-450,-200,-00);
    CPoint3D P2(-350,300,-00);
    CPoint3D P3(-200,-300,-00);
    CPoint3D P4(-200,400,-00);
    CPoint3D P5(200,-300,-00);
    CPoint3D P6(300,400,-00);
    CPoint3D P7(350,-200,-00);
    CPoint3D P8(400,300,-00);
    myCPList->Append(P1);
    myCPList->Append(P2);
    myCPList->Append(P3);
    myCPList->Append(P4);
    myCPList->Append(P5);
    myCPList->Append(P6);
    myCPList->Append(P7);
    myCPList->Append(P8);

    CGLPoint *gP1 = new CGLPoint(P1);
    CGLPoint *gP2 = new CGLPoint(P2);
    CGLPoint *gP3 = new CGLPoint(P3);
    CGLPoint *gP4 = new CGLPoint(P4);
    CGLPoint *gP5 = new CGLPoint(P5);
    CGLPoint *gP6 = new CGLPoint(P6);
    CGLPoint *gP7 = new CGLPoint(P7);
    CGLPoint *gP8 = new CGLPoint(P8);

    dContext->Display(gP1);
    dContext->Display(gP2);
    dContext->Display(gP3);
    dContext->Display(gP4);
    dContext->Display(gP5);
    dContext->Display(gP6);
    dContext->Display(gP7);
    dContext->Display(gP8);

    CBezierCurve B(myCPList);
    CGLCurve* gC = new CGLCurve(&B);
    dContext->Display(gC);

    QString str;
    str.sprintf("P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
    CGLFont *myFont1 = new CGLFont(str, P1);
    str.sprintf("P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P2);
    str.sprintf("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
    CGLFont *myFont3 = new CGLFont(str, P3);
    str.sprintf("P4(%0.3lf,%0.3lf,%0.3lf)",P4.GetX(), P4.GetY(), P4.GetZ());
    CGLFont *myFont4 = new CGLFont(str, P4);
    str.sprintf("P5(%0.3lf,%0.3lf,%0.3lf)",P5.GetX(), P5.GetY(), P5.GetZ());
    CGLFont *myFont5 = new CGLFont(str, P5);
    str.sprintf("P6(%0.3lf,%0.3lf,%0.3lf)",P6.GetX(), P6.GetY(), P6.GetZ());
    CGLFont *myFont6 = new CGLFont(str, P6);
    str.sprintf("P7(%0.3lf,%0.3lf,%0.3lf)",P7.GetX(), P7.GetY(), P7.GetZ());
    CGLFont *myFont7 = new CGLFont(str, P7);
    str.sprintf("P8(%0.3lf,%0.3lf,%0.3lf)",P8.GetX(), P8.GetY(), P8.GetZ());
    CGLFont *myFont8 = new CGLFont(str, P8);

    dContext->Display(myFont1);
    dContext->Display(myFont2);
    dContext->Display(myFont3);
    dContext->Display(myFont4);
    dContext->Display(myFont5);
    dContext->Display(myFont6);
    dContext->Display(myFont7);
    dContext->Display(myFont8);

    myCPList->Clear();
    delete myCPList;

    delete gP1;	delete gP2;	delete gP3;	delete gP4;
    delete gP5;	delete gP6;	delete gP7;	delete gP8;

    delete gC;

    delete myFont1;	delete myFont2;	delete myFont3; delete myFont4;
    delete myFont5;	delete myFont6;	delete myFont7; delete myFont8;

}

void CadSurfDoc::OnBspline()
{

    dContext->DeleteAll();


    CListOfCPoint3D *myCPList = new CListOfCPoint3D;
    CPoint3D P1(-450,-200,-00);
    CPoint3D P2(-350,300,-00);
    CPoint3D P3(-200,-300,-00);
    CPoint3D P4(-200,400,-00);
    CPoint3D P5(200,-300,-00);
    CPoint3D P6(300,400,-00);
    CPoint3D P7(350,-200,-00);
    CPoint3D P8(400,300,-00);
    myCPList->Append(P1);
    myCPList->Append(P2);
    myCPList->Append(P3);
    myCPList->Append(P4);
    myCPList->Append(P5);
    myCPList->Append(P6);
    myCPList->Append(P7);
    myCPList->Append(P8);

    CGLPoint *gP1 = new CGLPoint(P1);
    CGLPoint *gP2 = new CGLPoint(P2);
    CGLPoint *gP3 = new CGLPoint(P3);
    CGLPoint *gP4 = new CGLPoint(P4);
    CGLPoint *gP5 = new CGLPoint(P5);
    CGLPoint *gP6 = new CGLPoint(P6);
    CGLPoint *gP7 = new CGLPoint(P7);
    CGLPoint *gP8 = new CGLPoint(P8);

    dContext->Display(gP1);
    dContext->Display(gP2);
    dContext->Display(gP3);
    dContext->Display(gP4);
    dContext->Display(gP5);
    dContext->Display(gP6);
    dContext->Display(gP7);
    dContext->Display(gP8);

    CBSplineCurve B(myCPList);
    CGLCurve* gC = new CGLCurve(&B);
    dContext->Display(gC);

    QString str;
    str.sprintf("P1(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ());
    CGLFont *myFont1 = new CGLFont(str, P1);
    str.sprintf("P2(%0.3lf,%0.3lf,%0.3lf)",P2.GetX(), P2.GetY(), P2.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P2);
    str.sprintf("P3(%0.3lf,%0.3lf,%0.3lf)",P3.GetX(), P3.GetY(), P3.GetZ());
    CGLFont *myFont3 = new CGLFont(str, P3);
    str.sprintf("P4(%0.3lf,%0.3lf,%0.3lf)",P4.GetX(), P4.GetY(), P4.GetZ());
    CGLFont *myFont4 = new CGLFont(str, P4);
    str.sprintf("P5(%0.3lf,%0.3lf,%0.3lf)",P5.GetX(), P5.GetY(), P5.GetZ());
    CGLFont *myFont5 = new CGLFont(str, P5);
    str.sprintf("P6(%0.3lf,%0.3lf,%0.3lf)",P6.GetX(), P6.GetY(), P6.GetZ());
    CGLFont *myFont6 = new CGLFont(str, P6);
    str.sprintf("P7(%0.3lf,%0.3lf,%0.3lf)",P7.GetX(), P7.GetY(), P7.GetZ());
    CGLFont *myFont7 = new CGLFont(str, P7);
    str.sprintf("P8(%0.3lf,%0.3lf,%0.3lf)",P8.GetX(), P8.GetY(), P8.GetZ());
    CGLFont *myFont8 = new CGLFont(str, P8);

    dContext->Display(myFont1);
    dContext->Display(myFont2);
    dContext->Display(myFont3);
    dContext->Display(myFont4);
    dContext->Display(myFont5);
    dContext->Display(myFont6);
    dContext->Display(myFont7);
    dContext->Display(myFont8);

    myCPList->Clear();
    delete myCPList;

    delete gP1;	delete gP2;	delete gP3;	delete gP4;
    delete gP5;	delete gP6;	delete gP7;	delete gP8;

    delete gC;

    delete myFont1;	delete myFont2;	delete myFont3; delete myFont4;
    delete myFont5;	delete myFont6;	delete myFont7; delete myFont8;
}

void CadSurfDoc::OnPlane()
{

    dContext->DeleteAll();


    CPlane Pln1(1,0,1,-200);
    CGLPlane* gPln1 = new CGLPlane(Pln1);
    gPln1->SetMaterial(BRONZE);
    dContext->Display(gPln1);

    CPlane Pln2 = CPlane::ZOX();
    CGLPlane* gPln2 = new CGLPlane(Pln2);
    dContext->Display(gPln2);

    CPoint3D P1(-300, -100, 50), P2(20, 200, -100), P3(200, 200, 100);
    CGLPoint *gP1 = new CGLPoint(P1);
    CGLPoint *gP2 = new CGLPoint(P2);
    CGLPoint *gP3 = new CGLPoint(P3);
    dContext->Display(gP1);
    dContext->Display(gP2);
    dContext->Display(gP3);

    CPlane Pln3(P1, P2, P3);
    CGLPlane* gPln3 = new CGLPlane(Pln3, 1200, 800);
    gPln3->SetMaterial(PEWTER);
    dContext->Display(gPln3);

    CPoint3D P5(-1000, 0, -500); CVector3D X(1,0,0), Y(0,1,1);
    CPlane Pln4(P5, X, Y);
    CGLPlane* gPln4 = new CGLPlane(Pln4);
    gPln4->SetMaterial(GOLD);
    dContext->Display(gPln4);

    delete gPln1; delete gPln2; delete gPln3; delete gPln4;
    delete gP1; delete gP2; delete gP3;
}

void CadSurfDoc::OnCylinder()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double Rad = 100, Ht = 200;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CAxisSystem Ax1(CPoint3D(200,0,0), CVector3D(0,0,1));
    CCylinder C(Ax1, Ht, Rad, 0, PI);
    CGLSurface* gS = new CGLSurface(&C);
    dContext->Display(gS);

    QString str;
    str.sprintf("Cylinder Of Radius %0.3lf And Height %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetRadius(), C.GetHeight(), P.GetX(), P.GetY(), P.GetZ());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CPoint3D P1(-300, 100, 0);
    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);
    CAxisSystem Ax(P1, CVector3D(1,0,1));
    CCircle3D C1(Ax, Rad);
    CCylinder Cyl(C1, Ht);
    CGLCurve* gC1 = new CGLCurve(&C1);
    CGLSurface* gS1 = new CGLSurface(&Cyl);
    dContext->Display(gC1);
    dContext->Display(gS1);

    str.sprintf("Base Circle Of Radius %0.3lf ",C1.GetRadius());
    CPoint3D R = P1;
    R.Translate(R, C1.PointAtPara(0));
    CGLFont *myFont1 = new CGLFont(str, R);
    dContext->Display(myFont1);
    CVector3D D = Ax.GetDirection();
    str.sprintf("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P1);
    dContext->Display(myFont2);

    CPoint3D HP = P1;
    COneAxis ax(HP, D);
    HP.Translate(ax, Ht);
    str.sprintf("Height %0.3lf", Cyl.GetHeight());
    CGLFont *myFont3 = new CGLFont(str, HP);
    dContext->Display(myFont3);

    CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
    dContext->Display(gAx);

    delete myFont; delete myFont1;	delete myFont2;	delete myFont3;
    delete gP; delete gS; delete gP1; delete gC1; delete gS1; delete gAx;
}

void CadSurfDoc::OnCone()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double Rad = 100, Ht = 200, tRad = 50;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CAxisSystem Ax1(CPoint3D(200,0,0), CVector3D(0,0,1));
    CCone C(Ax1, Ht, Rad);
    CGLSurface* gS = new CGLSurface(&C);
    dContext->Display(gS);

    QString str;
    str.sprintf("Cone Of Radius %0.3lf And Height %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C.GetBaseRadius(), C.GetHeight(), P.GetX(), P.GetY(), P.GetZ());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CPoint3D P1(-200, -300, 0);
    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);
    CAxisSystem Ax(P1, CVector3D(0,-1,1));
    CCircle3D C1(Ax, Rad);
    CCone Con(C1, Ht);
    CGLCurve* gC1 = new CGLCurve(&C1);
    CGLSurface* gS1 = new CGLSurface(&Con);
    dContext->Display(gC1);
    dContext->Display(gS1);

    str.sprintf("Base Circle Of Radius %0.3lf ",C1.GetRadius());
    CPoint3D R = P1;
    R.Translate(R, C1.PointAtPara(0));
    CGLFont *myFont1 = new CGLFont(str, R);
    dContext->Display(myFont1);
    CVector3D D = Ax.GetDirection();
    str.sprintf("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P1);
    dContext->Display(myFont2);

    CPoint3D HP = P1;
    COneAxis ax(HP, D);
    HP.Translate(ax, Ht);
    str.sprintf("Height %0.3lf", Con.GetHeight());
    CGLFont *myFont3 = new CGLFont(str, HP);
    dContext->Display(myFont3);

    CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
    dContext->Display(gAx);

    CPoint3D P2(-200, 0, 0);
    CAxisSystem Ax2(P2, CVector3D(0,0,1));
    CCone C3(Ax2, Ht, Rad, tRad, PI/2, PI);
    CGLSurface* gS2 = new CGLSurface(&C3);
    dContext->Display(gS2);

    str.sprintf("Cone Of Base Radius %0.3lf, Top Radius %0.3lf And Height %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",C3.GetBaseRadius(), C3.GetTopRadius(), C3.GetHeight(), P2.GetX(), P2.GetY(), P2.GetZ());
    CGLFont *myFont4 = new CGLFont(str, P2);
    dContext->Display(myFont4);

    delete myFont; delete myFont1;	
    delete myFont2; delete myFont3; delete myFont4;
    delete gP; delete gS; delete gP1; delete gC1; 
    delete gS1; delete gS2; delete gAx;
}

void CadSurfDoc::OnSphere()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double Rad = 100;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CAxisSystem Ax1(P, CVector3D(0,0,1));
    CSphere S(Ax1, Rad, 0, 2*PI, -PI/2, 0);
    CGLSurface* gS = new CGLSurface(&S);
    dContext->Display(gS);

    QString str;
    str.sprintf("Sphere Of Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",S.GetRadius(),P.GetX(), P.GetY(), P.GetZ());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CPoint3D P1(-200, 100, 0);
    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);
    CAxisSystem Ax(P1, CVector3D(1,0,1));
    CSphere S1(Ax, Rad);
    CGLSurface* gS1 = new CGLSurface(&S1);
    dContext->Display(gS1);

    str.sprintf("Radius %0.3lf ",S1.GetRadius());
    CPoint3D R = P1;
    R.Translate(R, S1.PointAtPara(0, 0));
    CGLFont *myFont1 = new CGLFont(str, R);
    dContext->Display(myFont1);
    CVector3D D = Ax.GetDirection();
    str.sprintf("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont2 = new CGLFont(str, P1);
    dContext->Display(myFont2);

    CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
    dContext->Display(gAx);

    delete myFont; delete myFont1;	delete myFont2;
    delete gP; delete gS; delete gP1; delete gS1; delete gAx;

}

void CadSurfDoc::OnTorus()
{
    dContext->DeleteAll();

    CPoint3D P(200,0,0);
    double maj = 100, min = 50;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CAxisSystem Ax1(P, CVector3D(0,0,1));
    CTorus S(Ax1, maj, min, PI, 2*PI, PI, 2*PI);
    CGLSurface* gS = new CGLSurface(&S);
    dContext->Display(gS);

    QString str;
    str.sprintf("Torus Of Maj Radius %0.3lf, Min Radius %0.3lf At P(%0.3lf,%0.3lf,%0.3lf)",S.GetMajorRadius(),S.GetMinorRadius(),P.GetX(), P.GetY(), P.GetZ());
    CGLFont *myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CPoint3D P1(-200, 100, 0);
    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);
    CAxisSystem Ax(P1, CVector3D(1,0,1));
    CTorus S1(Ax, maj, min);
    CGLSurface* gS1 = new CGLSurface(&S1);
    dContext->Display(gS1);

    str.sprintf("Maj Radius %0.3lf ",S1.GetMajorRadius());
    CPoint3D R = P1;
    R.Translate(R, S1.PointAtPara(PI/2, 0));
    CGLFont *myFont1 = new CGLFont(str, R);
    dContext->Display(myFont1);

    str.sprintf("Min Radius %0.3lf ",S1.GetMinorRadius());
    CPoint3D R1 = P1;
    R1.Translate(R1, S1.PointAtPara(0, 0));
    CGLFont *myFont2 = new CGLFont(str, R1);
    dContext->Display(myFont2);

    CVector3D D = Ax.GetDirection();
    str.sprintf("Local Coordinate System At P1(%0.3lf,%0.3lf,%0.3lf), Direction(%0.3lf,%0.3lf,%0.3lf)",P1.GetX(), P1.GetY(), P1.GetZ(), D.GetX(), D.GetY(), D.GetZ());
    CGLFont *myFont3 = new CGLFont(str, P1);
    dContext->Display(myFont3);

    CGLAxisSystem* gAx = new CGLAxisSystem(Ax);
    dContext->Display(gAx);

    delete myFont; delete myFont1;	delete myFont2;	delete myFont3;
    delete gP; delete gS; delete gP1; delete gS1; delete gAx;
}

void CadSurfDoc::OnExtsurf()
{

    dContext->DeleteAll();


    CAxisSystem sAx(CPoint3D(-500, -800, 500), CVector3D(1, 0.5, 2));
    CParabola3D Par(sAx, 50);
    Par.SetYRange(-15, 15);
    CGLCurve* C1 = new CGLCurve(&Par);
    dContext->Display(C1);

    QString str;
    str.sprintf("Base Curve (Parabola)");
    CGLFont* myFont1 = new CGLFont(str, Par.PointAtPara(0));
    dContext->Display(myFont1);

    CAxisSystem sAx1(CPoint3D(500, -800, -100), CVector3D(-1, 5, -2));
    CEllipse3D El(sAx1, 200, 100);
    CGLCurve* C2 = new CGLCurve(&El);
    dContext->Display(C2);

    str.sprintf("Base Curve (Ellipse)");
    CGLFont* myFont2 = new CGLFont(str, El.PointAtPara((El.LastParameter()-El.FirstParameter())/2));
    dContext->Display(myFont2);

    CVector3D Dir(0,0,1);
    CExtrudedSurface E(&Par, Dir, -200, 100);
    CGLSurface* S = new CGLSurface(&E);
    dContext->Display(S);

    COneAxis Ax(Par.PointAtPara(0), Dir);
    CGLOneAxis* gAx = new CGLOneAxis(Ax, 200, 20);
    dContext->Display(gAx);

    CVector3D Dir1(-1,2,1);
    CExtrudedSurface E1(&El, Dir1, -200, 100);
    CGLSurface* S1 = new CGLSurface(&E1);
    dContext->Display(S1);

    COneAxis Ax1(El.PointAtPara(PI), Dir1);
    CGLOneAxis* gAx1 = new CGLOneAxis(Ax1, 200, 20);
    dContext->Display(gAx1);

    delete C1; delete C2; delete S; delete S1; delete gAx; delete gAx1;
    delete myFont1; delete myFont2;

}

void CadSurfDoc::OnRevsurf()
{

    dContext->DeleteAll();


    CListOfCPoint3D *myCPList = new CListOfCPoint3D;
    CPoint3D P1(442, 642, 0);
    CPoint3D P2(208, 598, 0);
    CPoint3D P3(132, 552, 0);
    CPoint3D P4(146, 458, 0);
    CPoint3D P5(462, 320, 0);
    CPoint3D P6(686, 48, 0);
    CPoint3D P7(486, -186, 0);
    CPoint3D P8(152, -176, 0);
    CPoint3D P9(0, -176, 0);
    myCPList->Append(P1);
    myCPList->Append(P2);
    myCPList->Append(P3);
    myCPList->Append(P4);
    myCPList->Append(P5);
    myCPList->Append(P6);
    myCPList->Append(P7);
    myCPList->Append(P8);
    myCPList->Append(P9);

    CBezierCurve B(myCPList);
    B.Scale(P9, 1.5);
    CGLCurve* C1 = new CGLCurve(&B);
    dContext->Display(C1);

    CPoint3D P = B.PointAtPara(0.5);
    QString str;
    str.sprintf("Base Curve (Bezier)");
    CGLFont* myFont = new CGLFont(str, P);
    dContext->Display(myFont);

    CVector3D Dir(0,1,0);
    COneAxis Ax(P9, Dir);
    CRevolvedSurface S1(&B, Ax, 0, 2*PI);
    CGLSurface* gS = new CGLSurface(&S1);    
    dContext->Display(gS);

    CGLOneAxis* gAx = new CGLOneAxis(Ax, 500, 50);
    dContext->Display(gAx);

    str.sprintf("Axis Of Revolution");
    CPoint3D Pa = Ax.GetPosition();
    Pa.Translate(Ax, 500);
    CGLFont* myFont1 = new CGLFont(str, Pa);
    dContext->Display(myFont1);

    myCPList->Clear();
    delete myCPList;

    CAxisSystem sAx(CPoint3D(-1500, -800, 500), CVector3D(1, 0.5, 2));
    CParabola3D Par(sAx, 50);
    Par.SetYRange(-15, 15);
    CGLCurve* C2 = new CGLCurve(&Par);
    dContext->Display(C2);

    str.sprintf("Base Curve (Parabola)");
    CGLFont* myFont2 = new CGLFont(str, Par.PointAtPara(10));
    dContext->Display(myFont2);

    COneAxis Ax2(sAx.GetPosition()-50, sAx.GetYDirection());
    CGLOneAxis* gAx1 = new CGLOneAxis(Ax2, 500, 50);
    dContext->Display(gAx1);

    str.sprintf("Axis Of Revolution");
    Pa = Ax2.GetPosition();
    Pa.Translate(Ax2, 500);
    CGLFont* myFont3 = new CGLFont(str, Pa);
    dContext->Display(myFont3);

    CRevolvedSurface S2(&Par, Ax2, 0, PI);
    CGLSurface* gS2 = new CGLSurface(&S2);
    gS2->SetMaterial(GOLD);
    dContext->Display(gS2);

    delete C1; delete C2; delete gS; delete gS2; delete gAx; delete gAx1;
    delete myFont; delete myFont1; delete myFont2; delete myFont3;
}

void CadSurfDoc::OnRuledsurf()
{

    dContext->DeleteAll();


    CListOfCPoint3D *myCPList = new CListOfCPoint3D;
    CPoint3D P1(-450,-200,0);
    CPoint3D P2(-350,300,0);
    CPoint3D P3(-200,-300,0);
    CPoint3D P4(-200,400,0);
    CPoint3D P5(200,-300,0);
    CPoint3D P6(300,400,0);
    CPoint3D P7(350,-200,0);
    CPoint3D P8(400,300,0);
    myCPList->Append(P1);
    myCPList->Append(P2);
    myCPList->Append(P3);
    myCPList->Append(P4);
    myCPList->Append(P5);
    myCPList->Append(P6);
    myCPList->Append(P7);
    myCPList->Append(P8);

    CBezierCurve B1(myCPList);
    B1.Translate(0,100,300);

    CBSplineCurve B2(myCPList);
    B2.Translate(0,100,-300);

    CAxisSystem Ax(CPoint3D(0,200, -300), CVector3D(0,0,1));
    CParabola3D Par(Ax, 50);
    Par.SetYRange(-15, 15);

    CGLCurve* gC1 = new CGLCurve(&B1);
    CGLCurve* gC2 = new CGLCurve(&B2);

    dContext->Display(gC1);
    dContext->Display(gC2);

    QString str;
    str.sprintf("Rail Curve 1 (Bezier)");
    CGLFont* myFont1 = new CGLFont(str, B1.PointAtPara((B1.LastParameter()-B1.FirstParameter())/2));
    dContext->Display(myFont1);

    str.sprintf("Rail Curve 2 (B-Spline)");
    CGLFont* myFont2 = new CGLFont(str, B2.PointAtPara((B2.LastParameter()-B2.FirstParameter())/2));
    dContext->Display(myFont2);

    CRuledSurface R(&B1, &B2);
    CGLSurface* S = new CGLSurface(&R);
    dContext->Display(S);

    myCPList->Clear();
    delete myCPList;

    CAxisSystem Ax1(CPoint3D(0,-500,200), CVector3D(0,0,1));
    CCircle3D C(Ax1, 200);
    C.Reverse();
    CGLCurve* gC3 = new CGLCurve(&C);
    dContext->Display(gC3);

    CAxisSystem Ax2(CPoint3D(0,-500,-200), CVector3D(0,0,1));
    CEllipse3D E(Ax2, 160, 70);
    CGLCurve* gC4 = new CGLCurve(&E);
    dContext->Display(gC4);

    str.sprintf("Rail Curve 1 (Circle)");
    CGLFont* myFont3 = new CGLFont(str, C.PointAtPara((C.LastParameter()-C.FirstParameter())/2));
    dContext->Display(myFont3);

    str.sprintf("Rail Curve 2 (Ellipse)");
    CGLFont* myFont4 = new CGLFont(str, E.PointAtPara((E.LastParameter()-E.FirstParameter())/2));
    dContext->Display(myFont4);

    CRuledSurface R1(&C, &E);
    CGLSurface* S1 = new CGLSurface(&R1);
    dContext->Display(S1);

    delete gC1; delete gC2; delete gC3; delete gC4; delete S; delete S1;
    //delete myFont1; delete myFont2; delete myFont3; delete myFont4;
}

void CadSurfDoc::OnPipe()
{
    dContext->DeleteAll();

    CListOfCPoint3D *myCPList = new CListOfCPoint3D;
    CPoint3D P1(-450,-300,-00);
    CPoint3D P2(-350,300,-00);
    CPoint3D P3(-200,-300,-00);
    CPoint3D P4(-200,400,-00);
    CPoint3D P5(200,-300,-00);
    CPoint3D P6(300,400,-00);
    CPoint3D P7(350,-300,-00);
    CPoint3D P8(400,300,-00);
    myCPList->Append(P1);
    myCPList->Append(P2);
    myCPList->Append(P3);
    myCPList->Append(P4);
    myCPList->Append(P5);
    myCPList->Append(P6);
    myCPList->Append(P7);
    myCPList->Append(P8);

    CAxisSystem Ax(CPoint3D(0,-500,0), CVector3D(0,0,1), CVector3D(1,0,0));
    CParabola3D P(Ax, 50);
    P.SetYRange(-10, 10);

    CGLCurve* gC = new CGLCurve(&P);
    dContext->Display(gC);

    CBezierCurve B(myCPList);
    CGLCurve* gC1 = new CGLCurve(&B);
    dContext->Display(gC1);

    CPipeSurface S(&B, 50); //circular section along bezier
    CGLSurface* gS = new CGLSurface(&S);
    dContext->Display(gS);

    CPipeSurface S1(&P, 80, 50); //elliptical section along parabola
    CGLSurface* gS1 = new CGLSurface(&S1);
    dContext->Display(gS1);

    myCPList->Clear();
    delete myCPList;

    delete gC; delete gC1; delete gS; delete gS1;
}

// User Defined surface creation
#include "UserSurface.h"
void CadSurfDoc::OnUserSurface() 
{
    cout << "called\n";
    dContext->DeleteAll();

    CUserSurface *us = new CUserSurface();
    CGLSurface* S = new CGLSurface(us);
    S->SetMaterial(BRONZE);
    dContext->Display(S);

    QString str;
    str.sprintf("User Defined Surface \"Klein Bottle Variant 2\"");
    CGLFont* myFont = new CGLFont(str, us->PointAtPara(0.5, 0.5));
    dContext->Display(myFont);	

    delete us; delete S; delete myFont;
}

//Transformations

void CadSurfDoc::OnTranslate()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double Rad = 100, Ht = 200;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CAxisSystem Ax11(CPoint3D(200,0,0), CVector3D(0,0,1));
    CCone C(Ax11, Rad, Ht);
    CGLSurface* gS = new CGLSurface(&C);
    dContext->Display(gS);

    CCone tC = C;
    tC.Translate(0,300,0);
    CGLSurface* gSt = new CGLSurface(&tC);
    dContext->Display(gSt);

    COneAxis Ax(CPoint3D(200,0,0), CVector3D(0,300,0));
    CGLOneAxis* gAx = new CGLOneAxis(Ax, 270, 30);
    dContext->Display(gAx);

    CPoint3D P1(-200,-300,-100);

    CGLPoint* gP1 = new CGLPoint(P1);
    dContext->Display(gP1);

    CCylinder C1(CAxisSystem(P1, CVector3D(1,0,1)), Rad, Ht);
    CGLSurface* gS1 = new CGLSurface(&C1);
    dContext->Display(gS1);

    CCylinder tC1 = C1;
    tC1.Translate(COneAxis::OX(), 400);
    CGLSurface* gSt1 = new CGLSurface(&tC1);
    dContext->Display(gSt1);

    COneAxis Ax1(P1, CVector3D(1,0,0));
    CGLOneAxis* gAx1 = new CGLOneAxis(Ax1, 460, 40);
    dContext->Display(gAx1);

    delete gP; delete gS; delete gSt; delete gAx;
    delete gP1; delete gS1; delete gSt1; delete gAx1;
}

void CadSurfDoc::OnRotate()
{

    dContext->DeleteAll();


    CPoint3D P(200,0,0);
    double maj = 100, min = 50;

    CGLPoint* gP = new CGLPoint(P);
    dContext->Display(gP);

    CAxisSystem Ax1(P, CVector3D(0,0,1));
    CTorus S(Ax1, maj, min);
    CGLSurface* gS = new CGLSurface(&S);
    dContext->Display(gS);

    COneAxis Ax(CPoint3D(-200, 00, 0), CVector3D(1,1,1));
    CGLOneAxis* gAx = new CGLOneAxis(Ax);
    dContext->Display(gAx);

    CTorus tS = S;
    tS.Rotate(Ax, PI/2);
    CGLSurface* gS1 = new CGLSurface(&tS);
    dContext->Display(gS1);

    delete gP; delete gS; delete gAx; delete gS1;
}

void CadSurfDoc::OnScale()
{

    dContext->DeleteAll();


    CAxisSystem Ax1(CPoint3D(0,-500,200), CVector3D(0,0,1));
    CCircle3D C(Ax1, 200);
    C.Reverse();

    CAxisSystem Ax2(CPoint3D(0,-500,-200), CVector3D(0,0,1));
    CEllipse3D E(Ax2, 160, 70);

    CRuledSurface R1(&C, &E);
    CGLSurface* S1 = new CGLSurface(&R1);
    dContext->Display(S1);

    CRuledSurface* sR = static_cast<CRuledSurface*>(R1.Copy());
    sR->Scale(CPoint3D::Origin(), -2.5);
    CGLSurface* S2 = new CGLSurface(sR);
    dContext->Display(S2);

    CRuledSurface* sR1 = static_cast<CRuledSurface*>(R1.Copy());
    sR1->Scale(CPoint3D::Origin(), 2.5);
    CGLSurface* S3 = new CGLSurface(sR1);
    dContext->Display(S3);

    delete S1; delete S2; delete sR; delete S3; delete sR1;
}

void CadSurfDoc::OnReflect()
{
    dContext->DeleteAll();


    CAxisSystem Ax1(CPoint3D(400,500,-200), CVector3D(0,0,1));
    CCircle3D C(Ax1, 200);
    C.Reverse();

    CAxisSystem Ax2(CPoint3D(400,500,-400), CVector3D(0,0,1));
    CEllipse3D E(Ax2, 160, 70);

    CRuledSurface R1(&C, &E);
    CGLSurface* S1 = new CGLSurface(&R1);
    dContext->Display(S1);

    QString str;
    str.sprintf("Original Surface");
    CGLFont* myFont1 = new CGLFont(str, R1.PointAtPara(0.5, 0.5));
    dContext->Display(myFont1);

    CRuledSurface* sR = static_cast<CRuledSurface*>(R1.Copy());
    sR->Mirror(CPoint3D::Origin());
    CGLSurface* S2 = new CGLSurface(sR);
    S2->SetMaterial(GOLD);
    dContext->Display(S2);

    str.sprintf("Mirrored @ Point Origin");
    CGLFont* myFont2 = new CGLFont(str, sR->PointAtPara(0.5, 0.5));
    dContext->Display(myFont2);

    CRuledSurface* sR1 = static_cast<CRuledSurface*>(R1.Copy());
    sR1->Mirror(COneAxis::OY());
    CGLSurface* S3 = new CGLSurface(sR1);
    S3->SetMaterial(COPPER);
    dContext->Display(S3);

    str.sprintf("Mirrored @ Axis OY");
    CGLFont* myFont3 = new CGLFont(str, sR1->PointAtPara(0.5, 0.5));
    dContext->Display(myFont3);

    CRuledSurface* sR2 = static_cast<CRuledSurface*>(R1.Copy());
    sR2->Mirror(CPlane::XOY());
    CGLSurface* S4 = new CGLSurface(sR2);
    S4->SetMaterial(BRONZE);
    dContext->Display(S4);

    str.sprintf("Mirrored @ Plane XOY");
    CGLFont* myFont4 = new CGLFont(str, sR2->PointAtPara(0.5, 0.5));
    dContext->Display(myFont4);

    delete S1; delete sR; delete S2; delete sR1; delete S3; delete sR2; delete S4;
    delete myFont1; delete myFont2; delete myFont3; delete myFont4;
}
