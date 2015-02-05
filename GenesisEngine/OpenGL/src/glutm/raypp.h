#ifndef GLUTM_RAYPP_H
#define GLUTM_RAYPP_H

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
	\brief   Ray++ Support
	\ingroup GlutMaster
*/

#include <glutm/config.h>

#ifdef GLUTM_RAYPP

#include <string>

namespace RAYPP
{
	class SCENE;
}

class Matrix;

extern RAYPP::SCENE *scene;

extern bool initScene();
extern bool raytraceScene(const std::string &filename,const int width,const int height,const Matrix &modelview);

#endif

#endif