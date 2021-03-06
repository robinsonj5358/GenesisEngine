#ifndef GLUTM_MAIN_H
#define GLUTM_MAIN_H

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
	\brief   Optional main() and WinMain() implementations
	\ingroup GlutMaster
*/

#include <glutm/config.h>

#ifdef GLT_WIN32
#pragma warning(disable: 4786)		
#endif

#include <vector>
#include <string>

/*! 
	\brief		User-defined entry point for GlutMaster based programs
	\ingroup	GlutMaster
	\note		Use of built-in main() and WinMain() is optional.
	\note		Client code needs to supply this function.
*/
extern bool GlutMain(const std::vector<std::string> &arg);

#endif
