//
//  BlockCommand.hpp
//  jasl
//
//  Created by Ben Jones on 10/03/2015.
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include <vector>

namespace jasl {
    class BlockCommand : public Command
    {
    public:
        BlockCommand(Function &func_, OptionalOutputStream const &output);
        bool execute() override;
    private:
        bool interpretFunctionBody();
        bool parseCommands(std::vector<Function> &functions);
    };
}
