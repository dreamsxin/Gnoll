#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../messages/include/messagequeue.h"
#include "../../include/cmessage.h"
#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessageQueue)

using namespace Gnoll::Core::Messages;

namespace
{
	static std::string MESSAGE_TYPENAME_1("type1");
	static std::string MESSAGE_TYPENAME_2("type2");

	static std::string MESSAGE("message");
}

BOOST_AUTO_TEST_CASE(no_message_at_creation)
{
	MessageQueue queue;

	BOOST_CHECK(queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(can_push_message)
{
	MessageQueue queue;
	MessageQueue::MessagePtr message(new Gnoll::Core::CMessage);

	BOOST_CHECK(queue.isEmpty());
	queue.pushMessage(message);

	BOOST_CHECK(!queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(can_abort_first_of_type)
{
	MessageQueue queue;
	MessageQueue::MessagePtr message1(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_1));

	queue.pushMessage(message1);
	queue.abortFirstOfType(MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(queue.isEmpty());

	queue.pushMessage(message1);
	queue.pushMessage(message1);
	queue.abortFirstOfType(MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(!queue.isEmpty());

	queue.abortFirstOfType(MessageType(MESSAGE_TYPENAME_1));
	BOOST_CHECK(queue.isEmpty());

	MessageQueue::MessagePtr message2(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_2));
	queue.pushMessage(message2);

	BOOST_CHECK(!queue.isEmpty());
	queue.abortFirstOfType(MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(!queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(can_abort_all_of_type)
{
	MessageQueue queue;
	MessageQueue::MessagePtr message1(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_1));

	queue.pushMessage(message1);
	queue.abortAllOfType(MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(queue.isEmpty());

	queue.pushMessage(message1);
	queue.pushMessage(message1);
	queue.abortAllOfType(MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(queue.isEmpty());

	MessageQueue::MessagePtr message2(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_2));
	queue.pushMessage(message2);

	BOOST_CHECK(!queue.isEmpty());
	queue.abortAllOfType(MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(!queue.isEmpty());
}

struct CountForEach
{
	CountForEach(int * counter) :
		m_counter(counter)
	{
	}

	void operator()(const MessageQueue::MessagePtr & )
	{
		++(*m_counter);
	}

	int * m_counter;
};

BOOST_AUTO_TEST_CASE(for_each_message_and_flush)
{
	int counter = 0;
	MessageQueue::ForEachFunction countObject = CountForEach(&counter);

	MessageQueue queue;
	MessageQueue::MessagePtr message1(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_1));
	MessageQueue::MessagePtr message2(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_2));

	queue.pushMessage(message1);
	queue.pushMessage(message2);

	queue.forEachAndClear(countObject);

	BOOST_CHECK_EQUAL(2, counter);

	queue.forEachAndClear(countObject);

	BOOST_CHECK_EQUAL(2, counter);
}

struct LoopMessageCounter
{
	LoopMessageCounter(int * counter, MessageQueue & queue) :
		m_counter(counter),
		m_queue(queue)
	{
	}

	void operator()(const MessageQueue::MessagePtr & message)
	{
		if (*m_counter == 0)
		{
			m_queue.pushMessage(message);
		}
		++(*m_counter);
	}

	int * m_counter;
	MessageQueue & m_queue;
};

struct NoopFunction
{
	void operator()(const MessageQueue::MessagePtr & message)
	{
	}
};

BOOST_AUTO_TEST_CASE(for_each_clears_queue)
{
	MessageQueue queue;
	MessageQueue::MessagePtr message1(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_1));
	MessageQueue::MessagePtr message2(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_2));

	queue.pushMessage(message1);

	queue.forEachAndClear(NoopFunction());
	BOOST_CHECK(queue.isEmpty());

	queue.forEachAndClear(NoopFunction());
	BOOST_CHECK(queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(for_each_acts_on_frozen_queue)
{
	MessageQueue queue;
	MessageQueue::MessagePtr message1(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_1));
	MessageQueue::MessagePtr message2(new Gnoll::Core::CMessage(MESSAGE_TYPENAME_2));

	int counter = 0;
	MessageQueue::ForEachFunction countObject = LoopMessageCounter(&counter, queue);

	queue.pushMessage(message1);
	queue.forEachAndClear(countObject);

	BOOST_CHECK_EQUAL(1, counter);

	queue.forEachAndClear(countObject);

	BOOST_CHECK_EQUAL(2, counter);

	queue.forEachAndClear(countObject);
	BOOST_CHECK_EQUAL(2, counter);
}

BOOST_AUTO_TEST_SUITE_END()

