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

#include <ostream>

#include "../Commands.h"

namespace VNVita
{
	class CommandFormatVisitor : public CommandVisitor
	{
	private:
		static constexpr const char * defaultIndent = "\t";

	private:
		std::ostream & outputStream;
		int indentLevel = 0;
		std::string indentString = defaultIndent;

	public:
		CommandFormatVisitor(std::ostream & outputStream) :
			outputStream(outputStream)
		{
		}

		CommandFormatVisitor(std::ostream & outputStream, const std::string & indentString) :
			outputStream(outputStream), indentString(indentString)
		{
		}

		CommandFormatVisitor(std::ostream & outputStream, std::string && indentString) :
			outputStream(outputStream), indentString(indentString)
		{
		}

	private:
		void increaseIndent()
		{
			++this->indentLevel;
		}

		void decreaseIndent()
		{
			--this->indentLevel;
		}

		void writeIndent()
		{
			for(int i = 0; i < this->indentLevel; ++i)
				this->outputStream << this->indentString;
		}

		const char * toString(SetOperation operation)
		{
			switch(operation)
			{
			case SetOperation::Assign:
				return "=";
			case SetOperation::Add:
				return "+";
			case SetOperation::Subtract:
				return "-";
			default:
				return "";
			}
		}

		const char * toString(IfOperation operation)
		{
			switch(operation)
			{
			case IfOperation::Equals:
				return "==";
			case IfOperation::NotEquals:
				return "!=";
			case IfOperation::GreaterThan:
				return ">";
			case IfOperation::LessThan:
				return "<";
			case IfOperation::GreaterThanEquals:
				return ">=";
			case IfOperation::LessThanEquals:
				return "<=";
			default:
				return "";
			}
		}

	protected:
		virtual void visitSkipCommand(SkipCommand & skipCommand) override
		{
			this->writeIndent();
			this->outputStream << "#" << '\n';
			static_cast<void>(skipCommand);
		}

		virtual void visitEndScriptCommand(EndScriptCommand & endScriptCommand) override
		{
			this->writeIndent();
			this->outputStream << "endscript" << '\n';
			static_cast<void>(endScriptCommand);
		}


		virtual void visitBackgroundLoadCommand(BackgroundLoadCommand & backgroundLoadCommand) override
		{
			this->writeIndent();

			this->outputStream << "bgload " << backgroundLoadCommand.getPath();

			if(backgroundLoadCommand.getFadeTime() >= 0)
				this->outputStream << ' ' << backgroundLoadCommand.getFadeTime();

			this->outputStream << '\n';
		}


		virtual void visitSetImageCommand(SetImageCommand & setImageCommand) override
		{
			this->writeIndent();
			this->outputStream << "setimg " << setImageCommand.getPath() << ' ' << setImageCommand.getX() << ' ' << setImageCommand.getY() << '\n';
		}


		virtual void visitChoiceCommand(ChoiceCommand & choiceCommand) override
		{
			this->writeIndent();

			this->outputStream << "choice ";

			const auto & choices = choiceCommand.getChoices();
			for(std::size_t index = 0; index < choices.size(); ++index)
			{
				if(index > 0)
					this->outputStream << '|';
				this->outputStream << choices[index];
			}

			this->outputStream << '\n';
		}


		virtual void visitJumpCommand(JumpCommand & jumpCommand) override
		{
			this->writeIndent();

			this->outputStream << "jump " << jumpCommand.getPath();

			if(jumpCommand.getLabel().size() > 0)
				this->outputStream << ' ' << jumpCommand.getLabel();

			this->outputStream << '\n';
		}


		virtual void visitDelayCommand(DelayCommand & delayCommand) override
		{
			this->writeIndent();
			this->outputStream << "delay " << delayCommand.getTime() << '\n';
		}


		virtual void visitRandomCommand(RandomCommand & randomCommand) override
		{
			this->writeIndent();
			this->outputStream << "random " << randomCommand.getVariable() << ' ' << randomCommand.getLow() << ' ' << randomCommand.getHigh() << '\n';
		}


