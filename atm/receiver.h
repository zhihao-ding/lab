#ifndef receiver_h
#define receiver_h

#include "sender.h"
#include "dispatcher.h"

namespace messaging
{
    class receiver
	{
	public:
		operator sender() {
		    return sender(&q);
		}

		dispatcher wait() {
		    return dispatcher(&q);
		}

	private:
		queue q;
	};
}

#endif /* receiver_h */
