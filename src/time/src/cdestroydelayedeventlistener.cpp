/***************************************************************************
 *   Copyright (C) 2007 by Paf                                             *
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


/*----------------------CDestroyDelayedEventListener-----------------------*\
|   This is defines listeners used by the time module                       |
|                                                                           |
|   Changelog :                                                             |
|               09/20/2007 - Paf - Initial release                          |
|               09/23/2007 - Paf - Renamed to CDestroyDelayedEventListener  |
|               09/30/2007 - Paf - Fix namespace (replace Core by Time)     |
|                                                                           |
\*-------------------------------------------------------------------------*/


#include "../include/cdestroydelayedeventlistener.h"



namespace Gnoll {

	namespace Time {

				
		CDestroyDelayedEventListener::CDestroyDelayedEventListener() 
		{
		}

		CDestroyDelayedEventListener::~CDestroyDelayedEventListener() 
		{
		}

		void CDestroyDelayedEventListener::handle ( shared_ptr<Message> message )
		{
		
			TimerEvent timerEvent ( message->getData<TimerEvent>());

			CTimeModule* timeModule = CTimeModule::getInstancePtr();

			timeModule->delDelayedEvent(timerEvent.delay, timerEvent.message);
		}

	}
}




