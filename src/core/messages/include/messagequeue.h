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

#ifndef __GENERICMESSENGER_H__
#define __GENERICMESSENGER_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <list>

namespace Gnoll
{
	namespace Core
	{
		class CMessage;

		namespace Messages
		{
			class MessageType;

			class MessageQueue
			{
				public:
					typedef boost::shared_ptr<CMessage> MessagePtr;
					typedef boost::function<void (const MessagePtr &) > ForEachFunction;

					MessageQueue();
					~MessageQueue();

					void pushMessage(MessagePtr message);

					void abortFirstOfType(const MessageType & messageType);
					void abortAllOfType(const MessageType & messageType);

					bool isEmpty() const;

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

