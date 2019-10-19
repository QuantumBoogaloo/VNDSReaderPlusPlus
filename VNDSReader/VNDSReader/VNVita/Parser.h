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

#include <istream>
#include <memory>

#include "Commands.h"
#include "CharReaders.h"
#include "ParseException.h"
#include "ParseResult.h"

namespace VNVita
{
	class Parser
	{
	private:
		static constexpr const char * delimiters = " \t\r";

	private:
		std::shared_ptr<CharReader> reader;

	public:
		Parser(std::shared_ptr<CharReader> reader) :
			reader(reader)
		{
		}

		bool tryParseNextCommand(ParseResult & result)
		{
			std::string text;

			if(this->reader->tryReadDelimited(text, "\n"))
			{
				auto reader = StringCharReader(text);
				result = this->parseNextCommand(reader);
				return true;
			}

			return false;
		}

	private:
		ParseResult parseNextCommand(CharReader & reader)
		{
			std::string name;

			if(!reader.tryReadDelimited(name, delimiters))
				return std::make_shared<SkipCommand>();

			if(name == "bgload")
				return parseBackgroundLoadCommand(reader);

			if(name == "setimg")
				return parseSetImageCommand(reader);

			if(name == "sound")
				return parseSoundCommand(reader);

			if(name == "music")
				return parseMusicCommand(reader);

			if(name == "text")
				return parseTextCommand(reader);

			if(name == "choice")
				return this->parseChoiceCommand(reader);

			if(name == "setvar")
				return this->parseLocalVariableCommand(reader);

			if(name == "gsetvar")
				return this->parseGlobalVariableCommand(reader);

			if(name == "if")
				return this->parseIfCommand(reader);

			if(name == "fi")
				return std::make_shared<FiCommand>();

			if(name == "jump")
				return this->parseJumpCommand(reader);

			if(name == "delay")
				return this->parseDelayCommand(reader);

			if(name == "random")
				return this->parseRandomCommand(reader);

			if(name == "label")
				return this->parseLabelCommand(reader);

			if(name == "goto")
				return this->parseGoToCommand(reader);

			if(name == "cleartext")
				return std::make_shared<ClearTextCommand>();

			if(name == "endscript")
				return std::make_shared<EndScriptCommand>();

			return std::make_shared<SkipCommand>();
		}

		ParseResult parseBackgroundLoadCommand(CharReader & reader)
		{
			std::string path;
			if(!tryReadString(reader, path))
				return ParseResult(std::make_shared<BackgroundLoadCommand>(path), "Unable to parse <path>");

			int fadeTime;
			if(!tryReadInt(reader, fadeTime))
				return std::make_shared<BackgroundLoadCommand>(path);

			return std::make_shared<BackgroundLoadCommand>(path, fadeTime);
		}

		ParseResult parseSetImageCommand(CharReader & reader)
		{
			std::string path;
			if(!tryReadString(reader, path))
				return ParseResult(std::make_shared<SetImageCommand>(path, 0, 0), "Unable to parse <path>");

			int x;
			if(!tryReadInt(reader, x))
				return ParseResult(std::make_shared<SetImageCommand>(path, x, 0), "Unable to parse <x>");

			int y;
			if(!tryReadInt(reader, y))
				return ParseResult(std::make_shared<SetImageCommand>(path, x, y), "Unable to parse <y>");

			return std::make_shared<SetImageCommand>(path, x, y);
		}

		ParseResult parseSoundCommand(CharReader & reader)
		{
			std::string path;
			if(!tryReadString(reader, path))
				return ParseResult(std::make_shared<PlaySoundCommand>(path), "Unable to parse <path>");

			if(path == "~")
				return std::make_shared<StopSoundCommand>();

			int repeats;
			if(!tryReadInt(reader, repeats))
				return std::make_shared<PlaySoundCommand>(path);

			return std::make_shared<PlaySoundCommand>(path, repeats);
		}

		ParseResult parseMusicCommand(CharReader & reader)
		{
			std::string path;
			if(!tryReadString(reader, path))
				return ParseResult(std::make_shared<PlayMusicCommand>(path), "Unable to parse <path>");

			if(path == "~")
				return std::make_shared<StopMusicCommand>();

			return std::make_shared<PlayMusicCommand>(path);
		}

		ParseResult parseTextCommand(CharReader & reader)
		{
			std::string text;
			if(!tryReadString(reader, text))
				return std::make_shared<TextCommand>("", TextOption::AwaitInput);

			switch(text[0])
			{
			case '~':
				return std::make_shared<TextCommand>("", TextOption::None);
			case '!':
				return std::make_shared<AwaitInputCommand>();
			case '@':
				return std::make_shared<TextCommand>(text.substr(1), TextOption::None);
			}

			return std::make_shared<TextCommand>(text, TextOption::AwaitInput);
		}

		ParseResult parseChoiceCommand(CharReader & reader)
		{
			auto choices = std::vector<std::string>();

			for(auto choice = reader.readUntil('|'); choice.size() > 0; choice = reader.readUntil('|'))
				choices.push_back(std::move(choice));

			return std::make_shared<ChoiceCommand>(std::move(choices));
		}

		ParseResult parseLocalVariableCommand(CharReader & reader)
		{
			std::string left;
			if(!tryReadString(reader, left))
				return ParseResult(std::make_shared<SetLocalVariableCommand>(left, SetOperation::Assign, ""), "Unable to parse <left>");

			std::string op;
			if(!tryReadString(reader, op))
				return ParseResult(std::make_shared<SetLocalVariableCommand>(left, SetOperation::Assign, ""), "Unable to parse <operation>");

			if(op == "~")
				return std::make_shared<ClearLocalVariablesCommand>();

			SetOperation operation;
			if(!tryReadSetOperation(op, operation))
				return ParseResult(std::make_shared<SetLocalVariableCommand>(left, SetOperation::Assign, ""), "Unable to parse <operation>");

			std::string right;
			if(!tryReadString(reader, right))
				return ParseResult(std::make_shared<SetLocalVariableCommand>(left, operation, ""), "Unable to parse <right>");

			return std::make_shared<SetLocalVariableCommand>(left, operation, right);
		}

