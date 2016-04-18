#ifndef dispatcher_h
#define dispatcher_h

#include "message_queue.h"

namespace messaging
{
	template<typename PreviousDispatcher,
		typename Message,
		typename Function>
	class TemplateDispatcher
	{
	public:
		TemplateDispatcher(TemplateDispatcher const&)=delete;
		TemplateDispatcher& operator=(TemplateDispatcher const&)=delete;

		TemplateDispatcher(TemplateDispatcher&& other):
			q_ptr(other.q_ptr), prev_ptr(other.prev_ptr), f(std::move(other.f)),
			chained(other.chained)
		{
		    other.chained = true;
		}

		TemplateDispatcher(queue *q_ptr_, PreviousDispatcher *prev_ptr, Function&& f_):
			q_ptr(q_ptr_), prev_ptr(prev_ptr_), f(std::forward<Function>(f_)),
			chained(false)
		{
		    prev_ptr_->chained = true;
		}

		template<typename OtherMsg, typename OtherFunc>
		TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
		handle(OtherFunc&& of)
		{
             return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(
					 q_ptr, this, std::forward<OtherFunc>(of));
		}

		~TemplateDispatcher() noexcept(false)
		{
		    if (!chained) wait_and_dispatch();
		}

	private:
		template<typename Dispatcher,
			typename OtherMsg,
			typename OtherFunc>
		friend class TemplateDispatcher;

		void wait_and_dispatch()
		{
		    for (;;) {
			    auto msg = q_ptr->wait_and_pop();
				if (dispatch(msg))
					break;
			}
		}

		bool dispatch(std::shared_ptr<message_base> const& msg)
		{
		    wrapped_message<Message> *wrapper = \
				dynamic_cast<wrapped_message<Message>*>(msg.get());
			if (wrapper != nullptr) {
			    f(wrapper->contents);
				return true;
			} else {
			    return prev_ptr->dispatch(msg);
			}
		}

	private:
		queue *q_ptr;
		PreviousDispatcher *prev_ptr;
		Function f;
		bool chained;
	};

	class close_queue {};

	class dispatcher
	{
	public:
		dispatcher(dispatcher const&)=delete;
		dispatcher& operator=(dispatcher const&)=delete;

		dispatcher(dispatcher&& other):
			q_ptr(other.q_ptr), chained(other.chained)
		{
		    other.chained = true;
		}

		explicit dispatcher(queue *q_ptr_):
			q_ptr(q_ptr_), chained(false)
		{}

		template<typename Message,typename Function>
		TemplateDispatcher<dispatcher, Message, Function>
		handle(Function&& f)
		{
		    return TemplateDispatcher<dispatcher, Message, Function>(
					q_ptr, this, std::forward<Function>(f));
		}

		~dispatcher() noexcept(false)
		{
		    if (!chained) wait_and_dispatch();
		}

	private:
		template<typename Dispatcher,
			typename Message,
			typename Function>
		friend class TemplateDispatcher;
	
		void wait_and_dispatch()
		{
		    for(;;)
			{
			    dispatch(q_ptr->wait_and_pop());
			}
		}

		bool dispatch(std::shared_ptr<message_base> const& msg)
		{
			if (dynamic_cast<wrapped_message<close_queue>*>(msg.get()))
			{
			    throw close_queue();
			}
            return false;
		}

	private:
		queue *q_ptr;
		bool chained;
	};

}

#endif /* dispatcher_h */
