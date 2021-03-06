/**************************************************************************
*   Copyright (C) 2008 by Bruno Mahe, Gabriel, Soax                       *
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


/*-------------------------soundplaylistener.cpp---------------------------*\
|   The sound module                                                        |
|                                                                           |
|   Changelog :                                                             |
|          11/06/2007 - Soax - Initial release                              |
|          02/04/2008 - Bruno Mahe - Update comments                        |
|                                                                           |
|                                                                           |
\*-------------------------------------------------------------------------*/




#include "../include/soundplaylistener.h"
#include <iostream>
using namespace std;

namespace Gnoll
{
	namespace Sound
	{

		SoundPlayListener::SoundPlayListener(vector< string > * _sound_queue)
		{
			sound_queue = _sound_queue;
		}
		
		void SoundPlayListener::handle( shared_ptr<CMessage> message)
		{
			string instance = message->getData<string>();
			sound_queue->push_back(instance);
		}
	}
}



