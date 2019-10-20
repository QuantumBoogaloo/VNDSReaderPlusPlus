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
#include <utility>

#include "CharReader.h"

namespace VNVita
{
	template< typename CharT, typename Traits = std::char_traits<CharT> >
	class BasicStringCharReader : public BasicCharReader<CharT, Traits>
	{
	private:
		size_type nextIndex = 0;
		string_type string;

	public:
		BasicStringCharReader(const string_type & string) :
			string(string)
		{
		}

		BasicStringCharReader(string_type && string) :
			string(std::move(string))
		{
		}

		~BasicStringCharReader() = default;

		bool hasNext() const override
		{
			return (this->nextIndex < this->string.size());
		}

		value_type peekNext() const override
		{
			return this->string.at(this->nextIndex);
		}

		value_type readNext() override
		{
			const value_type result = this->peekNext();
			++this->nextIndex;
			return result;
		}

		string_type readRemaining() override
		{
			if(this->hasNext())
				return "";

			const auto index = this->nextIndex;

			this->nextIndex = this->string.size();

			return this->string.substr(index);
		}

		bool tryReadRemaining(string_type & result) override
		{
			if(!this->hasNext())
				return false;

			result = this->string.substr(this->nextIndex);

			this->nextIndex = this->string.size();

		}

		string_type readDelimited(const_pointer charset) override
		{
			if(!this->hasNext())
				return "";

			const size_type first = this->string.find_first_not_of(charset, this->nextIndex);
			if(first == string_type::npos)
			{
				this->nextIndex = this->string.size();
				return "";
			}

			const size_type end = this->string.find_first_of(charset, first);
			if(end == string_type::npos)
			{
				this->nextIndex = this->string.size();
				return this->string.substr(first);
			}
			else
			{
				this->nextIndex = end;
				return this->string.substr(first, (end - first));
			}
		}

		bool tryReadDelimited(string_type & output, const_pointer charset) override
		{
			if(!this->hasNext())
				return false;

			const size_type first = this->string.find_first_not_of(charset, this->nextIndex);
			if(first == string_type::npos)
			{
				this->nextIndex = this->string.size();
				return false;
			}

			const size_type end = this->string.find_first_of(charset, first);
			if(end == string_type::npos)
			{
				this->nextIndex = this->string.size();
				output = this->string.substr(first);
			}
			else
			{
				this->nextIndex = end;
				output = this->string.substr(first, (end - first));
			}

			return true;
		}
	};

	using StringCharReader = BasicStringCharReader<char>;
}