		ParseResult parseGlobalVariableCommand(CharReader & reader)
		{
			std::string left;
			if(!tryReadString(reader, left))
				return ParseResult(std::make_shared<SetGlobalVariableCommand>(left, SetOperation::Assign, ""), "Unable to parse <left>");

			std::string op;
			if(!tryReadString(reader, op))
				return ParseResult(std::make_shared<SetGlobalVariableCommand>(left, SetOperation::Assign, ""), "Unable to parse <operation>");

			if(op == "~")
				return std::make_shared<ClearGlobalVariablesCommand>();

			SetOperation operation;
			if(!tryReadSetOperation(op, operation))
				return ParseResult(std::make_shared<SetGlobalVariableCommand>(left, SetOperation::Assign, ""), "Unable to parse <operation>");

			std::string right;
			if(!tryReadString(reader, right))
				return ParseResult(std::make_shared<SetGlobalVariableCommand>(left, operation, ""), "Unable to parse <right>");

			return std::make_shared<SetGlobalVariableCommand>(left, operation, right);
		}

		ParseResult parseIfCommand(CharReader & reader)
		{
			std::string left;
			if(!tryReadString(reader, left))
				return ParseResult(std::make_shared<IfCommand>(left, IfOperation::Equals, ""), "Unable to parse <left>");

			std::string op;
			if(!tryReadString(reader, op))
				return ParseResult(std::make_shared<IfCommand>(left, IfOperation::Equals, ""), "Unable to parse <operation>");

			IfOperation operation;
			if(!tryReadIfOperation(op, operation))
				return ParseResult(std::make_shared<IfCommand>(left, IfOperation::Equals, ""), "Unable to parse <operation>");

			std::string right;
			if(!tryReadString(reader, right))
				return ParseResult(std::make_shared<IfCommand>(left, IfOperation::Equals, right), "Unable to parse <right>");

			return std::make_shared<IfCommand>(left, operation, right);
		}

		ParseResult parseJumpCommand(CharReader & reader)
		{
			std::string path;
			if(!tryReadString(reader, path))
				return ParseResult(std::make_shared<JumpCommand>(path), "Unable to parse <path>");

			std::string label;
			if(!tryReadString(reader, label))
				return std::make_shared<JumpCommand>(path);

			return std::make_shared<JumpCommand>(path, label);
		}

		ParseResult parseDelayCommand(CharReader & reader)
		{
			int time;
			if(!tryReadInt(reader, time))
				return ParseResult(std::make_shared<DelayCommand>(0), "Unable to parse <time>");

			return std::make_shared<DelayCommand>(time);
		}

		ParseResult parseRandomCommand(CharReader & reader)
		{
			std::string variable;
			if(!tryReadString(reader, variable))
				return ParseResult(std::make_shared<RandomCommand>(variable, 0, 0), "Unable to parse <variable>");

			int low;
			if(!tryReadInt(reader, low))
				return ParseResult(std::make_shared<RandomCommand>(variable, low, 0), "Unable to parse <low>");

			int high;
			if(!tryReadInt(reader, high))
				return ParseResult(std::make_shared<RandomCommand>(variable, low, high), "Unable to parse <high>");

			return std::make_shared<RandomCommand>(variable, low, high);
		}

		ParseResult parseLabelCommand(CharReader & reader)
		{
			std::string label;
			if(!tryReadString(reader, label))
				return ParseResult(std::make_shared<LabelCommand>(label), "Unable to parse <label>");

			return std::make_shared<LabelCommand>(label);
		}

		ParseResult parseGoToCommand(CharReader & reader)
		{
			std::string label;
			if(!tryReadString(reader, label))
				return ParseResult(std::make_shared<GoToCommand>(label), "Unable to parse <label>");

			return std::make_shared<GoToCommand>(label);
		}

	private:
		static bool tryReadString(CharReader & reader, std::string & result)
		{
			return reader.tryReadDelimited(result, delimiters);
		}

		static bool tryReadInt(CharReader & reader, int & result)
		{
			std::string text;
			if(!tryReadString(reader, text))
				return false;

			try
			{
				result = std::stoi(text);
				return true;
			}
			catch(...)
			{
				return false;
			}
		}

		static bool tryReadSetOperation(const std::string & op, SetOperation & operation)
		{
			if(op == "=")
			{
				operation = SetOperation::Assign;
				return true;
			}

			if(op == "+")
			{
				operation = SetOperation::Add;
				return true;
			}

			if(op == "-")
			{
				operation = SetOperation::Subtract;
				return true;
			}

			return false;
		}

		static bool tryReadIfOperation(const std::string & op, IfOperation & operation)
		{
			if(op == "==")
			{
				operation = IfOperation::Equals;
				return true;
			}

			if(op == "!=")
			{
				operation = IfOperation::NotEquals;
				return true;
			}

			if(op == "<")
			{
				operation = IfOperation::LessThan;
				return true;
			}

			if(op == ">")
			{
				operation = IfOperation::GreaterThan;
				return true;
			}

			if(op == "<=")
			{
				operation = IfOperation::LessThanEquals;
				return true;
			}

			if(op == ">=")
			{
				operation = IfOperation::GreaterThanEquals;
				return true;
			}

			return false;
		}
	};
}