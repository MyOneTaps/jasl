//
//  StartCommand.hpp
//  jasl
//
//  Created by Ben Jones on on 10/03/2015.
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include <vector>

namespace jasl {
    class StartCommand : public Command
    {
    public:
        StartCommand(Function &func_, OptionalOutputStream const &output);
        bool execute() override;
    private:
        bool interpretFunctionBody();
        bool parseCommands(std::vector<Function> &functions);
    };
}
