#include <iostream>
#include <fstream>

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
	//ProcessFile("I:/Documents/Quantum Boogaloo/VNDS Notes/Scripts - Copy/cross channel/script/adstart.scr");

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

void ProcessFile(std::string path)
{
	using namespace VNVita;

	// Open input file
	std::ifstream inputFile(path);

	// Create parser
	Parser parser(std::make_shared<IStreamCharReader>(inputFile));
	
	// Read commands into vector
	std::vector<std::shared_ptr<Command>> commands;

	for(auto command = parser.parseNextCommand(); command != nullptr; command = parser.parseNextCommand())
		commands.push_back(std::move(command));

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