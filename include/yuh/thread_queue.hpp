#include <queue>
#include <type_traits>
#include <thread>
#include <mutex>


namespace yuh
{
	template<typename T, typename Container = std::deque<T>>
	class thread_queue
	{
	public:
		typedef thread_queue<T, Container> this_type;
		typedef std::queue<T, Container> Queue;
		typedef typename Queue::container_type container_type;
		typedef typename Queue::value_type value_type;
		typedef typename Queue::size_type size_type;
		typedef typename Queue::reference reference;
		typedef typename Queue::const_reference const_reference;

		thread_queue()
			: q()
		{	// construct with empty container
		}

		thread_queue(const this_type& rhs)
			: q(rhs.q)
		{	// construct by copying rhs container
		}

		explicit thread_queue(const Container& cont)
			: q(cont)
		{	// construct by copying specified container
		}

		explicit thread_queue(const Queue& que)
			: q(que)
		{	// construct by copying specified queue
		}

		this_type& operator=(const Queue& rhs)
		{	// assign by copying rhs
			q = rhs;
			return (*this);
		}

		this_type& operator=(const this_type& rhs)
		{	// assign by copying rhs
			std::lock_guard<std::mutex> lock(mtx_);
			q = rhs.q;
			return (*this);
		}

#if _HAS_CPP0X
		template<class Alloc>
		explicit thread_queue(const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(al)
		{	// construct with empty container, allocator
		}

		template<class Alloc>
		thread_queue(const Container& cont, const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(cont, al)
		{	// construct by copying specified container, allocator
		}

		template<class Alloc>
		thread_queue(const Queue& que, const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(que, al)
		{	// construct by copying specified queue, allocator
		}

		template<class Alloc>
		thread_queue(const this_type& rhs, const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(rhs.q, al)
		{	// construct by copying rhs container, allocator
		}
#endif /* _HAS_CPP0X */

		thread_queue(this_type&& rhs)
			: q(_STD move(rhs.q))
		{	// construct by moving rhs
		}

		explicit thread_queue(Container&& cont)
			: q(_STD move(cont))
		{	// construct by moving specified container
		}

		explicit thread_queue(Queue&& que)
			: q(_STD move(que))
		{	// construct by moving specified queue
		}

		template<class Alloc>
		thread_queue(Container&& cont, const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(_STD move(cont), al)
		{	// construct by moving specified container, allocator
		}

		template<class Alloc>
		thread_queue(Queue&& que, const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(_STD move(que), al)
		{	// construct by moving specified container, allocator
		}

		template<class Alloc>
		thread_queue(this_type&& rhs, const Alloc& al,
			typename std::enable_if<std::uses_allocator<Container, Alloc>::value,
			void>::type ** = 0)
			: q(_STD move(rhs.q), al)
		{	// construct by moving rhs container, allocator
		}

		this_type& operator=(Queue&& rhs)
		{	// assign by moving rhs
			std::lock_guard<std::mutex> lock(mtx_);
			q = _STD move(rhs);
			return (*this);
		}

		this_type& operator=(this_type&& rhs)
		{	// assign by moving rhs
			std::lock_guard<std::mutex> lock(mtx_);
			q = _STD move(rhs.q);
			return (*this);
		}

		void push(value_type&& val)
		{	// insert element at beginning
			std::lock_guard<std::mutex> lock(mtx_);
			q.push(_STD move(val));
		}
#ifdef _MSC_VER
#define _QUEUE_EMPLACE( \
	TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, X1, X2, X3, X4) \
	TEMPLATE_LIST(_CLASS_TYPE) \
	void emplace(LIST(_TYPE_REFREF_ARG)) \
		{	/* insert element at beginning */ \
		std::lock_guard<std::mutex> lock(mtx_); \
		q.emplace(LIST(_FORWARD_ARG)); \
		}

		_VARIADIC_EXPAND_0X(_QUEUE_EMPLACE, , , , )
#undef _QUEUE_EMPLACE
#endif
			bool empty() const
		{	// test if queue is empty
			std::lock_guard<std::mutex> lock(mtx_);
			return (q.empty());
		}

		size_type size() const
		{	// return length of queue
			std::lock_guard<std::mutex> lock(mtx_);
			return (q.size());
		}

		reference front()
		{	// return first element of mutable queue
			std::lock_guard<std::mutex> lock(mtx_);
			return (q.front());
		}

		const_reference front() const
		{	// return first element of nonmutable queue
			std::lock_guard<std::mutex> lock(mtx_);
			return (q.front());
		}

		reference back()
		{	// return last element of mutable queue
			std::lock_guard<std::mutex> lock(mtx_);
			return (q.back());
		}

		const_reference back() const
		{	// return last element of nonmutable queue
			std::lock_guard<std::mutex> lock(mtx_);
			return (q.back());
		}

		void push(const value_type& val)
		{	// insert element at beginning
			std::lock_guard<std::mutex> lock(mtx_);
			q.push(val);
		}

		void pop()
		{	// erase element at end
			std::lock_guard<std::mutex> lock(mtx_);
			q.pop();
		}

		const Container& _Get_container() const
		{	// get reference to container
			return (q._Get_container());
		}

		const Queue& _Get_queue() const
		{	// get reference to container
			return (q);
		}

		void swap(this_type& rhs)
		{	// exchange contents with rhs
			std::unique_lock<std::mutex> ll(mtx_, std::defer_lock);
			std::unique_lock<std::mutex> lr(rhs.mtx_, std::defer_lock);

			std::lock(ll, lr);

			q.swap(rhs.q);
		}


	protected:
		Queue q;
		mutable std::mutex mtx_;
	};
}
namespace std
{
	template<class T,
	class Container> inline
		void swap(yuh::thread_queue<T, Container>& lhs,
		yuh::thread_queue<T, Container>& rhs)
	{	// swap lhs and rhs queues
		lhs.swap(rhs);
	}
}
namespace yuh
{
	template<class T,
	class Container> inline
		bool operator==(const thread_queue<T, Container>& lhs,
		const thread_queue<T, Container>& rhs)
	{	// test for queue equality
		return (lhs._Get_queue() == rhs._Get_queue());
	}

	template<class T,
	class Container> inline
		bool operator!=(const thread_queue<T, Container>& lhs,
		const thread_queue<T, Container>& rhs)
	{	// test for queue inequality
		return (!(lhs == rhs));
	}

	template<class T,
	class Container> inline
		bool operator<(const thread_queue<T, Container>& lhs,
		const thread_queue<T, Container>& rhs)
	{	// test if lhs < rhs for queues
		return (lhs._Get_queue() < rhs._Get_queue());
	}

	template<class T,
	class Container> inline
		bool operator>(const thread_queue<T, Container>& lhs,
		const thread_queue<T, Container>& rhs)
	{	// test if lhs > rhs for queues
		return (rhs < lhs);
	}

	template<class T,
	class Container> inline
		bool operator<=(const thread_queue<T, Container>& lhs,
		const thread_queue<T, Container>& rhs)
	{	// test if lhs <= rhs for queues
		return (!(rhs < lhs));
	}

	template<class T,
	class Container> inline
		bool operator>=(const thread_queue<T, Container>& lhs,
		const thread_queue<T, Container>& rhs)
	{	// test if lhs >= rhs for queues
		return (!(lhs < rhs));
	}
}