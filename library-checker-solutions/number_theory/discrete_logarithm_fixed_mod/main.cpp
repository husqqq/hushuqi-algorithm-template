// Adapted from yosupo06/library-checker-problems (Apache-2.0).
// Problem: discrete_logarithm_fixed_mod; upstream commit: 04c8de378bab67be926325de2871f0babb8e6451.
#include <iostream>

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif

#ifndef LC_INLINE_8058F6337C4C161D44B2
#define LC_INLINE_8058F6337C4C161D44B2

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif


namespace numtheo {
	constexpr i64 modint_inner = -2000000000;
	template<class Derived, i64 P, bool _64 = false> class ModIntBase {
		/*
		lt 0 for dynamic, lt -1e9 for internal use
		occupied P shown below:
		1	dis_log.hpp > ord
		2	excrt.hpp > excrt(T, T, T, T)
		3	pollard_rho.hpp > miller_rabin
		4	pollard_rho.hpp > pollard_rho
		5	prim_root.hpp > is_prim_root
		6	square_decomp_2.hpp > sqdecomp2_m4e1
		*/
	public:
		using val_t = std::conditional_t<_64, u64, u32>;
		using mul_t = i128::up_t<val_t>;
	protected:
		val_t val;
		inline static val_t dyn_mod = 0;
	public:
		static void set_mod(val_t p) {
			static_assert(P < 0);
			dyn_mod = p;
		}
		static val_t mod() {
			if (P < 0) {
				return dyn_mod;
			}
			return P;
		}
		ModIntBase() : val(0) {}
		template<class T> ModIntBase(T v, bool need_mod = true) : val(
			static_cast<val_t>(need_mod ? (v < 0 ? mod() - (-v) % mod() : v % mod()) : v)
		) {}
		void read_by_mod() {
			std::string s;
			std::cin >> s;
			val = 0;
			for (char c : s) {
				val = (static_cast<mul_t>(val) * 10 + (c ^ 48)) % mod();
			}
		}
		friend std::istream &operator>>(std::istream &in, Derived &x) {
			in >> x.val;
			return in;
		}
		val_t value() const {
			return val;
		}
		Derived operator+(Derived x) const {
			Derived ret;
			ret.val = val + x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator+=(Derived x) {
			val += x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator-() const {
			Derived ret;
			ret.val = mod() - val;
			return ret;
		}
		Derived operator-(Derived x) const {
			Derived ret;
			ret.val = val + mod() - x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator-=(Derived x) {
			val += mod() - x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator*(Derived x) const {
			return static_cast<Derived>(static_cast<mul_t>(val) * x.val);
		}
		Derived &operator*=(Derived x) {
			val = static_cast<val_t>(static_cast<mul_t>(val) * x.val % mod());
			return static_cast<Derived&>(*this);
		}
		friend bool operator==(Derived x, Derived y) {
			return x.val == y.val;
		}
		friend bool operator!=(Derived x, Derived y) {
			return x.val != y.val;
		}
	};
	template<i64 P, bool _64> class ModIntPr : public ModIntBase<ModIntPr<P, _64>, P, _64> { // P prime
	private:
		using Base = ModIntBase<ModIntPr<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P, bool _64> class ModInt : public ModIntBase<ModInt<P, _64>, P, _64> {
	private:
		using Base = ModIntBase<ModInt<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P> using ModIntPr32 = ModIntPr<P, false>;
	template<i64 P> using ModIntPr64 = ModIntPr<P, true>;
	template<i64 P> using ModInt32 = ModInt<P, false>;
	template<i64 P> using ModInt64 = ModInt<P, true>;
}
#endif

#ifndef LC_INLINE_EB431F74FD523A2751A1
#define LC_INLINE_EB431F74FD523A2751A1

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif

#ifndef LC_INLINE_61A83145CB506E11AD75
#define LC_INLINE_61A83145CB506E11AD75

#include <cmath>
#include <numeric>
#include <optional>
#include <vector>

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif

#ifndef LC_INLINE_82BC8CB834151524212E
#define LC_INLINE_82BC8CB834151524212E

#include <array>
#include <bitset>
#include <random>

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif


namespace ds {
	const u64 hash_rnum = 0x137891695f3759df;
	u64 hash64(u64 x) {
		x ^= (x >> 31);
		x *= (hash_rnum ^ 1145141919810);
		x ^= (x << 29);
		x *= (hash_rnum ^ 0x1145141919810);
		x ^= (x >> 23);
		x *= (x ^ hash_rnum);
		x ^= (x << 37);
		return x;
	}
	template<class T> u64 hash(T x) {
		if (std::is_same_v<T, __u128>) {
			return hash64(static_cast<u64>(x)) ^ hash64(static_cast<u64>(x >> 64));
		} else {
			return hash64(x);
		}
	}
	template<class key_t, class val_t, u32 N = 4194304> class hash_table {
	private:
		u32 _size;
		std::bitset<N> occupied;
		std::vector<key_t> key;
		std::vector<val_t> val;
		u32 id(u64 x) const {
			return (x & (N - 1));
		}
		u32 key_pos(key_t k) const {
			u32 ret = id(hash(k));
			while (occupied[ret] && key[ret] != k) {
				ret = ((ret + 1) & (N - 1));
			}
			return ret;
		}
	public:
		hash_table() : key(N), val(N) {}
		u32 size() const {
			return _size;
		}
		bool exists(key_t k) const {
			return key[key_pos(k)] == k;
		}
		val_t &operator[](key_t k) {
			u32 ind = key_pos(k);
			if (occupied[ind] == false) {
				occupied[ind] = true;
				++_size;
			}
			key[ind] = k;
			return val[ind];
		}
	};
}
#endif

#ifndef LC_INLINE_8058F6337C4C161D44B2
#define LC_INLINE_8058F6337C4C161D44B2

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif


namespace numtheo {
	constexpr i64 modint_inner = -2000000000;
	template<class Derived, i64 P, bool _64 = false> class ModIntBase {
		/*
		lt 0 for dynamic, lt -1e9 for internal use
		occupied P shown below:
		1	dis_log.hpp > ord
		2	excrt.hpp > excrt(T, T, T, T)
		3	pollard_rho.hpp > miller_rabin
		4	pollard_rho.hpp > pollard_rho
		5	prim_root.hpp > is_prim_root
		6	square_decomp_2.hpp > sqdecomp2_m4e1
		*/
	public:
		using val_t = std::conditional_t<_64, u64, u32>;
		using mul_t = i128::up_t<val_t>;
	protected:
		val_t val;
		inline static val_t dyn_mod = 0;
	public:
		static void set_mod(val_t p) {
			static_assert(P < 0);
			dyn_mod = p;
		}
		static val_t mod() {
			if (P < 0) {
				return dyn_mod;
			}
			return P;
		}
		ModIntBase() : val(0) {}
		template<class T> ModIntBase(T v, bool need_mod = true) : val(
			static_cast<val_t>(need_mod ? (v < 0 ? mod() - (-v) % mod() : v % mod()) : v)
		) {}
		void read_by_mod() {
			std::string s;
			std::cin >> s;
			val = 0;
			for (char c : s) {
				val = (static_cast<mul_t>(val) * 10 + (c ^ 48)) % mod();
			}
		}
		friend std::istream &operator>>(std::istream &in, Derived &x) {
			in >> x.val;
			return in;
		}
		val_t value() const {
			return val;
		}
		Derived operator+(Derived x) const {
			Derived ret;
			ret.val = val + x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator+=(Derived x) {
			val += x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator-() const {
			Derived ret;
			ret.val = mod() - val;
			return ret;
		}
		Derived operator-(Derived x) const {
			Derived ret;
			ret.val = val + mod() - x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator-=(Derived x) {
			val += mod() - x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator*(Derived x) const {
			return static_cast<Derived>(static_cast<mul_t>(val) * x.val);
		}
		Derived &operator*=(Derived x) {
			val = static_cast<val_t>(static_cast<mul_t>(val) * x.val % mod());
			return static_cast<Derived&>(*this);
		}
		friend bool operator==(Derived x, Derived y) {
			return x.val == y.val;
		}
		friend bool operator!=(Derived x, Derived y) {
			return x.val != y.val;
		}
	};
	template<i64 P, bool _64> class ModIntPr : public ModIntBase<ModIntPr<P, _64>, P, _64> { // P prime
	private:
		using Base = ModIntBase<ModIntPr<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P, bool _64> class ModInt : public ModIntBase<ModInt<P, _64>, P, _64> {
	private:
		using Base = ModIntBase<ModInt<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P> using ModIntPr32 = ModIntPr<P, false>;
	template<i64 P> using ModIntPr64 = ModIntPr<P, true>;
	template<i64 P> using ModInt32 = ModInt<P, false>;
	template<i64 P> using ModInt64 = ModInt<P, true>;
}
#endif

#ifndef LC_INLINE_F9CE07F3334595192902
#define LC_INLINE_F9CE07F3334595192902

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif

#ifndef LC_INLINE_4734A32E523D45E3876D
#define LC_INLINE_4734A32E523D45E3876D

#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif

#ifndef LC_INLINE_8058F6337C4C161D44B2
#define LC_INLINE_8058F6337C4C161D44B2

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif


namespace numtheo {
	constexpr i64 modint_inner = -2000000000;
	template<class Derived, i64 P, bool _64 = false> class ModIntBase {
		/*
		lt 0 for dynamic, lt -1e9 for internal use
		occupied P shown below:
		1	dis_log.hpp > ord
		2	excrt.hpp > excrt(T, T, T, T)
		3	pollard_rho.hpp > miller_rabin
		4	pollard_rho.hpp > pollard_rho
		5	prim_root.hpp > is_prim_root
		6	square_decomp_2.hpp > sqdecomp2_m4e1
		*/
	public:
		using val_t = std::conditional_t<_64, u64, u32>;
		using mul_t = i128::up_t<val_t>;
	protected:
		val_t val;
		inline static val_t dyn_mod = 0;
	public:
		static void set_mod(val_t p) {
			static_assert(P < 0);
			dyn_mod = p;
		}
		static val_t mod() {
			if (P < 0) {
				return dyn_mod;
			}
			return P;
		}
		ModIntBase() : val(0) {}
		template<class T> ModIntBase(T v, bool need_mod = true) : val(
			static_cast<val_t>(need_mod ? (v < 0 ? mod() - (-v) % mod() : v % mod()) : v)
		) {}
		void read_by_mod() {
			std::string s;
			std::cin >> s;
			val = 0;
			for (char c : s) {
				val = (static_cast<mul_t>(val) * 10 + (c ^ 48)) % mod();
			}
		}
		friend std::istream &operator>>(std::istream &in, Derived &x) {
			in >> x.val;
			return in;
		}
		val_t value() const {
			return val;
		}
		Derived operator+(Derived x) const {
			Derived ret;
			ret.val = val + x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator+=(Derived x) {
			val += x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator-() const {
			Derived ret;
			ret.val = mod() - val;
			return ret;
		}
		Derived operator-(Derived x) const {
			Derived ret;
			ret.val = val + mod() - x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator-=(Derived x) {
			val += mod() - x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator*(Derived x) const {
			return static_cast<Derived>(static_cast<mul_t>(val) * x.val);
		}
		Derived &operator*=(Derived x) {
			val = static_cast<val_t>(static_cast<mul_t>(val) * x.val % mod());
			return static_cast<Derived&>(*this);
		}
		friend bool operator==(Derived x, Derived y) {
			return x.val == y.val;
		}
		friend bool operator!=(Derived x, Derived y) {
			return x.val != y.val;
		}
	};
	template<i64 P, bool _64> class ModIntPr : public ModIntBase<ModIntPr<P, _64>, P, _64> { // P prime
	private:
		using Base = ModIntBase<ModIntPr<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P, bool _64> class ModInt : public ModIntBase<ModInt<P, _64>, P, _64> {
	private:
		using Base = ModIntBase<ModInt<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P> using ModIntPr32 = ModIntPr<P, false>;
	template<i64 P> using ModIntPr64 = ModIntPr<P, true>;
	template<i64 P> using ModInt32 = ModInt<P, false>;
	template<i64 P> using ModInt64 = ModInt<P, true>;
}
#endif


namespace numtheo {
	template<class T> bool miller_rabin(T x) {
		using MI = ModInt<modint_inner + 3, std::is_same_v<T, u64>>; // mod : x
		MI::set_mod(x);
		if (x == 1) {
			return false;
		}
		if (!(x & 1)) {
			return x == 2;
		}
		T t = (std::is_same_v<T, u64> ? __builtin_ctzll(x - 1) : __builtin_ctz(x - 1)), u = (x - 1) >> t;
		std::vector<T> a_list;
		if (std::is_same_v<T, u64>) {
			a_list = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
		} else {
			a_list = {2, 7, 61};
		}
		for (T a : a_list) {
			a %= x;
			if (a == 0) {
				continue;
			}
			if (std::gcd(a, x) != 1) {
				return false;
			}
			MI v = qpow(MI(a, false), u, MI(1, false));
			if (v.value() == 1) {
				continue;
			}
			bool nfound = false;
			for (u32 s = 0; s < t; ++s) {
				if (v.value() == x - 1) {
					nfound = true;
					break;
				}
				v *= v;
			}
			if (nfound == false) {
				return false;
			}
		}
		return true;
	}
	template<class T> T pollard_rho(T x) {
		using MI = ModInt<modint_inner + 4, std::is_same_v<T, u64>>; // mod : x
		MI::set_mod(x);
		if (!(x & 1)) {
			return 2;
		}
		MI c;
		std::conditional_t<std::is_same_v<T, u64>, std::mt19937_64, std::mt19937> rndc(std::random_device{}());
		while (true) {
			c = MI(rndc() % (x - 1) + 1, false);
			auto f = [c](MI _x)->MI {
				return _x * _x + c;
			};
			MI s = MI(0, false), t = MI(0, false), prod = MI(1, false);
			for (u32 k = 1;; ++k) {
				u32 counter = 0;
				for (u32 step = 0; !(step >> k); ++step) {
					t = f(t);
					prod *= s - t;
					++counter;
					if (counter == 128) {
						T g = std::gcd(prod.value(), x);
						if (g == x) {
							goto newc;
						}
						if (g != 1) {
							return g;
						}
						counter = 0;
					}
				}
				T g = std::gcd(prod.value(), x);
				if (g == x) {
					goto newc;
				}
				if (g != 1) {
					return g;
				}
				s = t;
			}
			newc:;
		}
	}
	template<class T> void prime_factors(T x, std::vector<T> &ret) {
		if (x <= 1) {
			return;
		}
		if (miller_rabin(x)) {
			ret.emplace_back(x);
			return;
		}
		T fact = pollard_rho(x);
		prime_factors(fact, ret);
		prime_factors(x / fact, ret);
		return;
	}
	template<class T> std::vector<std::pair<T, u32>> prime_factors(T x) {
		std::vector<T> ret1;
		prime_factors(x, ret1);
		std::sort(ret1.begin(), ret1.end());
		std::vector<std::pair<T, u32>> ret;
		for (T i : ret1) {
			if (ret.empty() || i != ret.back().first) {
				ret.emplace_back(i, 1);
			} else {
				++ret.back().second;
			}
		}
		return ret;
	}
}
#endif


namespace numtheo {
	template<class T> T phi(T x) {
		auto prf = prime_factors(x);
		for (auto i : prf) {
			x = x / i.first * (i.first - 1);
		}
		return x;
	}
}
#endif


namespace numtheo {
	template<i64 P, bool _64>
	std::optional<std::conditional_t<_64, u64, u32>> dis_log(ModIntPr<P, _64> a, ModIntPr<P, _64> b) {
		using MIP = ModIntPr<P, _64>;
		using val_t = typename MIP::val_t;
		using mul_t = typename MIP::mul_t;
		u32 B = static_cast<u32>(std::sqrt(MIP::mod())) + 2;
		MIP a_to_y = 1;
		ds::hash_table<val_t, val_t> bay2y;
		for (u32 y = 0; y < B; ++y, a_to_y *= a) {
			bay2y[(b * a_to_y).value()] = y;
		}
		MIP a_to_B = a_to_y, a_to_B_to_x = a_to_B;
		for (u32 x = 1; x <= B; ++x, a_to_B_to_x *= a_to_B) {
			if (bay2y.exists(a_to_B_to_x.value()) == true) {
				return static_cast<mul_t>(B) * x - bay2y[a_to_B_to_x.value()];
			}
		}
		return std::nullopt;
	}
	template<i64 P, bool _64> std::vector<std::optional<std::conditional_t<_64, u64, u32>>>
	dis_logs(ModIntPr<P, _64> a, const std::vector<ModIntPr<P, _64>> &b) {
		using MIP = ModIntPr<P, _64>;
		using val_t = typename MIP::val_t;
		using mul_t = typename MIP::mul_t;
		u32 B = static_cast<u32>(std::sqrt(MIP::mod() / b.size())) + 2;
		u32 xlim = static_cast<u32>(MIP::mod() / B) + 3;
		MIP a_to_B = qpow(a, B, MIP(1, false)), a_to_B_to_x = a_to_B;
		ds::hash_table<val_t, val_t> aBx2x;
		for (u32 x = 1; x <= xlim; ++x, a_to_B_to_x *= a_to_B) {
			if (aBx2x.exists(a_to_B_to_x.value()) == false) {
				aBx2x[a_to_B_to_x.value()] = x;
			}
		}
		std::vector<std::optional<val_t>> ret(b.size(), std::nullopt);
		MIP a_to_y = 1;
		for (u32 y = 0; y < B; ++y, a_to_y *= a) {
			for (u32 i = 0; i < ret.size(); ++i) {
				val_t bayv = (b[i] * a_to_y).value();
				if (aBx2x.exists(bayv) == false) {
					continue;
				}
				val_t cura = static_cast<val_t>(static_cast<mul_t>(aBx2x[bayv]) * B - y);
				if (ret[i].has_value() == false) {
					ret[i] = cura;
				} else {
					ret[i] = std::min(ret[i].value(), cura);
				}
			}
		}
		return ret;
	}
}
#endif

#ifndef LC_INLINE_9ABB108847C684299789
#define LC_INLINE_9ABB108847C684299789

#include <numeric>
#include <vector>

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif


namespace numtheo {
	std::vector<u32> primes, mpf;
	u32 enumerated_prime;
	void euler_sieve(u32 N) {
		if (mpf.size() > N) {
			return;
		}
		primes.clear();
		mpf.resize(N + 1);
		std::iota(mpf.begin(), mpf.end(), 0);
		enumerated_prime = N;
		for (u32 i = 2; i <= N; ++i) {
			if (mpf[i] == i) {
				primes.emplace_back(i);
			}
			for (u32 j : primes) {
				u64 k = static_cast<u64>(i) * j;
				if (k > N) {
					break;
				}
				mpf[k] = j;
				if (i % j == 0) {
					break;
				}
			}
		}
	}
}
#endif

#ifndef LC_INLINE_8058F6337C4C161D44B2
#define LC_INLINE_8058F6337C4C161D44B2

#ifndef LC_INLINE_F79A70D2F8BCE9C7F13C
#define LC_INLINE_F79A70D2F8BCE9C7F13C

#include <algorithm>
#include <cstdint>
#include <string>
#include <iostream>
#include <type_traits>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using __u128 = __uint128_t;
using __i128 = __int128_t;

std::ostream &operator<<(std::ostream &out, __u128 x) {
	if (x == 0) {
		out << "0";
		return out;
	}
	std::string _o;
	while (x != 0) {
		_o += char((x % 10) | 48);
		x /= 10;
	}
	std::reverse(_o.begin(), _o.end());
	out << _o;
	return out;
}
std::ostream &operator<<(std::ostream &out, __i128 x) {
	if (x < 0) {
		out << "-";
		x *= -1;
	}
	out << static_cast<__u128>(x);
	return out;
}

namespace i128 {
	// type traits
	template<class T> struct make_unsigned {
		using type = std::make_unsigned_t<T>;
	};
	template<> struct make_unsigned<__int128_t> {
		using type = __uint128_t;
	};
	template<class T> using make_unsigned_t = typename make_unsigned<T>::type;
	template<class T> struct make_signed {
		using type = std::make_signed_t<T>;
	};
	template<> struct make_signed<__uint128_t> {
		using type = __int128_t;
	};
	template<class T> using make_signed_t = typename make_signed<T>::type;
	template<class T> struct up {
		using type = void;
	};
	template<> struct up<u8> {
		using type = u16;
	};
	template<> struct up<u16> {
		using type = u32;
	};
	template<> struct up<u32> {
		using type = u64;
	};
	template<> struct up<u64> {
		using type = __u128;
	};
	template<> struct up<i8> {
		using type = i16;
	};
	template<> struct up<i16> {
		using type = i32;
	};
	template<> struct up<i32> {
		using type = i64;
	};
	template<> struct up<i64> {
		using type = __i128;
	};
	template<class T> using up_t = typename up<T>::type;
}

template<class T, class U> T qpow(T x, U y, const T &mul_iden = 1) {
	T ret = mul_iden;
	while (y) {
		if (y & 1) {
			ret *= x;
		}
		x *= x;
		y >>= 1;
	}
	return ret;
}

#define __func_str__ static_cast<std::string>(__func__)
#endif


namespace numtheo {
	constexpr i64 modint_inner = -2000000000;
	template<class Derived, i64 P, bool _64 = false> class ModIntBase {
		/*
		lt 0 for dynamic, lt -1e9 for internal use
		occupied P shown below:
		1	dis_log.hpp > ord
		2	excrt.hpp > excrt(T, T, T, T)
		3	pollard_rho.hpp > miller_rabin
		4	pollard_rho.hpp > pollard_rho
		5	prim_root.hpp > is_prim_root
		6	square_decomp_2.hpp > sqdecomp2_m4e1
		*/
	public:
		using val_t = std::conditional_t<_64, u64, u32>;
		using mul_t = i128::up_t<val_t>;
	protected:
		val_t val;
		inline static val_t dyn_mod = 0;
	public:
		static void set_mod(val_t p) {
			static_assert(P < 0);
			dyn_mod = p;
		}
		static val_t mod() {
			if (P < 0) {
				return dyn_mod;
			}
			return P;
		}
		ModIntBase() : val(0) {}
		template<class T> ModIntBase(T v, bool need_mod = true) : val(
			static_cast<val_t>(need_mod ? (v < 0 ? mod() - (-v) % mod() : v % mod()) : v)
		) {}
		void read_by_mod() {
			std::string s;
			std::cin >> s;
			val = 0;
			for (char c : s) {
				val = (static_cast<mul_t>(val) * 10 + (c ^ 48)) % mod();
			}
		}
		friend std::istream &operator>>(std::istream &in, Derived &x) {
			in >> x.val;
			return in;
		}
		val_t value() const {
			return val;
		}
		Derived operator+(Derived x) const {
			Derived ret;
			ret.val = val + x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator+=(Derived x) {
			val += x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator-() const {
			Derived ret;
			ret.val = mod() - val;
			return ret;
		}
		Derived operator-(Derived x) const {
			Derived ret;
			ret.val = val + mod() - x.val;
			if (ret.val >= mod()) {
				ret.val -= mod();
			}
			return ret;
		}
		Derived &operator-=(Derived x) {
			val += mod() - x.val;
			if (val >= mod()) {
				val -= mod();
			}
			return static_cast<Derived&>(*this);
		}
		Derived operator*(Derived x) const {
			return static_cast<Derived>(static_cast<mul_t>(val) * x.val);
		}
		Derived &operator*=(Derived x) {
			val = static_cast<val_t>(static_cast<mul_t>(val) * x.val % mod());
			return static_cast<Derived&>(*this);
		}
		friend bool operator==(Derived x, Derived y) {
			return x.val == y.val;
		}
		friend bool operator!=(Derived x, Derived y) {
			return x.val != y.val;
		}
	};
	template<i64 P, bool _64> class ModIntPr : public ModIntBase<ModIntPr<P, _64>, P, _64> { // P prime
	private:
		using Base = ModIntBase<ModIntPr<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P, bool _64> class ModInt : public ModIntBase<ModInt<P, _64>, P, _64> {
	private:
		using Base = ModIntBase<ModInt<P, _64>, P, _64>;
		using Base::Base;
	public:
		using typename Base::val_t;
		using typename Base::mul_t;
		using Base::mod;
		using Base::set_mod;
	};
	template<i64 P> using ModIntPr32 = ModIntPr<P, false>;
	template<i64 P> using ModIntPr64 = ModIntPr<P, true>;
	template<i64 P> using ModInt32 = ModInt<P, false>;
	template<i64 P> using ModInt64 = ModInt<P, true>;
}
#endif


namespace farey_tech_hpp {
	template<i64 P, bool _64> struct aux {
		using val_t = std::conditional_t<_64, u64, u32>;
		inline static u32 cbrt_log, cbrt, cbrt2;
		inline static bool enable = false;
		inline static std::vector<std::pair<u32, u32>> far, pre, suc;
		static void preproc() {
			using MIP = numtheo::ModIntPr<P, _64>;
			if (enable == true) {
				return;
			}
			enable = true;
			cbrt_log = (31 ^ __builtin_clz(static_cast<u32>(std::cbrt(MIP::mod()) + 1))) + 1;
			cbrt = 1 << cbrt_log, cbrt2 = 1 << (cbrt_log << 1);
			far.assign(cbrt2 + 1, std::make_pair(0, 0));
			for (u32 p = 0; p <= cbrt; ++p) {
				for (u32 q = (p == 1 ? 1 : p + 1); q <= cbrt; ++q) {
					u32 cur = static_cast<u32>((static_cast<u64>(p) << (aux::cbrt_log << 1)) / q);
					if (aux::far[cur].second == 0) {
						aux::far[cur] = std::make_pair(p, q);
					}
				}
			}
			pre = far;
			for (auto it = pre.begin(); it != pre.end(); ++it) {
				if (it->second == 0) {
					*it = *std::prev(it);
				}
			}
			suc = far;
			for (auto it = std::prev(suc.end()); it != std::prev(suc.begin()); --it) {
				if (it->second == 0) {
					*it = *std::next(it);
				}
			}
		}
		static std::pair<u32, i32> get(val_t x) {
			using MIP = numtheo::ModIntPr<P, _64>;
			u32 cur = static_cast<u32>((static_cast<u64>(x) << (cbrt_log << 1)) / MIP::mod());
			u32 q;
			i32 t = static_cast<i32>(static_cast<i64>(x) * pre[cur].second - static_cast<i64>(MIP::mod()) * pre[cur].first);
			if (static_cast<u32>(std::abs(t)) <= cbrt2) {
				q = pre[cur].second;
			} else {
				q = suc[cur].second;
				t = static_cast<i32>(static_cast<i64>(x) * q - static_cast<i64>(MIP::mod()) * suc[cur].first);
			}
			return std::make_pair(q, t);
		}
		inline static std::vector<numtheo::ModIntPr<P, _64>> inv_v;
		inline static std::vector<val_t> ln_table;
		inline static numtheo::ModIntPr<P, _64> g;
	};
}

namespace numtheo {
	template<i64 P, bool _64> void fast_ln_preproc(ModIntPr<P, _64> g) {
		using MIP = ModIntPr<P, _64>;
		using aux = farey_tech_hpp::aux<P, _64>;
		aux::preproc();
		aux::g = g;
		// ln for <=sqrt primes
		aux::ln_table.resize(aux::cbrt2 + 1);
		u32 sqrtP = static_cast<u32>(std::sqrt(MIP::mod())) + 2;
		euler_sieve(sqrtP);
		std::vector<MIP> pr;
		for (u32 i : primes) {
			if (i <= sqrtP) {
				pr.emplace_back(i);
			} else {
				break;
			}
		}
		auto prln = dis_logs(g, pr);
		for (u32 i = 0; i < pr.size(); ++i) {
			if (prln[i].has_value() == false) {
				throw std::invalid_argument(__func_str__ + " : g is not a primitive root");
			}
			aux::ln_table[primes[i]] = prln[i].value();
		}
		aux::ln_table[1] = 0;
		// ln for <=sqrt
		for (u32 i = 4; i <= sqrtP; ++i) {
			if (mpf[i] == i) {
				continue;
			}
			aux::ln_table[i] = aux::ln_table[mpf[i]] + aux::ln_table[i / mpf[i]];
			if (aux::ln_table[i] >= MIP::mod() - 1) {
				aux::ln_table[i] -= MIP::mod() - 1;
			}
		}
		// ln for <=cbrt2
		for (u32 i = sqrtP + 1; i <= aux::cbrt2; ++i) {
			u32 k = MIP::mod() / i, r = MIP::mod() % i;
			aux::ln_table[i] = static_cast<typename MIP::val_t>((static_cast<u64>((MIP::mod() - 1) >> 1) + aux::ln_table[r] + (MIP::mod() - 1) - aux::ln_table[k]) % (MIP::mod() - 1));
		}
	}
	template<i64 P, bool _64> std::conditional_t<_64, u64, u32> fast_dis_ln(ModIntPr<P, _64> x) {
		using MIP = ModIntPr<P, _64>;
		using aux = farey_tech_hpp::aux<P, _64>;
		auto [q, t] = aux::get(x.value());
		return static_cast<typename MIP::val_t>((static_cast<u64>(t > 0 ? aux::ln_table[t] : ((MIP::mod() - 1) >> 1) + aux::ln_table[-t]) + (MIP::mod() - 1) - aux::ln_table[q]) % (MIP::mod() - 1));
	}
}
#endif


using MIP = numtheo::ModIntPr32<-1>;

int main() {
	u32 p;
	scanf("%u", &p);
	MIP::set_mod(p);
	u32 gval;
	scanf("%u", &gval);
	numtheo::fast_ln_preproc(MIP(gval));
	u32 n;
	scanf("%u", &n);
	while (n--) {
		u32 xval;
		scanf("%u", &xval);
		std::cout << numtheo::fast_dis_ln(MIP(xval)) << '\n';
	}
	return 0;
}
