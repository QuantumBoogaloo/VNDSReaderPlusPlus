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
	class BasicIStreamCharReader : public BasicCharReader<CharT, Traits>
	{
	private:
		std::istream & inputStream;

	public:
		BasicIStreamCharReader(std::istream & inputStream) :
			inputStream(inputStream)
		{
		}

		~BasicIStreamCharReader() = default;

		bool hasNext() const override
		{
			return !traits_type::eq_int_type(this->inputStream.peek(), traits_type::eof());
		}

		value_type peekNext() const override
		{
			return traits_type::to_char_type(this->inputStream.peek());
		}

		value_type readNext() override
		{
			return traits_type::to_char_type(this->inputStream.get());
		}
	};

	using IStreamCharReader = BasicIStreamCharReader<char>;
}
