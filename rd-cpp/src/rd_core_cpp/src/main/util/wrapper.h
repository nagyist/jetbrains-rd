//
// Created by jetbrains on 05.02.2019.
//

#ifndef RD_CPP_WRAPPER_H
#define RD_CPP_WRAPPER_H

#include "optional.hpp"

#include <type_traits>
#include <memory>
#include <utility>

namespace rd {
	template<typename T>
	class Wrapper;

	template<typename T, typename R = void>
	struct helper {
		using value_or_wrapper_type = T;
		using opt_or_wrapper_type = tl::optional<T>;
		using property_storage = tl::optional<T>;
		using raw_type = T;
	};

	template<typename T>
	struct helper<T, typename std::enable_if<std::is_abstract<T>::value>::type> {
		using value_or_wrapper_type = Wrapper<T>;
		using opt_or_wrapper_type = Wrapper<T>;
		using property_storage = Wrapper<T>;
		using raw_type = T;
	};

	template<typename T>
	struct helper<Wrapper<T>> {
		using value_or_wrapper_type = Wrapper<T>;
		using opt_or_wrapper_type = Wrapper<T>;
		using property_storage = Wrapper<Wrapper<T>>;
		using raw_type = T;
	};

	template<typename T>
	using value_or_wrapper = typename helper<T>::value_or_wrapper_type;

	template<typename T>
	using opt_or_wrapper = typename helper<T>::opt_or_wrapper_type;

	template<typename T>
	using property_storage = typename helper<T>::property_storage;

	template<typename T>
	using raw_type = typename helper<T>::raw_type;

	template<typename T>
	using wrapper_storage = std::unique_ptr<T>;

	template<typename T>
	class Wrapper : private wrapper_storage<T> {
	private:
		template<typename>
		friend
		class Wrapper;

		using Base = wrapper_storage<T>;
//		std::unique_ptr<T> ptr;
	public:
		//region ctor/dtor

		Wrapper() = default;

		Wrapper(Wrapper const &) = delete;

		Wrapper &operator=(Wrapper const &) = delete;

		Wrapper(Wrapper &&) = default;

		Wrapper &operator=(Wrapper &&) = default;

		template<typename F, typename R = typename std::enable_if<!std::is_abstract<typename std::remove_reference<F>::type>::value>::type>
		Wrapper(F &&value) : Base(std::make_unique<typename std::remove_reference<F>::type>(std::forward<F>(value))) {}

		template<typename F>
		Wrapper(std::unique_ptr<F> &&ptr) : Base(std::move(ptr)) {}
 
		template<typename U = T, typename R = typename std::enable_if<!std::is_abstract<typename std::remove_reference<U>::type>::value>::type>
		Wrapper(tl::optional<T> &&opt) {
			if (opt) {
				*this = std::make_unique<T>(std::move(*opt));
			}
		}

		template<typename R>
		static Wrapper<T> dynamic(Wrapper<R> &&w) {
			return Wrapper<T>{std::unique_ptr<T>(dynamic_cast<T *>(w.release()))};
		}
		//endregion

		constexpr T &operator*() &{
			return *static_cast<Base &>(*this);
		};

		constexpr T const &operator*() const &{
			return *static_cast<Base const &>(*this);
		};

		/*constexpr T &&operator*() &&{
			return *ptr.get();
		};*/

		template<typename R>
		operator Wrapper<R>() &&{
			return Wrapper<T>(static_cast<Base>(*this));
		}

		constexpr explicit operator bool() const noexcept {
			return Base::operator bool();
		}

		friend bool operator==(const Wrapper &lhs, const Wrapper &rhs) {
			return *lhs.get() == *rhs.get();
		}

		friend bool operator!=(const Wrapper &lhs, const Wrapper &rhs) {
			return !(rhs == lhs);
		}
	};

	namespace wrapper {
		template<typename T>
		decltype(auto) get(T &&w) {
			return std::forward<T>(w);
		}

		template<typename T>
		decltype(auto) get(T const &w) {
			return w;
		}

		template<typename T>
		T &get(Wrapper<T> &w) {
			return *w;
		}

		template<typename T>
		T const &get(Wrapper<T> const &w) {
			return *w;
		}

		template<typename T>
		T &&get(Wrapper<T> &&w) {
			return std::move(*w);
		}

		/*template<typename T>
	Wrapper<T> make_wrapper(T &&value) {
		return Wrapper<T>(std::move(value));
	}

	template<typename T>
	Wrapper<T> make_wrapper(std::unique_ptr<T> &&value) {
		return Wrapper<T>(std::move(value));
	}*/

		template<typename T>
		typename std::enable_if<!std::is_abstract<T>::value, T>::type unwrap(Wrapper<T> &&ptr) {
			return std::move(*ptr);
		}

		template<typename T>
		typename std::enable_if<std::is_abstract<T>::value, Wrapper<T>>::type unwrap(Wrapper<T> &&ptr) {
			return Wrapper<T>(std::move(ptr));
		}
	}
}

namespace std {
	template<typename T>
	struct hash<rd::Wrapper<T>> {
		size_t operator()(const rd::Wrapper<T> &value) const noexcept {
			return std::hash<T>()(*value);
		}
	};
}
#endif //RD_CPP_WRAPPER_H