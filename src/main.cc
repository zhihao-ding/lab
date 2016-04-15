#include <iostream>
#include "actor.h"

int main()
{
    actor at1, at2, at3;

    at1.start();
    at2.start();
    at3.start();

    int counter = 0;
    while (true)
    {
        message msg;
        msg.id = ++counter;
        std::cout << "> " << std::endl;
        std::cin >> msg.context;

        at1.send(msg);
        at2.send(msg);
        at3.send(msg);

        if (msg.context == "exit")
            break;
    }

    return 0;
}

