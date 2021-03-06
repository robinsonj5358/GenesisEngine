#ifndef NODE_SKYBOX_H
#define NODE_SKYBOX_H

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
	\brief   OpenGL Sky Box Node
	\ingroup Node
*/

#include <node/shapes.h>
#include <glt/texture.h>

/*! \class   GltSkyBox
    \brief   OpenGL Sky Box
    \ingroup GLT

	\todo Support large textures
*/

class GltSkyBox : public GltShape
{
public:
	/// Constructor
	GltSkyBox();
	/// Destructor
	virtual ~GltSkyBox();

	/// Draw sky-box
	virtual void draw() const;

	/// Reset textures
	void clear();

	//
	// Access textures
	//

	/// Right-hand texture
	GltTexture &positiveX();
	/// Left-hand texture
	GltTexture &negativeX();
	/// Ceiling texture
	GltTexture &positiveY();
	/// Floor texture
	GltTexture &negativeY();
	/// Back texture
	GltTexture &positiveZ();
	/// Front texture
	GltTexture &negativeZ();

	/// Field of view
	      GLdouble &fov();
	/// Field of view
	const GLdouble  fov() const;

private:

	GltTexture _positiveX;
	GltTexture _negativeX;
	GltTexture _positiveY;
	GltTexture _negativeY;
	GltTexture _positiveZ;
	GltTexture _negativeZ;

	GLdouble   _fov;
};

#endif
