/**************************************************************************
*   Copyright (C) 2008 by Paf                                             *
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


/*----------------------CMouseButtonEventsTranslator-----------------------*\
|   This is translate mouse button events to action events                  |
|                                                                           |
|   Changelog :                                                             |
|               01/11/2008 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/

#ifndef __CMOUSEBUTTONEVENTSTRANSLATOR_H__
#define __CMOUSEBUTTONVENTSTRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../dynamicobject/include/dynamicobject.h"
#include "../../core/include/cmessagelistener.h"
#include "../../core/include/cmessage.h"


using namespace boost;
using namespace Gnoll::Core;
using namespace Gnoll::DynamicObject;

namespace Gnoll
{
	namespace Input 
	{

		class CMouseButtonEventsTranslator : public CMessageListener
		{

			private:

				/**
				 * DynamicObject which contains a translation map for events from mouse
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> mouseButtonEventTranslationMap;


				/**
				 * DynamicObject which contains mouse config
				 */
				shared_ptr<Gnoll::DynamicObject::DynamicObject> mouseConfig;


				/**
				 * CMessageType for MOUSE_PRESSED messages
				 */
				CMessageType mouseButtonPressedEvent;


				/**
				 * CMessageType for MOUSE_RELEASED messages
				 */
				CMessageType mouseButtonReleasedEvent;


				/**
				 * How long a button has been pressed
				 */
				map<string, unsigned long int> m_durationButtonPressed;

				/**
				 * When a button has been pressed
				 */
				map<string, unsigned long int> m_buttonPressed;

				/**
				 * When trigger has been called last time
				 */
				unsigned long int m_lastTimeTriggerCalled;


			public:

				/**
				* This is a constructor
				*/
				CMouseButtonEventsTranslator();

				/**
				* This is a destructor
				*/
				virtual ~CMouseButtonEventsTranslator(); 

				/**
				* This method is called in order to process a message
				* @param message The message this method will have to process
				*/
				virtual void handle ( shared_ptr<CMessage> message );

				/**
				* Send mouse button events if any button has been pressed
				*/
				void trigger(shared_ptr<CMessage> _msg);
		};
	};
};

#endif // __CMOUSEEVENTSTRANSLATOR_H__

