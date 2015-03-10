//
//  CommandInterpretor.cpp
//  lightlang
//
//  Created by Ben Jones 
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#include "CommandInterpretor.hpp"
#include "CommandParser.hpp"
#include "VarCache.hpp"
#include "commands/BlockCommand.hpp"
#include "commands/CallCommand.hpp"
#include "commands/CVarCommand.hpp"
#include "commands/EchoCommand.hpp"
#include "commands/IfCommand.hpp"
#include "commands/MathCommand.hpp"
#include "commands/RepeatCommand.hpp"
#include "commands/StartCommand.hpp"
#include "commands/VarCommand.hpp"

#include <boost/spirit/include/qi.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <iterator>

#define PROCESS_X_COMMAND(X)           \
    X c(func, outputStream);           \
    (void)c.execute();                 \
    errorMessage = c.getErrorMessage();


namespace lightlang {

    namespace {
        bool searchString(Function &func, std::string const &name)
        {
            return func.name.find(name) != std::string::npos;
        }
    }

    std::string
    CommandInterpretor::interpretFunc(Function &func,
                                      OptionalOutputStream const &outputStream) const
    {
        return doInterpretFunc(func, outputStream);
    }

    std::string
    CommandInterpretor::doInterpretFunc(Function &func,
                                        OptionalOutputStream const &outputStream) const
    {
        std::string errorMessage;
        if (searchString(func, "var")) {

            PROCESS_X_COMMAND(VarCommand);

        } else if(searchString(func, "m_")) {

            PROCESS_X_COMMAND(MathCommand);

        } else if(searchString(func, "int") ||
                  searchString(func, "double") ||
                  searchString(func, "bool")) {

            PROCESS_X_COMMAND(CVarCommand);

        } else if(searchString(func, "echo")) {

            PROCESS_X_COMMAND(EchoCommand);

        } else if(searchString(func, "if")) {

            PROCESS_X_COMMAND(IfCommand);

        } else if(searchString(func, "repeat")) {

            PROCESS_X_COMMAND(RepeatCommand);

        } else if(searchString(func, "block")) {
            
            PROCESS_X_COMMAND(BlockCommand);

        } else if(searchString(func, "start")) {
            
            PROCESS_X_COMMAND(StartCommand);

        } else if(searchString(func, "call")) {

            PROCESS_X_COMMAND(CallCommand);

        }
        if(errorMessage.empty()) { return std::string("Couldn't interpret function"); }
        return errorMessage;
    }
    
    std::string
    CommandInterpretor::parseAndInterpretSingleCommand(std::string const &cs,
                                                       OptionalOutputStream const &outputStream) const
    {

        using boost::spirit::ascii::space;
        typedef std::string::const_iterator iterator_type;
        typedef CommandParser<iterator_type> Parser;
        Parser functionGrammar;
        auto iter = std::begin(cs);
        auto end = std::end(cs);
        Function func;
        bool successfulParse = boost::spirit::qi::phrase_parse(iter, end, functionGrammar, space, func);
        if (successfulParse) {
            return doInterpretFunc(func, outputStream);
        } 
        return std::string("Unsuccessful parse");
    }

    std::vector<Function>
    CommandInterpretor::parseCommandFile(std::string const &path) const
    {
        using boost::spirit::ascii::space;
        typedef boost::spirit::istream_iterator iterator_type;
        typedef CommandParser<iterator_type> Parser;
        Parser functionGrammar;
        std::vector<Function> functions;

        // open file, disable skipping of whitespace
        std::ifstream in(path.c_str());
        in.unsetf(std::ios::skipws);

        // wrap istream into iterator
        boost::spirit::istream_iterator begin(in);
        boost::spirit::istream_iterator end;

        bool result;
        while (begin != end) {
            result = boost::spirit::qi::phrase_parse(begin,
                                                     end,
                                                     functionGrammar,
                                                     space,
                                                     functions);
            if (!result) {
                break;
            }
        }

        return functions;
    }

    std::vector<Function>
    CommandInterpretor::parseStringCollection(std::string const &stringCollection) const
    {

        // store the script in global static. Used to do block 
        // (lightlang name for subroutine) lookups
        VarCache::script = stringCollection;

        using boost::spirit::ascii::space;
        typedef std::string::const_iterator iterator_type;
        typedef CommandParser<iterator_type> Parser;
        Parser functionGrammar;
        auto iter = std::begin(stringCollection);
        auto end = std::end(stringCollection);
        std::vector<Function> functions;

        bool result;
        while (iter != end) {
            result = boost::spirit::qi::phrase_parse(iter,
                                                     end,
                                                     functionGrammar,
                                                     space,
                                                     functions);
            if (!result) {
                break;
            }
        }

        return functions;
    }
}