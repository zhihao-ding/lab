#ifndef bank_h
#define bank_h

#include "receiver.h"

class bank_machine
{
public:
	bank_machine():
		balance(199)
	{}

	void done()
	{
	    get_sender().send(messaging::close_queue());
	}

	void run()
	{
	    try {
			for(;;) 
			{
			    incoming.wait()
					.handle<verify_pin>(
					[&](verify_pin const& msg){
					    if (msg.pin == "1937") {
						    msg.atm_queue.send(pin_verified());
						} else {
						    msg.atm_queue.send(pin_incorrect());
						}
					}).handle<withdraw>(
					[&](withdraw const& msg){
					    if (balance >= msg.amount) {
						    msg.atm_queue.send(withdraw_ok());
							balance -= msg.amount;
						} else {
						    msg.atm_queue.send(withdraw_denied());
						}
					}).handle<get_balance>(
					[&](get_balance const& msg){
					    msg.atm_queue.send(::balance(balance));
					}).handle<cancel_withdrawal>(
					[&](cancel_withdrawal const& msg){
					});
			}
		} catch (messaging::close_queue const&) {
		}
	}

	messaging::sender get_sender()
	{
	    return incoming;
	}

private:
	messaging::receiver incoming;
	unsigned balance;
};

#endif /* bank_h */

