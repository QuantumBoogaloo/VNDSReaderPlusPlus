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

#include <string>

#include "Command.h"
#include "CommandVisitor.h"

namespace VNVita
{
	class PlaySoundCommand : public Command
	{
	private:
		static constexpr int defaultRepeats = 1;

	private:
		std::string path;
		int repeats;

	public:
		PlaySoundCommand(const std::string & path, int repeats = defaultRepeats) :
			path(path), repeats(repeats)
		{
		}

		PlaySoundCommand(std::string && path, int repeats = defaultRepeats) :
			path(path), repeats(repeats)
		{
		}

		const std::string & getPath() const
		{
			return this->path;
		}

		int getRepeats() const
		{
			return this->repeats;
		}

		void accept(CommandVisitor & visitor) override
		{
			visitor.visitPlaySoundCommand(*this);
		}
	};
}