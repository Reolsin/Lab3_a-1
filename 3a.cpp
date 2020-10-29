#include "Binary.h" //случайно указано не то название файла
#include <stdlib.h>
#include <algorithm>
#include <charconv>
#include <cstring>

void invert(char& c)
{
	c = c == '0' ? '1' : '0';
}

namespace Prog3a {
	std::ostream& Binary::print(std::ostream& s, const Binary& bi) {
		if (bi.arr[0] == '\0')
			s << "Binary number not found..." << std::endl;
		else
			s << bi.GetBin();
		return s;
	}

	std::istream& Binary::scan(std::istream& s, Binary& bi) {
		s.read(bi.arr, 15);
		return s;
	}

	void Binary::NormolizeArr() //странно что функция из private области нигде не используется
	{

			int j = 1;
			while (arr[j] == '0')
			{
				j++;
			}
			if (arr[j] == '\0')
			{
				j--;
				arr[0] = '0';
			}
			int i = 1;
			j--;
			for (; arr[i + j] != '\0'; i++)
			{
				arr[i] = arr[i + j];
			}
			arr[i] = '\0';
	}

	void Binary::SetDefaultArr() {
		arr = new char[SZ]{};
		for (int i = 0; i < SZ - 1; i++) {
			arr[i] = '0';
		}
	}

	Binary::Binary(){
		SetDefaultArr();
	}

	Binary::Binary(const long& num)
	{
		SetDefaultArr();

		char tmp[SZ - 1]{};
		auto [ptr, _] = std::to_chars(tmp, tmp + SZ, std::abs(num), 2); //я бы реализовал без auto
		
		const auto size = ptr - tmp;
		std::copy_n(tmp, size, arr + SZ - 1 - size);

		if (num < 0)
			arr[0] = '1';
	}

	Binary::Binary(const char* _arr)
	{
		if (IsCorrectArr(_arr))
		{
			SetDefaultArr();

			bool sgn = true;
			if (_arr[0] == '-' || _arr[0] == '+')
			{
				if (_arr[0] == '-')
					sgn = false;

				_arr++;
			}

			const auto len = std::strlen(_arr);
			std::copy_n(_arr, len, arr + SZ - 1 - len);
			arr[0] = sgn ? '0' : '1';
		}
		else
		{
			SetDefaultArr();
		}
	}

	Binary::Binary(const Binary& other)
	{
		SetDefaultArr();

		std::copy(arr, arr + SZ, other.arr); //использование std функций как раз в тех местах где это нужно 👍👍
	}

	Binary::Binary(Binary&& other) :
		arr(std::move(other.arr)) //👍👍
	{
		other.arr = nullptr;
	}

	bool Binary::IsCorrectArr(const char* _digits){
		int i = 0;
		if (_digits[0] == '-' || _digits[0] == '+')
		{
			i = 1;
		}
		while (_digits[i] != '\0')
		{
			if (!(_digits[i] == '0' || _digits[i] == '1'))
			{
				return false;
			}
			i++;
		}
		return true;
	}

	std::string quest() {
		std::stringstream s;
		s << "How do u want to enter the number? " << std::endl;
		s << "[1] Regular number." << std::endl;
		s << "[2] Binary number." << std::endl;
		return	s.str();
	}

	Binary Binary::Add(const Binary& arg)const //приятная и понятная функция 👍👍
	{
		auto a = GetAdditionalCode();
		auto b = arg.GetAdditionalCode();

		if (a.size() < b.size())
			std::swap(a, b);

		std::string res(a.length(), '0');
		bool carry = false;
		for (int i = a.length() - 1; i >= 0; --i)
		{
			res[i] = (a[i] - '0' + b[i] - '0' + (carry ? 1 : 0)) % 2 + '0';

			if ((a[i] == '1' && b[i] == '1') || (carry && (a[i] == '1' || b[i] == '1')))
				carry = true;
			else
				carry = false;

			if (!i && (a[0] - '0' + b[0] - '0' + (carry ? 1 : 0)) % 2 != (res[0] - '0'))
				throw std::overflow_error("Overflow in addition");
		}

		if (res[0] == '0')
			return res.c_str();

		Binary converted;
		converted.SetBinaryWithAdditionalCode(res.c_str());

		return converted;
	}

	void Binary::SetBinaryWithAdditionalCode(const char* _arr)
	{
		std::string str(_arr);

		bool carry = true;
		for (int i = str.length() - 1; carry; --i)
		{
			if (!carry)
				break;

			if (str[i] == '0')
				str[i] = '1';
			else
			{
				str[i] = '0';

				carry = false;
			}

			if (i == 1 && carry)
				throw std::overflow_error("Conversation from additional code overflow");
		}

		std::for_each(std::begin(str) + 1, std::end(str), &invert);

		std::copy(std::begin(str), std::end(str), arr);
	}

	Binary& Binary::operator=(const Binary& other){
		if (this != &other)
			std::copy(arr, arr + SZ, other.arr);

		return *this;
	}

	Binary& Binary::operator=(Binary&& other){
		if (this != &other)
		{
			delete[] arr;

			arr = std::move(other.arr);
			other.arr = nullptr;
		}

		return *this;
	}

	Binary& Binary::incr() {//я пишу такие функции c inline
		return *this = Add(1);
	}

	Binary& Binary::dicr() {
		return *this = Add(-1);
	}

	int Binary::Sign() const {
		return arr[0] == '0';
	}

	string Binary::GetAdditionalCode() const {
		std::string str(arr);
		if (Sign())
			return str;

		std::for_each(std::begin(str) + 1, std::end(str), &invert);

		bool carry = true;
		for (int i = SZ - 1; carry; --i)
		{
			if (!carry)
				break;

			if (str[i] == '0')
			{
				str[i] = '1';
				carry = false;
			}
			else
				str[i] = '0';

			if (i == 1 && carry)
				throw std::overflow_error("Conversation to additional code overflow");
		}

		return str;
	}
}
