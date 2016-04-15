#ifndef _actor_h_
#define _actor_h_

#include <thread>
#include <string>
#include "threadsafe_queue.h"

struct message
{
    int id;
    std::string context;
};

class actor
{
public:
    actor() {}

    actor(actor const&)=delete;
    actor& operator=(actor const&)=delete;

    void send(message const& msg)
    {
        mailbox.push(msg);
    }

    message recv()
    {
        message msg;
        mailbox.wait_and_pop(msg);
        return msg;
    }

    void start()
    {
        std::thread t(&actor::run, this);
        t.detach();
    }

    void run()
    {
        while (true) {
            message msg = recv();
            std::cout << "actor #" << std::this_thread::get_id()
                      << ", process message #" << msg.id
                      << ", context: " << msg.context
                      << std::endl;
            if (msg.context == "exit")
                break;
        }
    }

private:
    threadsafe_queue<message> mailbox;
};

#endif // _actor_h_

