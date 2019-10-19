#include <iostream>
#include <fstream>
#include <typeinfo>

#include "VNVita\Commands.h"
#include "VNVita\Visitors.h"
#include "VNVita\CharReaders.h"
#include "VNVita\Parser.h"
#include "VNVita\ParseException.h"

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

void ProcessFile(std::string path);

int main(int argumentCount, const char * arguments[])
{
	if(argumentCount < 1)
	{
		std::cout << "Usage: VNDSReader <path> ...\n";
		return EXIT_FAILURE;
	}

	for(int index = 0; index < argumentCount; ++index)
	{
		try
		{
			ProcessFile(arguments[index]);
		}
		catch(VNVita::ParseException & exception)
		{
			std::cerr << "Error: " << exception.what() << '\n';
		}
	}

	return EXIT_SUCCESS;
}

bool tryReplaceExtension(const std::string & input, std::string & result, const std::string & newExtension)
{
	const auto position = input.find('.');

	if(position == std::string::npos)
		return false;

	result = input;

	auto iterator = std::begin(result);
	std::advance(iterator, position);

	result.replace(iterator, std::end(result), newExtension);
	return true;
}

std::vector<VNVita::ParseResult> readResults(VNVita::Parser & parser)
{
	using VNVita::ParseResult;

	std::vector<ParseResult> results;

	ParseResult result;
	while(parser.tryParseNextCommand(result))
		results.push_back(result);

	return results;
}

std::vector<std::shared_ptr<VNVita::Command>> filterErroneousCommands(const std::vector<VNVita::ParseResult> & results)
{
	using VNVita::Command;
	using VNVita::IfCommand;
	using VNVita::FiCommand;
	
	std::vector<std::shared_ptr<Command>> commands;

	bool isInErroneousIfBlock = false;

	for(std::size_t index = 0; index < results.size(); ++index)
	{
		const auto & result = results[index];

		if(result.getCommand() == nullptr)
			continue;

		if(isInErroneousIfBlock)
		{
			const auto commandPointer = result.getCommand().get();
			const auto command = dynamic_cast<FiCommand *>(commandPointer);
			if(command != nullptr)
				isInErroneousIfBlock = false;
		}
		else if(result.hasException())
		{
			const auto commandPointer = result.getCommand().get();
			const auto command = dynamic_cast<IfCommand *>(commandPointer);
			if(command != nullptr)
				isInErroneousIfBlock = true;
		}
		else
		{
			commands.push_back(result.getCommand());
		}
	}

	return commands;
}

void ProcessFile(std::string path)
{
	using namespace VNVita;

	// Open input file
	std::ifstream inputFile(path);

	// Create parser
	Parser parser(std::make_shared<IStreamCharReader>(inputFile));
	
	// Read commands into vector
	auto results = readResults(parser);

	// Filter the commands
	auto commands = filterErroneousCommands(results);

	// Get output file path
	std::string outputPath;
	if(!tryReplaceExtension(path, outputPath, ".vnvita"))
		return;

	// Open output file
	std::ofstream outputFile(outputPath);

	// Create formatter
	CommandFormatVisitor formatter(outputFile);

	// Format commands
	for(const auto & command : commands)
		formatter.visit(*command);
}