//
//  BlockCommand.cpp
//  jasl
//
//  Created by Ben Jones on on 10/03/2015.
//  Copyright (c) 2015-present Ben Jones. All rights reserved.
//

#include "BlockCommand.hpp"
#include "core/CommandInterpretor.hpp"
#include "core/RegisterCommand.hpp"
#include "commands/ParamExtractor.hpp"
#include "caching/VarExtractor.hpp"

namespace jasl {

    BlockCommand::BlockCommand(Function &func_, 
                               SharedCacheStack const &sharedCache, 
                               OptionalOutputStream const &output)
    : Command(func_, std::make_shared<CacheStack>(), output)
    {
    }

    bool BlockCommand::execute()
    {
        popParams(m_func.paramB, m_sharedCache);
        return interpretFunctionBody();
    }

    bool BlockCommand::interpretFunctionBody()
    {
        std::vector<Function> innerFuncs;
        bool success = VarExtractor::tryAnyCast<std::vector<Function>>(innerFuncs, m_func.paramC);
        if (success) {
            success = parseCommands(innerFuncs);
        } else {
            setLastErrorMessage("block: Error interpreting block's body");
            return false;
        }
        return true;
    }

    bool BlockCommand::parseCommands(std::vector<Function> &functions) 
    {
        CommandInterpretor ci;
        for(auto & f : functions) {
            (void)ci.interpretFunc(f, m_sharedCache, m_outputStream);
        }
        return true;
    }
}
