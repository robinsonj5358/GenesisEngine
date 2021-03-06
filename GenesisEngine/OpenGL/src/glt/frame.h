#ifndef GLT_FRAME_H
#define GLT_FRAME_H

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
    \brief OpenGL Frame Rate Information
    \ingroup GLT

    $Id: frame.h,v 1.10 2002/10/07 16:27:46 nigels Exp $

    $Log: frame.h,v $
    Revision 1.10  2002/10/07 16:27:46  nigels
    Added CVS version control info


*/

#include <iosfwd>
#include <vector>
#include <string>

#include <misc/timer.h>
#include <misc/cbuffer.h>

/*! \class GltFrameRate
    \brief OpenGL Frame Rate Information Class
    \ingroup GLT
*/

class GltFrameRate
{
public:
	/// Constructor
	GltFrameRate(const int sampleSize = 11);
	/// Destructor
	~GltFrameRate();

	/// Call this once for each redraw
	GltFrameRate &operator++();
	
	/// Get the total elapsed time since construction
	double elapsedTime() const;
	/// Get the total number of frames
	int   totalFrames() const;

	/// Get the (averaged) frame rate
	double frameRate() const;
	/// Get the (averaged) time per frame
	double frameTime() const;

	/// Summary frame rate information
	std::string summary() const;

	/// Detailed fram rate information
	std::string detailed() const;

private:

	Timer           _timer;				// Timer used for frame rate calculation
	int             _frames;			// Total number of frames
	double			_step;				// Minimum time step between recalculation
	
	double          _lastCalc;			// Time of last recalculation
	double          _lastFrameRate;		// Previously calculated frame rate (frame/sec)
	double          _lastFrameTime;		// Previously calculated frame time (sec/frame)
	
	cbuffer<double> _buffer;			// Circular buffer storage 
};

#endif
