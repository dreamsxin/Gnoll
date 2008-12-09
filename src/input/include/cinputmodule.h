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


/*-----------------------------cinputmodule.h-------------------------------*\
|   Interface of all the game input modules                                  |
|                                                                            |
|   Changelog :                                                              |
|               06/23/2006 - Paf - Initial release                           |
|                                                                            |
\*--------------------------------------------------------------------------*/

#include "../../core/include/cmodule.h"


#ifndef __CINPUTMODULE_H__
#define __CINPUTMODULE_H__

using namespace Gnoll::Core;

/**
 *	Interface of all game modules. 
 */ 
class CInputModule: public CModule
{
	public:
		/**
		 * @copydoc CModule::init
		 */
		virtual void init() = 0;

		/**
		 * @copydoc CModule::process
		 */
		virtual void process() = 0;

		/**
		 * @copydoc CModule::exit 
		 */
		virtual void exit() = 0;

		/**
		 * @copydoc CModule::~CModule
		 */
		virtual ~CInputModule() {};
};

#endif // __CINPUTMODULE_H__
