//
//  ReleaseCommand.cpp
//  jasl
//
//  Created by Ben Jones on 15/11/15
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//

#include "ReleaseCommand.hpp"
#include "core/RegisterCommand.hpp"
#include <sstream>

bool jasl::ReleaseCommand::m_registered = 
registerCommand<jasl::ReleaseCommand>();

namespace jasl
{
    ReleaseCommand::ReleaseCommand(Function &func_,
                                   SharedCacheStack const &sharedCache,
                                   OptionalOutputStream const &output)
    : Command(func_, sharedCache, output)
    {
    }

    std::vector<std::string> ReleaseCommand::getCommandNames()
    {
        return {"release"};
    }

    bool ReleaseCommand::execute()
    {
        if(trySymbolExtraction()) { return true; }
        setLastErrorMessage("exec: couldn't parse");
        return false;
    }

    bool ReleaseCommand::trySymbolExtraction()
    {
        // Now try extracting a symbol
        std::string symbol;
        if(m_func.getValueA<std::string>(symbol, m_sharedCache)) {
        	m_sharedCache->eraseValue(symbol);
            return true;
        }
        return false;
    }
}
