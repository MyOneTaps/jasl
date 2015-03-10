//
//  BlockCommand.cpp
//  lightlang
//
//  Created by Ben Jones on on 10/03/2015.
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#include "BlockCommand.hpp"
#include "../CommandInterpretor.hpp"
#include "../VarExtractor.hpp"


namespace lightlang {

    BlockCommand::BlockCommand(Function &func_, OptionalOutputStream const &output)
    : Command(func_, output)
    {

    }

    bool BlockCommand::execute()
    {
        return interpretFunctionBody();
    }


    bool BlockCommand::interpretFunctionBody()
    {
        std::vector<Function> innerFuncs;
        bool success = VarExtractor::tryAnyCast<std::vector<Function>>(innerFuncs, m_func.paramB);
        if (success) {
            success = parseCommands(innerFuncs);
        } else {
            m_errorMessage = "repeat: Error interpreting repeat's body";
            appendToOutput(m_errorMessage);
            return false;
        }
        return true;
    }

    bool BlockCommand::parseCommands(std::vector<Function> &functions) 
    {
        CommandInterpretor ci;
        for(auto & f : functions) {
            (void)ci.interpretFunc(f, m_outputStream);
        }
        return true;
    }
}
