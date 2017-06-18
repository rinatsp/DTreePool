#pragma once

#include <atomic>
#include <cassert>
#include <limits>
#include <stdexcept>

namespace rigtorp {

	template <typename T> class MPMCQueue {
	const size_t default_capasity = 300000;
	private:
		static_assert(std::is_nothrow_copy_assignable<T>::value ||
			std::is_nothrow_move_assignable<T>::value,
			"T must be nothrow copy or move assignable");

		static_assert(std::is_nothrow_destructible<T>::value,
			"T must be nothrow destructible");

	public:
		explicit MPMCQueue(const size_t capacity)
			: capacity_(default_capasity),
			slots_(capacity > 0 ? new Slot[capacity + 2 * kPadding] : nullptr),
			head_(0), tail_(0) {
			if (capacity_ < 1) {
				throw std::invalid_argument("capacity < 1");
			}
			assert(alignof(MPMCQueue<T>) >= kCacheLineSize);
			assert(reinterpret_cast<char *>(&tail_) -
				reinterpret_cast<char *>(&head_) >=
				kCacheLineSize);
		}

		explicit MPMCQueue()
			: capacity_(default_capasity),
			slots_(new Slot[default_capasity + 2 * kPadding]),
			head_(0), tail_(0) {
			if (default_capasity < 1) {
				throw std::invalid_argument("default_capasity < 1");
			}
			assert(alignof(MPMCQueue<T>) >= kCacheLineSize);
			assert(reinterpret_cast<char *>(&tail_) -
				reinterpret_cast<char *>(&head_) >=
				kCacheLineSize);
		}
		~MPMCQueue() { delete[] slots_; }

		// non-copyable and non-movable
		MPMCQueue(const MPMCQueue &) = delete;
		MPMCQueue &operator=(const MPMCQueue &) = delete;

		template <typename... Args> void emplace(Args &&... args) noexcept {
			static_assert(std::is_nothrow_constructible<T, Args &&...>::value,
				"T must be nothrow constructible with Args&&...");
			auto const head = head_.fetch_add(1);
			auto &slot = slots_[idx(head)];
			while (turn(head) * 2 != slot.turn.load(std::memory_order_acquire))
				;
			slot.construct(std::forward<Args>(args)...);
			slot.turn.store(turn(head) * 2 + 1, std::memory_order_release);
		}

		template <typename... Args> bool try_emplace(Args &&... args) noexcept {
			static_assert(std::is_nothrow_constructible<T, Args &&...>::value,
				"T must be nothrow constructible with Args&&...");
			auto head = head_.load(std::memory_order_acquire);
			for (;;) {
				auto &slot = slots_[idx(head)];
				if (turn(head) * 2 == slot.turn.load(std::memory_order_acquire)) {
					if (head_.compare_exchange_strong(head, head + 1)) {
						slot.construct(std::forward<Args>(args)...);
						slot.turn.store(turn(head) * 2 + 1, std::memory_order_release);
						return true;
					}
				}
				else {
					auto const prevHead = head;
					head = head_.load(std::memory_order_acquire);
					if (head == prevHead) {
						return false;
					}
				}
			}
		}

		void push(const T &v) noexcept {
			static_assert(std::is_nothrow_copy_constructible<T>::value,
				"T must be nothrow copy constructible");
			emplace(v);
		}

		template <typename P,
			typename = typename std::enable_if<
			std::is_nothrow_constructible<T, P &&>::value>::type>
			void push(P &&v) noexcept {
			emplace(std::forward<P>(v));
		}

		bool try_push(const T &v) noexcept {
			static_assert(std::is_nothrow_copy_constructible<T>::value,
				"T must be nothrow copy constructible");
			return try_emplace(v);
		}

		template <typename P,
			typename = typename std::enable_if<
			std::is_nothrow_constructible<T, P &&>::value>::type>
			bool try_push(P &&v) noexcept {
			return try_emplace(std::forward<P>(v));
		}

		void pop(T &v) noexcept {
			auto const tail = tail_.fetch_add(1);
			auto &slot = slots_[idx(tail)];
			while (turn(tail) * 2 + 1 != slot.turn.load(std::memory_order_acquire))
				;
			v = slot.move();
			slot.destroy();
			slot.turn.store(turn(tail) * 2 + 2, std::memory_order_release);
		}

		bool try_pop(T &v) noexcept {
			auto tail = tail_.load(std::memory_order_acquire);
			for (;;) {
				auto &slot = slots_[idx(tail)];
				if (turn(tail) * 2 + 1 == slot.turn.load(std::memory_order_acquire)) {
					if (tail_.compare_exchange_strong(tail, tail + 1)) {
						v = slot.move();
						slot.destroy();
						slot.turn.store(turn(tail) * 2 + 2, std::memory_order_release);
						return true;
					}
				}
				else {
					auto const prevTail = tail;
					tail = tail_.load(std::memory_order_acquire);
					if (tail == prevTail) {
						return false;
					}
				}
			}
		}

	private:
		constexpr size_t idx(size_t i) const noexcept {
			return i % capacity_ + kPadding;
		}
		constexpr size_t turn(size_t i) const noexcept { return i / capacity_; }

		static constexpr size_t kCacheLineSize = 512;

		struct Slot {
			~Slot() noexcept {
				if (turn & 1) {
					destroy();
				}
			}

			template <typename... Args> void construct(Args &&... args) noexcept {
				static_assert(std::is_nothrow_constructible<T, Args &&...>::value,
					"T must be nothrow constructible with Args&&...");
				new (&storage) T(std::forward<Args>(args)...);
			}

			void destroy() noexcept {
				static_assert(std::is_nothrow_destructible<T>::value,
					"T must be nothrow destructible");
				reinterpret_cast<T *>(&storage)->~T();
			}

			T &&move() noexcept { return reinterpret_cast<T &&>(storage); }

			// Align to avoid false sharing between adjacent slots
			alignas(kCacheLineSize)std::atomic<size_t> turn = { 0 };
			typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
		};

		// Padding to avoid false sharing between slots_ and adjacent allocations
		static constexpr size_t kPadding = (kCacheLineSize - 1) / sizeof(Slot) + 1;

	private:
		const size_t capacity_;
		Slot *const slots_;

		// Align to avoid false sharing between head_ and tail_
		alignas(kCacheLineSize)std::atomic<size_t> head_;
		alignas(kCacheLineSize)std::atomic<size_t> tail_;

		// Padding to avoid adjacent allocations to share cache line with tail_
		char padding_[kCacheLineSize - sizeof(decltype(tail_))];
	};
}