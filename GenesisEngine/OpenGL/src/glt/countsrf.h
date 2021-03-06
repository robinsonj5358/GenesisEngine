#ifndef GLT_COUNTSRF_H
#define GLT_COUNTSRF_H

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
    \brief   Surface Counting Interface 
    \ingroup GLT

    $Id: countsrf.h,v 1.7 2002/10/07 16:27:46 nigels Exp $

    $Log: countsrf.h,v $
    Revision 1.7  2002/10/07 16:27:46  nigels
    Added CVS version control info


*/

#include <glt/config.h>
#include <glt/gl.h>

class GltShape;

typedef enum                                      
{
	COUNT_SURFACES_ALL,				/*!< Count the maximum number of surfaces exactly. */
	COUNT_SURFACES_99TH_PERCENTILE	/*!< Take the maximum of the 99th percentile, ignoring outliers. */
} countSurfacesMode;

/*! \brief Use the stencil buffer to count the number of surfaces drawn at each pixel
    \ingroup GLT
    \param shape Shapes
    \warning Uses the current culling configuration
*/

void countSurfaces(const GltShape &shape);

/*! \brief Use the stencil buffer to count the number of surfaces drawn at each pixel
    \ingroup GLT
    \param shape Shapes
    \param mode  Count Mode
    \return Maximum number of surfaces covering a pixel
    \warning Uses the current culling configuration
*/

GLubyte countSurfaces(const GltShape &shape,const countSurfacesMode mode);

/*! \brief Visualise the number of rendered surfaces at each pixel
    \ingroup GLT
    \param shape Shapes
    \return Maximum number of surfaces covering a pixel
    \warning Uses the current culling configuration
*/

GLubyte countSurfacesVisualise(const GltShape &shape);


/*! \brief Extract a stencil buffer histogram for the current viewport
    \ingroup GLT
    \param  histogram  Array of GLuint histogram entries
    \return Number of pixels in the current viewport
*/

GLuint  stencilHistogram(GLuint histogram[256]);

#endif
