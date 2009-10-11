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

#ifndef __MESSENGER_H__
#define __MESSENGER_H__

namespace boost
{
	template<class T>
	class shared_ptr;
}

namespace Gnoll
{
	namespace Core
	{
		class CMessage;

		namespace Messages
		{
			class MessageType;
			class Listener;

			/** Interface for message managers
			*
			* A message manager is in charge distributing messages to corresponding listeners.
			*
			* In any of the methods with a message in parameter, InvalidMessage will be thrown if the message type is invalid.
			* Being invalid nor not is determined by the Messenger implementation.
			*/
			class Messenger
			{
				public:
					typedef boost::shared_ptr<Listener> ListenerPtr;
					typedef boost::shared_ptr<CMessage> MessagePtr;

					// Messenger();
					virtual ~Messenger() {};

					/** Adds an association between the listener and a message type.
					 *
					 * @throw if the association already exists, throws HandlerAlreadyRegistered
					 */
					virtual void addListener(ListenerPtr listener, const MessageType & messagetype) = 0;

					/** Removes an association between the listener and a message type.
					 *
					 * @throw if the association doesn't exists, throws CannotDeleteListener
					 */
					virtual void delListener(ListenerPtr listener, const MessageType & messagetype) = 0;

					/** Sends the given message to all listeners associated with its message type immediatly
					 *
					 * @throw if no listener is associated to the message type, NoOneIsListening is thrown.
					 */
					virtual void triggerMessage(MessagePtr message) = 0;

					/** Adds the given message to the queue for later processin by processQueue()
					 *
					 * @throw if no listener is associated to the message type, NoOneIsListening is thrown.
					 */
					virtual void queueMessage(MessagePtr message ) = 0;

					/** Aborts the first message of the given type in the queue. */
					virtual void abortFirstMessage(const MessageType & messagetype) = 0;

					/** Aborts all messages of the given type in the queue. */
					virtual void abortAllMessages(const MessageType & messagetype) = 0;

					/** Sends all queued messages to all listeners associated with their respective type. */
					virtual void processQueue() = 0;
			};
		}
	}
}

#endif

