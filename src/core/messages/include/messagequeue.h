/***************************************************************************
 *   Copyright (C) 2006 by Gnoll Team                                      *
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

#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <list>

namespace Gnoll
{
	namespace Core
	{
		class CMessage;

		namespace Messages
		{
			class MessageType;

			/** A specialized and thread safe queue handling messages. */
			class MessageQueue
			{
				public:
					typedef boost::shared_ptr<CMessage> MessagePtr;
					typedef boost::function<void (const MessagePtr &) > ForEachFunction;

					MessageQueue();
					~MessageQueue();

					/** Adds a message to the queue.
					 *
					 * If pushMessage is called because directly or indirectly by forEachAndClear, the
					 * message is not added to the current handled queue.
					 */
					void pushMessage(MessagePtr message);

					/** Aborts the first message of the given type in the queue. */
					void abortFirstOfType(const MessageType & messageType);

					/** Aborts all messages of the given type in the queue. */
					void abortAllOfType(const MessageType & messageType);

					/** Checks if the queue is empty.
					 *
					 * @return true if the queue has no element. When called directly or indirectly from forEachAndClear, returns false.
					 */
					bool isEmpty() const;

					/** Calls a function for all pushed messages.
					 *
					 * The messages are called in the order they were pushed.
					 */
					void forEachAndClear(ForEachFunction function);

				private:
					typedef std::list<MessagePtr> MessageContainer;
					typedef std::list<MessagePtr> * MessageContainerPtr;

					enum
					{
						NUMBER_OF_QUEUES = 2
					};

					MessageContainer m_messages[NUMBER_OF_QUEUES];
					MessageContainerPtr m_writtenMessages;
					MessageContainerPtr m_readMessages;

					mutable boost::recursive_mutex m_writeMutex;
					mutable boost::recursive_mutex m_readMutex;

					void clearReadMessages();

					MessageContainer & getWrittenMessages();
					const MessageContainer & getWrittenMessages() const;

					MessageContainer & getReadMessages();
					const MessageContainer & getReadMessages() const;

					void SwapContainer();
			};
		}
	}
}

#endif