		virtual void visitLabelCommand(LabelCommand & labelCommand) override
		{
			this->writeIndent();
			this->outputStream << "label " << labelCommand.getLabel() << '\n';
		}


		virtual void visitGoToCommand(GoToCommand & gotoCommand) override
		{
			this->writeIndent();
			this->outputStream << "goto " << gotoCommand.getLabel() << '\n';
		}


		virtual void visitClearTextCommand(ClearTextCommand & clearTextCommand) override
		{
			this->writeIndent();
			this->outputStream << "cleartext" << '\n';
			static_cast<void>(clearTextCommand);
		}


		virtual void visitIfCommand(IfCommand & ifCommand) override
		{
			this->writeIndent();
			this->outputStream << "if " << ifCommand.getLeft() << ' ' << toString(ifCommand.getOperation()) << ' ' << ifCommand.getRight() << '\n';
			this->increaseIndent();
		}

		virtual void visitFiCommand(FiCommand & fiCommand) override
		{
			this->decreaseIndent();
			this->writeIndent();
			this->outputStream << "fi" << '\n';
			static_cast<void>(fiCommand);
		}


		virtual void visitAwaitInputCommand(AwaitInputCommand & awaitInputCommand) override
		{
			this->writeIndent();
			this->outputStream << "text !" << '\n';
			static_cast<void>(awaitInputCommand);
		}


		virtual void visitTextCommand(TextCommand & textCommand) override
		{
			this->writeIndent();

			this->outputStream << "text ";

			if(textCommand.getOption() == TextOption::None)
			{
				if(textCommand.getText().size() == 0)
					this->outputStream << '~';
				else
					this->outputStream << '@' << textCommand.getText();
			}
			else
			{
				this->outputStream << textCommand.getText();
			}

			this->outputStream << '\n';
		}


		virtual void visitSetLocalVariableCommand(SetLocalVariableCommand & setLocalVariableCommand) override
		{
			this->writeIndent();
			this->outputStream << "setvar " << setLocalVariableCommand.getLeft() << ' ' << toString(setLocalVariableCommand.getOperation()) << ' ' << setLocalVariableCommand.getRight() << '\n';
		}

		virtual void visitClearLocalVariablesCommand(ClearLocalVariablesCommand & clearLocalVariablesCommand) override
		{
			this->writeIndent();
			this->outputStream << "setvar ~ ~" << '\n';
		}


		virtual void visitSetGlobalVariableCommand(SetGlobalVariableCommand & setGlobalVariableCommand) override
		{
			this->writeIndent();
			this->outputStream << "gsetvar " << setGlobalVariableCommand.getLeft() << ' ' << toString(setGlobalVariableCommand.getOperation()) << ' ' << setGlobalVariableCommand.getRight() << '\n';
		}

		virtual void visitClearGlobalVariablesCommand(ClearGlobalVariablesCommand & clearGlobalVariablesCommand) override
		{
			this->writeIndent();
			this->outputStream << "gsetvar ~ ~" << '\n';
		}


		virtual void visitPlayMusicCommand(PlayMusicCommand & playMusicCommand) override
		{
			this->writeIndent();
			this->outputStream << "music " << playMusicCommand.getPath() << '\n';
		}

		virtual void visitStopMusicCommand(StopMusicCommand & stopMusicCommand) override
		{
			this->writeIndent();
			this->outputStream << "music ~" << '\n';
		}


		virtual void visitPlaySoundCommand(PlaySoundCommand & playSoundCommand) override
		{
			this->writeIndent();

			this->outputStream << "sound " << playSoundCommand.getPath();

			if(playSoundCommand.getRepeats() > 1) 
				this->outputStream << ' ' << playSoundCommand.getRepeats();

			this->outputStream << '\n';			
		}

		virtual void visitStopSoundCommand(StopSoundCommand & stopSoundCommand) override
		{
			this->writeIndent();
			this->outputStream << "sound ~" << '\n';
		}
	};
}