#ifndef NODE_SKYSPHERE_H
#define NODE_SKYSPHERE_H

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
	\brief   OpenGL Sky Sphere Node
	\ingroup Node
*/

#include <node/shapes.h>
#include <glt/colmap.h>

/*! \class   GltSkySphere
    \brief   OpenGL Sphere Box
    \ingroup GLT
*/

class GltSkySphere : public GltShape
{
public:
	/// Constructor
	GltSkySphere();
	/// Destructor
	virtual ~GltSkySphere();

	/// Draw sky-sphere
	virtual void draw() const;

	/// Clear
	void clear();

	/// Color map
	GltColorMap &map();

	/// Field of view
	      GLdouble &fov();
	/// Field of view
	const GLdouble  fov() const;

private:

	static void drawSphere(const GltColorMap &colmap,const int slices);

	GltColorMap _map;
	int         _slices;

	GLdouble   _fov;
};

#endif
