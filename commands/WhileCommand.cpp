//
//  WhileCommand.cpp
//  jasl
//
//  Created by Ben Jones on 11/03/15
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#include "WhileCommand.hpp"
#include "../CommandInterpretor.hpp"
#include <string>

namespace jasl {

    WhileCommand::WhileCommand(Function &func_, OptionalOutputStream const &output)
    : Command(func_, output)
    {

    }

    bool WhileCommand::execute() 
    {

        // how many time should repeat loop for?
        auto extracted = VarExtractor::trySingleBoolExtraction(m_func.paramA);
        if(!extracted) {
            m_errorMessage = "while: problem extracting boolean expression";
            appendToOutput(m_errorMessage);
            return false;
        }
        return doLoop(*extracted);
    }

    bool WhileCommand::doLoop(bool const truth)
    {

        bool goodtogo = truth;

        while(goodtogo) {
            // parse inner functions
            std::vector<Function> innerFuncs;
            bool success = VarExtractor::tryAnyCast<std::vector<Function>>(innerFuncs, m_func.paramB);
            if (success) {
                success = parseCommands(innerFuncs);
            } else {
                m_errorMessage = "repeat: Error interpreting while's body";
                appendToOutput(m_errorMessage);
                return false;
            }

            // update loop condition
            auto extracted = VarExtractor::trySingleBoolExtraction(m_func.paramA);
            goodtogo = *extracted;
        }

        return true;
    }

    bool WhileCommand::parseCommands(std::vector<Function> &functions) 
    {
        CommandInterpretor ci;
        for(auto & f : functions) {
            (void)ci.interpretFunc(f, m_outputStream);
        }
        return true;
    }
}