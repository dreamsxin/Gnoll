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

#include "../include/messagequeue.h"
#include "../include/messagetype.h"
#include "../../include/cmessage.h"
#include <boost/bind.hpp>
#include <algorithm>

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			namespace Details
			{
				static bool isMessageOfType(MessageQueue::MessagePtr message, const MessageType & type)
				{
					return message->getType() == type;
				}
			}

			MessageQueue::MessageQueue()
			{
				BOOST_STATIC_ASSERT(NUMBER_OF_QUEUES >= 2);

				m_writtenMessages = &m_messages[0];
				m_readMessages = &m_messages[1];
			}

			MessageQueue::~MessageQueue()
			{
			}

			void MessageQueue::pushMessage(MessagePtr message)
			{
				boost::recursive_mutex::scoped_lock writeLock(m_writeMutex);
				getWrittenMessages().push_back(message);
			}

			void MessageQueue::abortFirstOfType(const MessageType & messageType)
			{
				boost::recursive_mutex::scoped_lock writeLock(m_writeMutex);
				MessageContainer::iterator foundMessage = std::find_if(getWrittenMessages().begin(), getWrittenMessages().end(), boost::bind(Details::isMessageOfType, _1, messageType));

				if (foundMessage != getWrittenMessages().end())
				{
					getWrittenMessages().erase(foundMessage);
				}
			}

			void MessageQueue::abortAllOfType(const MessageType & messageType)
			{
				boost::recursive_mutex::scoped_lock writeLock(m_writeMutex);
				getWrittenMessages().remove_if(boost::bind(Details::isMessageOfType, _1, messageType));
			}

			bool MessageQueue::isEmpty() const
			{
				boost::recursive_mutex::scoped_lock writeLock(m_writeMutex);
				return getWrittenMessages().empty();
			}

			void MessageQueue::forEachAndClear(ForEachFunction function)
			{
				boost::recursive_mutex::scoped_lock readLock(m_readMutex);
				SwapContainer();
				std::for_each(getReadMessages().begin(), getReadMessages().end(), function);
				clearReadMessages();
			}

			void MessageQueue::clearReadMessages()
			{
				getReadMessages().clear();
			}

			MessageQueue::MessageContainer & MessageQueue::getWrittenMessages()
			{
				return *m_writtenMessages;
			}

			const MessageQueue::MessageContainer & MessageQueue::getWrittenMessages() const
			{
				return *m_writtenMessages;
			}
			
			MessageQueue::MessageContainer & MessageQueue::getReadMessages()
			{
				return *m_readMessages;
			}

			const MessageQueue::MessageContainer & MessageQueue::getReadMessages() const
			{
				return *m_readMessages;
			}
			
			void MessageQueue::SwapContainer()
			{
				boost::recursive_mutex::scoped_lock writeLock(m_writeMutex);
				std::swap(m_writtenMessages, m_readMessages);
			}
		}
	}
}


