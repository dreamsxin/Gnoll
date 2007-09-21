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


/*----------------------cgenericmessagemanager.cpp-------------------------*\
|   This is a generic message manager                                       |
|                                                                           |
|   Changelog :                                                             |
|               05/15/2006 - Paf - Initial release                          |
|                                                                           |
\*-------------------------------------------------------------------------*/
#include <iostream>

#include "../include/cgenericmessagemanager.h"




bool CGenericMessageManager::addListener ( shared_ptr<CMessageListener> handler, CMessageType messagetype )
{

	if ( validateType( messagetype ) == false )
		return false;


	/*
	 * If this new listener handle message types that are unknown, 
	 * this new message type is added to the set m_messagetypes.
	 * If it fails, it returns false
	 */

	if ( m_messagetypes.find(messagetype) == m_messagetypes.end() )
	{

		pair<set<CMessageType>::iterator,bool> result = m_messagetypes.insert(messagetype);

		if ( ( result.second == false ) ||  ( result.first == m_messagetypes.end() ) )
			return false;

	}


	/*
	 * Each tupple (handler, message type) must be unique.
	 * So it's checked.
	 */

	bool found = false;

	for (multimap< CMessageType, shared_ptr<CMessageListener> >::iterator it = m_listeners.lower_bound(messagetype);
       ( (it != m_listeners.upper_bound(messagetype) ) && (found == false) ); ++it)
	{
		if ( (*it).second == handler )
			found = true;
	}

	if (found == true)
		return false;


	/*
	 * Finally the new listener is added :)
	 */

	m_listeners.insert(pair< CMessageType, shared_ptr<CMessageListener> >(messagetype, handler));
	return true;

}

		
bool CGenericMessageManager::delListener ( shared_ptr<CMessageListener> handler, CMessageType messagetype )
{
	if ( validateType( messagetype ) == false )
		return false;

	/*
	 * If this listener handle message types that are unknown, 
	 * it returns false because it's impossible to have it also in the listener's multimap.
	 */

	if ( m_messagetypes.find(messagetype) == m_messagetypes.end() )
	{

		pair<set<CMessageType>::iterator,bool> result = m_messagetypes.insert(messagetype);

		if ( ( result.second == false ) ||  ( result.first == m_messagetypes.end() ) )
			return false;

	}


	/*
	 * Then it's deleted from the listener's multimap
	 */

	bool found = false;
	multimap< CMessageType, shared_ptr<CMessageListener> >::iterator result = m_listeners.end();

	for (multimap< CMessageType, shared_ptr<CMessageListener> >::iterator it = m_listeners.lower_bound(messagetype);
       ( (it != m_listeners.upper_bound(messagetype) ) && (found == false) ); ++it)
	{
		if ( (*it).second == handler )
		{
			found = true;
			result = it;
		}
	}

	if (found == false)
		return false;

	m_listeners.erase( result );





	/*
	 * If this listener was the last one to handle its message type, 
	 * the message type has to be deleted from the set of message types.
	 */

	if ( m_listeners.find(messagetype) == m_listeners.end() )
	{

		set<CMessageType>::iterator result = m_messagetypes.find(messagetype);

		if ( result != m_messagetypes.end() )
		{
			m_messagetypes.erase(result);
		}
		else
			// message type not found
			return false;
	}



	return true;

}


bool CGenericMessageManager::trigger ( shared_ptr<CMessage> message )
{

	if ( validateType( message->getType() ) == false )
		return false;


	/*
	 * First, the message is processed by listeners which listen all types of messages
	 */

	CMessageType anytype(MSG_ANYTYPE);

	for (multimap< CMessageType, shared_ptr<CMessageListener> >::iterator it = m_listeners.lower_bound(anytype);
       it != m_listeners.upper_bound(anytype) ; ++it)
	{

		((*it).second)->handle(message);

	}


	/*
	 * Then, the message is processed by listeners according to its type
	 */

	for (multimap< CMessageType, shared_ptr<CMessageListener> >::iterator it = m_listeners.lower_bound(message->getType());
       it != m_listeners.upper_bound(message->getType())  ; ++it)
	{

		((*it).second)->handle(message);

	}

	/*
	 * Everything is ok, so true is returned
	 */
	return true;

}


bool CGenericMessageManager::queueMessage ( shared_ptr<CMessage> message )
{
	if ( validateType( message->getType() ) == false )
		return false;


	/*
	 * The message type has to be registered
	 */

	if ( m_messagetypes.find(message->getType()) == m_messagetypes.end() )
		return false;


	/*
	 * If there is no one to listen...
	 */

	if ( m_listeners.find(message->getType()) == m_listeners.end() )
		return false;


	/*
	 * Finally the message is enqueue
	 */

	m_messages[m_activequeue].push_back(message);
	return true;

}


bool CGenericMessageManager::abortMessage ( CMessageType messagetype, bool alloftype )
{
	if ( validateType( messagetype ) == false )
		return false;

	/*
	 * Each message of messagetype in the message queue is deleted if alloftype = true
	 *	or the first one if alloftype = false	
	 */

	bool result = false;
	bool noincr = false;
	for ( list< shared_ptr<CMessage> >::iterator it = m_messages[m_activequeue].begin(); it != m_messages[m_activequeue].end() && (result == false); noincr ? it : it++)
	{
		noincr = false;
		if ( (*it)->getType() == messagetype )
		{
			m_messages[m_activequeue].erase(it);
			it = m_messages[m_activequeue].begin();
			noincr = true;

			if ( alloftype == false )
			{
				result = true;
			}
		}
	}

	return true;
}


bool CGenericMessageManager::validateType( CMessageType messagetype )
{
	/*
	 * We don't want any empty strings
	 */

	if (messagetype.getTypeStr().empty())
		return false;

	return true;
}


void CGenericMessageManager::process( )
{

	/*
	 * We can't process an active message queue because a handler could create a
	 * new message which would be processed and would create another message and so on...
	 * So it would turn into an infinite loop.
	 */

	unsigned int queuetoprocess = m_activequeue;
	m_activequeue = (m_activequeue + 1) % NUMQUEUE;



	/*
	 * We send messages to every handlers which sniff everything
	 */

	CMessageType anytype(MSG_ANYTYPE);

	for ( list< shared_ptr<CMessage> >::iterator itmsg = m_messages[queuetoprocess].begin(); itmsg != m_messages[queuetoprocess].end(); itmsg++ )
	{

		for (multimap< CMessageType, shared_ptr<CMessageListener> >::iterator it = m_listeners.lower_bound(anytype);
   	    it != m_listeners.upper_bound(anytype) ; ++it)
		{
	
			((*it).second)->handle( *itmsg );
	
		}


	/*
	 * Then we send messages to registered handlers 
	 */
	
		for (multimap< CMessageType, shared_ptr<CMessageListener> >::iterator it = m_listeners.lower_bound((*itmsg)->getType());
  	     it != m_listeners.upper_bound((*itmsg)->getType())  ; ++it)
		{

			((*it).second)->handle( *itmsg );

		}

	}


	/*
	 * All messages are erased
	 */

	m_messages[queuetoprocess].erase( m_messages[queuetoprocess].begin(), m_messages[queuetoprocess].end() );
}
