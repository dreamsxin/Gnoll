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

#ifndef __LISTENERCONTAINER_H__
#define __LISTENERCONTAINER_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			class Listener;
			class MessageType;

			/** Specialized and thread safe container for Listeners.
			 *
			 * The Listener container associates Listener instantes and message types.
			 * It can then call a functor for a given message type calling each listener listening to this type.
			 */
			class ListenerContainer
			{
				public:
					typedef boost::shared_ptr<Listener> ListenerPtr;
					typedef boost::function<void (ListenerContainer::ListenerPtr &) > ForEachFunction;

					ListenerContainer();
					virtual ~ListenerContainer();

					/** Adds an association between the listener and a message type.
					 *
					 * @throw if the association already exists, throws HandlerAlreadyRegistered
					 */
					void add(ListenerPtr listener, const MessageType & messageType);

					/** Removes an association between the listener and a message type.
					 *
					 * @throw if the association doesn't exists, throws CannotDeleteListener
					 */
					void del(ListenerPtr listener, const MessageType & messageType);

					/** Checks if an association has been added for the given message type.
					 *
					 * @return true if there is a listener associated to the given message type. false if not.
					 */
					bool hasListenerForType(const MessageType & messageType) const;

					/** Checks if a given listener is associated to a given message type.
					 *
					 * @return true if the given listener is associated to the given message type. false if not.
					 */
					bool isListenerAssociatedToType(ListenerPtr listener, const MessageType & messageType);

					/** Calls the given function for each listener associated to the given message type.
					 *
					 * The order of listeners passed to the function is undefined.
					 */
					void forEach(const MessageType & messageType, ForEachFunction function);

				private:
					typedef std::multimap<MessageType, ListenerPtr > ContainerType;

					ContainerType m_listeners;

					mutable boost::recursive_mutex m_mutex;

					void throwIfAlreadyListeningToType(ListenerPtr listener, const MessageType & messageType);

					ContainerType::iterator getListenerIteratorForType(ListenerPtr listener, const MessageType & messageType);
			};
		}
	}
}

#endif

