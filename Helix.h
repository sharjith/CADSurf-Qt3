/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


/***************************************************************************
 *            Helix.h
 *
 *  Fri Jul 21 16:40:24 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


// Helix.h: interface for the CHelix class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HELIX_H
#define _HELIX_H

#include "Circle3D.h"
#include "AxisSystem.h"

class CHelix : public CCircle3D 
{
    public:
	CHelix(const CAxisSystem& Ax, const double& radius, const double& start, const double& end, const double lead);
	virtual ~CHelix();
	virtual void Reverse();
	virtual CPoint3D PointAtPara(const double& para);
	virtual double FirstParameter() const;
	virtual double LastParameter() const;
	virtual bool IsClosed() const;
	CurveType Type() const;
	bool IsOfType(const CurveType&) const;
	virtual CCurve* Copy() const;
	virtual void Translate(double dx, double dy, double dz);
	virtual void Translate(const CVector3D&);
	virtual void Translate(const CPoint3D&, const CPoint3D&);
	virtual void Rotate(const COneAxis&, double);
	virtual void Scale(const CPoint3D&, double);
	virtual void Mirror(const CPoint3D&);
	virtual void Mirror(const COneAxis&);
	virtual void Mirror(const CPlane&);
	double Lead() const { return itsLead;}
    private:
	//CAxisSystem itsLocation;
	//double itsRadius;
	double itsFirstParameter;
	double itsLastParameter;
	double itsLead;
};

#endif /* _HELIX_H */

