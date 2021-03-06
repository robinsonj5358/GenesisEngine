#ifndef NODE_LINE_H
#define NODE_LINE_H

/*

  GLT OpenGL C++ Toolkit (LGPL)
  Copyright (C) 2000-2002 Nigel Stewart  

  Email: nigels@nigels.com   
  WWW:   http://www.nigels.com/glt/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

/*! \file 
	\brief   Line Node
	\ingroup Node
*/

#include <node/shape.h>
#include <math/vector3.h>
#include <glt/gl.h>

#include <vector>

///////////////////////////// GltLine ///////////////////////////////////////

/*! \clas    GltLine 
	\brief   Line Node
	\ingroup Node
*/

class GltLine : public GltShape
{
public:
	/// Constructor
	GltLine();
	/// Destructor
	virtual ~GltLine();

	virtual void draw() const;
	virtual bool boundingBox(Vector &min,Vector &max) const;
    virtual std::string description() const;

	// 
	// Get/set
	//
												/// Polyline vertecies
	const std::vector<Vector> &points() const;
												/// Polyline normals
	const std::vector<Vector> &normals() const;

												/// Polyline vertecies
	      std::vector<Vector> &points();
												/// Polyline normals
	      std::vector<Vector> &normals();

												/// Width of polyline
	const GLfloat &width() const;
												/// Width of polyline
	      GLfloat &width();

private:
	
	std::vector<Vector> _points;
	std::vector<Vector> _normals;

	GLfloat             _width;
};

#endif
