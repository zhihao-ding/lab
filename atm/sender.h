#ifndef sender_h
#define sender_h

#include "message_queue.h"

namespace messaging
{
    class sender
	{
	public:
		sender():
			q_ptr(nullptr)
		{}

		explicit sender(queue *q_ptr_):
			q_ptr(q_ptr_)
		{}

		template<typename Message>
		void send(Message const& msg)
		{
		    if (q_ptr) q_ptr->push(msg);
		}

	private:
		queue *q_ptr;
	};
}

#endif /* sender_h */

