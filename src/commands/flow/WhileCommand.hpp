//
//  WhileCommand.hpp
//  jasl
//
//  Created by Ben Jones on 11/03/15
//  Copyright (c) 2015-present Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"
#include <string>
#include <vector>
#include <memory>

namespace jasl {

    class WhileCommand : public Command
    {
    public:
        WhileCommand(Function &func_,
                     SharedCacheStack const &sharedCache = SharedCacheStack(),
                     OptionalOutputStream const &output = OptionalOutputStream());
        virtual ~WhileCommand();
        bool execute() override;
        static std::vector<std::string> getCommandNames();
    private:
        bool parseCommands();
        bool m_weHaveFuncs;
        std::vector<std::shared_ptr<Command>> m_commands;
        static bool m_registered;
    };

}
