#ifndef GLT_RASTER_H
#define GLT_RASTER_H

/*

  GLT OpenGL C++ Toolkit      
  Copyright (C) 2000-2002 Nigel Stewart
  Email: nigels@nigels.com   WWW: http://www.nigels.com/glt/

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
    \brief   OpenGL Raster Position Class
    \ingroup GLT

    $Id: raster.h,v 1.7 2002/10/07 16:33:35 nigels Exp $

    $Log: raster.h,v $
    Revision 1.7  2002/10/07 16:33:35  nigels
    Added CVS info


*/

#include <glt/config.h>
#include <glt/gl.h>

/*! \class   GltRasterPos
    \brief   OpenGL Raster Position Class
    \ingroup GLT

    Convenient manipulation of raster position for bitmap rendering. (and text)
*/

class GltRasterPos
{
public:
	
	/// Constructor
	GltRasterPos(const bool getIt = false);
	/// Constructor
	~GltRasterPos();

	/// Get the current OpenGL raster position
	void get();
	/// Set the current OpenGL raster position
	void set();
	/// Set the current OpenGL raster position
	void set(int x,int y);

								/// X raster positon			
	GLdouble &x();
								/// Y raster positon			
	GLdouble &y();
								/// Z raster positon			
	GLdouble &z();
								/// W raster positon			
	GLdouble &w();

								/// X raster positon			
	const GLdouble &x() const;
								/// Y raster positon			
	const GLdouble &y() const;
								/// Z raster positon			
	const GLdouble &z() const;
								/// W raster positon			
	const GLdouble &w() const;

private:
	
	/// OpenGL raster position in x,y,z and w
	GLdouble _pos[4];
};

#endif
