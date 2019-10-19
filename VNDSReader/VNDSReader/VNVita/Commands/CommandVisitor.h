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

#include "Command.h"

namespace VNVita
{
	class SkipCommand;
	class EndScriptCommand;
	class BackgroundLoadCommand;
	class SetImageCommand;
	class ChoiceCommand;
	class JumpCommand;
	class DelayCommand;
	class RandomCommand;
	class LabelCommand;
	class GoToCommand;
	class ClearTextCommand;
	class IfCommand;
	class FiCommand;
	class AwaitInputCommand;
	class TextCommand;
	class SetLocalVariableCommand;
	class ClearLocalVariablesCommand;
	class SetGlobalVariableCommand;
	class ClearGlobalVariablesCommand;
	class PlayMusicCommand;
	class StopMusicCommand;
	class PlaySoundCommand;
	class StopSoundCommand;
}

namespace VNVita
{
	class CommandVisitor
	{
	public:
		virtual ~CommandVisitor() = default;

		void visit(Command & command)
		{
			command.accept(*this);
		}

	protected:
		virtual void visitSkipCommand(SkipCommand & skipCommand) = 0;
		virtual void visitEndScriptCommand(EndScriptCommand & endScriptCommand) = 0;

		virtual void visitBackgroundLoadCommand(BackgroundLoadCommand & backgroundLoadCommand) = 0;

		virtual void visitSetImageCommand(SetImageCommand & setImageCommand) = 0;

		virtual void visitChoiceCommand(ChoiceCommand & choiceCommand) = 0;

		virtual void visitJumpCommand(JumpCommand & jumpCommand) = 0;

		virtual void visitDelayCommand(DelayCommand & delayCommand) = 0;

		virtual void visitRandomCommand(RandomCommand & randomCommand) = 0;

		virtual void visitLabelCommand(LabelCommand & labelCommand) = 0;

		virtual void visitGoToCommand(GoToCommand & gotoCommand) = 0;

		virtual void visitClearTextCommand(ClearTextCommand & clearTextCommand) = 0;

		virtual void visitIfCommand(IfCommand & ifCommand) = 0;
		virtual void visitFiCommand(FiCommand & fiCommand) = 0;

		virtual void visitAwaitInputCommand(AwaitInputCommand & awaitInputCommand) = 0;

		virtual void visitTextCommand(TextCommand & textCommand) = 0;

		virtual void visitSetLocalVariableCommand(SetLocalVariableCommand & setLocalVariableCommand) = 0;
		virtual void visitClearLocalVariablesCommand(ClearLocalVariablesCommand & clearLocalVariablesCommand) = 0;

		virtual void visitSetGlobalVariableCommand(SetGlobalVariableCommand & setGlobalVariableCommand) = 0;
		virtual void visitClearGlobalVariablesCommand(ClearGlobalVariablesCommand & clearGlobalVariablesCommand) = 0;

		virtual void visitPlayMusicCommand(PlayMusicCommand & playMusicCommand) = 0;
		virtual void visitStopMusicCommand(StopMusicCommand & stopMusicCommand) = 0;

		virtual void visitPlaySoundCommand(PlaySoundCommand & playSoundCommand) = 0;
		virtual void visitStopSoundCommand(StopSoundCommand & stopSoundCommand) = 0;

	protected:
		friend class SkipCommand;
		friend class EndScriptCommand;
		friend class BackgroundLoadCommand;
		friend class SetImageCommand;
		friend class ChoiceCommand;
		friend class JumpCommand;
		friend class DelayCommand;
		friend class RandomCommand;
		friend class LabelCommand;
		friend class GoToCommand;
		friend class ClearTextCommand;
		friend class IfCommand;
		friend class FiCommand;
		friend class AwaitInputCommand;
		friend class TextCommand;
		friend class SetLocalVariableCommand;
		friend class ClearLocalVariablesCommand;
		friend class SetGlobalVariableCommand;
		friend class ClearGlobalVariablesCommand;
		friend class PlayMusicCommand;
		friend class StopMusicCommand;
		friend class PlaySoundCommand;
		friend class StopSoundCommand;
	};
}