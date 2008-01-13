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


/*-----------------------------soundmanager.cpp----------------------------*\
|   This is basic sound manager with cache ability.                         |
|                                                                           |
|   Changelog :                                                             |
|               11/06/2007 - Soax - Initial release                         |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/soundmanager.h"


namespace Gnoll {
	namespace Sound {

							

		SoundManager::SoundManager()
		{
				
		
		}
		
		SoundManager::~SoundManager()
		{
		}
		
		
		shared_ptr<Sound> SoundManager::loadImpl( shared_ptr<IStream> _stream, string _instance)
		{	
			return AudioCodecManager::getInstancePtr()->decodeStream(_stream, _instance);
			
		}
		
		
		bool SoundManager::saveImpl( shared_ptr<IStream> _stream, shared_ptr<Sound> _obj, string _instance)
		{
		
		}

	}
}
