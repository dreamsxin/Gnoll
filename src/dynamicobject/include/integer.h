/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __INTEGER_H__
#define __INTEGER_H__

#include <sstream>

#include "scalar.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace DynamicObject
	{
		/**
		*	This is a simple attribute. </br>
		*	This hold a signed integer.
		*/
		class Integer : public Scalar<int>
		{
			public:
				/**
				 * Returns Integer's DynamicObject name
				 * @return Integer's DynamicObject name
				 */
				inline static const char* DYNAMIC_OBJECT_NAME() { return "integer"; }

				Integer(int value = 0) : Scalar<int>(Integer::DYNAMIC_OBJECT_NAME(), value) {};
		};
	};
};

#endif
