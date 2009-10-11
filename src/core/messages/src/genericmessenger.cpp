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

#include "../include/genericmessenger.h"
#include "../include/messagequeue.h"
#include "../include/listenercontainer.h"
#include "../include/listener.h"
#include "../include/exceptions.h"

namespace Gnoll
{
	namespace Core
	{
		namespace Messages
		{
			namespace Details
			{
				class Sender
				{
					public:
						Sender(const GenericMessenger::MessagePtr & message) :
							m_message(message)
						{
							assert(message);
						}

						void operator()(ListenerContainer::ListenerPtr & listener)
						{
							listener->handle(m_message);
						}

					private:
						const GenericMessenger::MessagePtr & m_message;
				};

				struct TriggerMessage
				{
					TriggerMessage(GenericMessenger & messenger) :
						m_messenger(messenger)
					{
					}

					void operator()(const MessageQueue::MessagePtr & message)
					{
						m_messenger.uncheckedTriggerMessage(message);
					}

					GenericMessenger & m_messenger;
				};
			}

			GenericMessenger::GenericMessenger() :
				m_listeners(new ListenerContainer()),
				m_messageQueue(new MessageQueue)
			{
			}

			GenericMessenger::~GenericMessenger()
			{
			}

			void GenericMessenger::throwIfTypeNotValid(const MessageType & type)
			{
				if (type.getTypeStr().empty())
				{
					throw Exceptions::InvalidMessage();
				}
			}

			void GenericMessenger::throwIfMessageNotValid(MessagePtr message)
			{
				throwIfTypeNotValid(message->getType());
			}

			void GenericMessenger::throwIfNoListenerForMessage(MessagePtr message)
			{
				if (!m_listeners->hasListenerForType(message->getType()))
				{
					throw Exceptions::NoOneIsListening();
				}
			}

			void GenericMessenger::addListener(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
				m_listeners->add(listener, messageType);
			}

			void GenericMessenger::delListener(ListenerPtr listener, const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);
				m_listeners->del(listener, messageType);
			}

			void GenericMessenger::triggerMessage(MessagePtr message)
			{
				throwIfMessageNotValid(message);
				throwIfNoListenerForMessage(message);

				uncheckedTriggerMessage(message);
			}

			void GenericMessenger::uncheckedTriggerMessage(const MessagePtr & message)
			{
				// TODO : MSG_ANYTYPE could be kept somewhere
				Details::Sender sendToListener(message);
				m_listeners->forEach(MessageType(MSG_ANYTYPE), sendToListener);
				m_listeners->forEach(message->getType(), sendToListener);
			}

			void GenericMessenger::queueMessage(MessagePtr message)
			{
				throwIfMessageNotValid(message);
				throwIfNoListenerForMessage(message);

				m_messageQueue->pushMessage(message);
			}

			void GenericMessenger::abortFirstMessage(const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);

				m_messageQueue->abortFirstOfType(messageType);
			}

			void GenericMessenger::abortAllMessages(const MessageType & messageType)
			{
				throwIfTypeNotValid(messageType);

				m_messageQueue->abortAllOfType(messageType);
			}

			void GenericMessenger::processQueue()
			{
				m_messageQueue->forEachAndClear(Details::TriggerMessage(*this));
			}
		}
	}
}

