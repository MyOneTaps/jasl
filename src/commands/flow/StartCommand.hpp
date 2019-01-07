//
//  StartCommand.hpp
//  jasl
//
//  Created by Ben Jones on on 10/03/2015.
//  Copyright (c) 2015-present Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"
#include <vector>

namespace jasl {
    class StartCommand : public Command
    {
    public:
        StartCommand(Function &func_,
                     SharedCacheStack const &sharedCache = SharedCacheStack(),
                     OptionalOutputStream const &output = OptionalOutputStream());
        bool execute() override;
        static std::vector<std::string> getCommandNames();
    private:
        bool interpretFunctionBody();
        bool parseCommands(std::vector<Function> &functions);
        static bool m_registered;
    };
}
