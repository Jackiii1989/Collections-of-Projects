#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include <ostream>
#include <type_traits>
#include <string_view>
#include <string>

using namespace std;
class Person;
namespace JSON {

	class Object {
	public:
		virtual ~Object() = default;

		virtual void writeJson(std::ostream &out) const = 0;
	};

	template <typename T>
	constexpr bool is_char_v = std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>;

	template <typename T>
	constexpr bool is_numeric = std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double>;

	template <typename T>
	constexpr bool is_string = std::is_array<T>::value && std::is_convertible<T, const char*>::value &&\
		!std::is_class<T>::value || std::is_same_v < T, std::basic_string<char> >;

	/*
	  --> error C2593: 'operator <<' is ambiguous:
	  it does not know what to do for a certain data type(not defined): temporary optin use constexpr in if statements
	*/

	template <typename T>
	constexpr std::ostream& findWhatToWrite(std::ostream &out, T const &val) {

		using type = typename std::remove_reference<T>::type;

		if constexpr (std::is_null_pointer<type>::value)
			out << "null";
		else if constexpr (std::is_pointer<type>::value)
			out << *val;
		else if constexpr (is_numeric<type>)
			out << val;
		else if constexpr (std::is_same<type, bool>::value)
			out << std::boolalpha << val;
		else if  constexpr (is_char_v<type> || is_string<type>)
			out << '"' << val << '"';
		return out;
	}

	void writeArray1(std::ostream &out) { // zero based variadic template
	}

	template <typename T>
	void writeValue(std::ostream &out, T const &val) {// 1 argument based variadic template

		using type = typename std::remove_reference<T>::type;
		using type2 = typename std::remove_pointer<type>::type;

		if constexpr (std::is_null_pointer<type>::value) { // any nullpointer references 
			out << "null";
		}
		else if constexpr ((1) && std::is_pointer<type>::value && std::is_class<type2>::value) { // pointer to class or reference
			writeObjectElement1(out, val);
		}
		else if constexpr (std::is_base_of<Object, type>::value) { // normal objects 
			writeObjectElement(out, val);
		}
		else if constexpr (!std::is_base_of<Object, type>::value) // primitive objects
		{
			findWhatToWrite(out, val);
		}

	}

	template <typename T>
	void writeArray1(std::ostream &out, T const &val) {
		writeValue(out, val);
	}

	template <typename T, typename... TArgs>
	void writeArray1(std::ostream &out, T const &val, TArgs &&...elems) {
		writeValue(out, val);
		out << ",";
		writeArray1(out, std::forward<TArgs>(elems)...);
	}

	template <typename... TArgs>
	void writeArray(std::ostream &out, TArgs &&...elems) {
		out << "[";
		writeArray1(out, std::forward<TArgs>(elems)...);
		out << "]";
	}
	/*-------------------------------------------------------------------------------------------------*/

	// link: https://github.com/isocpp/CppCoreGuidelines/issues/749 --> why is recursion in templates bad

	void writeObjectElement(std::ostream &out) { // zero based variadic template
	}

	template <typename T = Person >
	void writeObjectElement(std::ostream &out, T& p) {
		p.writeJson(out);
	}

	template <typename T = Person >
	void writeObjectElement1(std::ostream &out, T* p) {
		if (p == nullptr) {
			out << "null";
			return;
		}
		(p)->writeJson(out);
	}

	template <typename T>
	void writeObjectElement(std::ostream &out, std::string_view name, T const &value) {
		out << '\"' << name << "\": ";
		writeValue(out, value);
	}

	template < typename Tv, typename... TArgs>
	void writeObjectElement(std::ostream &out, std::string_view name, Tv const &value, TArgs &&...elems) {  // 2 oder mehr based variadic template
		writeObjectElement(out, name, value);
		out << ",";
		writeObjectElement(out, std::forward<TArgs>(elems)...);
	}

	template <typename... TArgs>
	void writeObject(std::ostream &out, TArgs &&...elems) {
		out << "{";
		writeObjectElement(out, std::forward<TArgs>(elems)...);
		out << "}";
	}
}

#endif