#pragma once

//
//  Copyright (C) 2019 Pharap (@Pharap)
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <cstddef>
#include <string>

namespace VNVita
{
	template< typename CharT, typename Traits = std::char_traits<CharT> >
	class BasicCharReader
	{
	public:
		using value_type = CharT;
		using reference = value_type &;
		using const_reference = const value_type &;
		using pointer = value_type *;
		using const_pointer = const value_type *;
		using traits_type = Traits;
		using size_type = std::size_t;
		using string_type = std::basic_string<CharT, Traits>;

	private:
		static bool isInSet(value_type c, const_pointer charset, size_type length)
		{
			for(size_type index = 0; index < length; ++index)
				if(traits_type::eq(c, charset[index]))
					return true;

			return false;
		}

	public:
		virtual ~BasicCharReader() = default;

		virtual bool hasNext() const = 0;

		virtual value_type peekNext() const = 0;

		virtual value_type readNext() = 0;

		virtual string_type readRemaining()
		{
			string_type result;

			while(this->hasNext())
				result += this->readNext();

			return result;
		}

		virtual bool tryReadRemaining(string_type & result)
		{
			if(!this->hasNext())
				return false;

			result = this->readRemaining();
		}

		string_type readUntil(value_type c)
		{
			while(true)
			{
				if(!this->hasNext())
					return false;

				const value_type next = this->peekNext();
				if(traits_type::eq(next, c))
					break;

				this->readNext();
			}

			string_type result;

			while(this->hasNext())
			{
				const value_type next = this->readNext();
				if(traits_type::eq(next, c))
					break;

				result += next;
			}

			return result;
		}

		bool tryReadUntil(string_type & output, value_type c)
		{
			while(true)
			{
				if(!this->hasNext())
					return false;

				const value_type next = this->peekNext();
				if(traits_type::eq(next, c))
					break;

				this->readNext();
			}

			string_type result;

			while(this->hasNext())
			{
				const value_type next = this->readNext();
				if(traits_type::eq(next, c))
					break;

				result += next;
			}

			output = std::move(result);

			return true;
		}

		string_type readDelimited(const_pointer charset)
		{
			const size_type charsetLength = traits_type::length(charset);

			while (true)
			{
				if(!this->hasNext())
					return "";

				const value_type next = this->peekNext();
				if(!isInSet(next, charset, charsetLength))
					break;

				this->readNext();
			}

			string_type result;

			while(this->hasNext())
			{
				const value_type next = this->readNext();
				if(isInSet(next, charset, charsetLength))
					break;

				result += next;
			}

			return result;
		}

		bool tryReadDelimited(string_type & output, const_pointer charset)
		{
			const size_type charsetLength = traits_type::length(charset);

			while(true)
			{
				if(!this->hasNext())
					return false;

				const value_type next = this->peekNext();
				if(!isInSet(next, charset, charsetLength))
					break;

				this->readNext();
			}

			string_type result;

			while(this->hasNext())
			{
				const value_type next = this->readNext();
				if(isInSet(next, charset, charsetLength))
					break;

				result += next;
			}

			output = std::move(result);

			return true;
		}
	};

	using CharReader = BasicCharReader<char>;
}