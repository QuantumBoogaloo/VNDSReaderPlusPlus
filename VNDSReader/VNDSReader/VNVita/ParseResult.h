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

#include <memory>

#include "Commands.h"
#include "ParseException.h"

namespace VNVita
{
	class ParseResult
	{
	private:
		std::shared_ptr<Command> command;
		ParseException exception;
		bool hasExceptionValue;

	public:
		ParseResult() = default;

		ParseResult(std::shared_ptr<Command> command) :
			command(command), hasExceptionValue(false)
		{
		}

		ParseResult(std::shared_ptr<Command> command, const ParseException & exception) :
			command(command), exception(exception), hasExceptionValue(true)
		{
		}

		ParseResult(std::shared_ptr<Command> command, ParseException && exception) :
			command(command), exception(exception), hasExceptionValue(true)
		{
		}

		const std::shared_ptr<Command> & getCommand() const
		{
			return this->command;
		}

		bool hasException() const
		{
			return this->hasExceptionValue;
		}

		const ParseException & getException() const
		{
			return this->exception;
		}
	};
}
