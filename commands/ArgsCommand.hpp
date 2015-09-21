//
//  ArgsCommand.cpp
//  jasl
//
//  Created by Ben Jones on 13/03/15
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include "../VarExtractor.hpp"
#include "../VarCache.hpp"
#include <sstream>
#include <cstdint>

namespace jasl
{
    class ArgsCommand : public Command
    {
    public:
        ArgsCommand(Function &func_,
                      OptionalOutputStream const &output = OptionalOutputStream())
        : Command(func_, output)
        {
        }

        bool execute() override
        {

            auto arg = VarExtractor::trySingleIntExtraction(m_func.paramA);
            if(!arg) {
                setLastErrorMessage("repeat: problem extracting integer");
                return false;
            }

            std::string argString;
            if(!m_func.getValueB<std::string>(argString)) {
                setLastErrorMessage("args: couldn't parse argument string variable name");
                return false;
            }

            if(*arg < VarCache::args.size()) {
                auto argument(VarCache::args[*arg]);
                VarCache::stringCache[argString] = argument;
                return true;
            }

            return false;
        }
    };

}