//
//  FileAppendStringCommand.hpp
//  jasl
//
//  Copyright (c) 2017 Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"

namespace jasl
{
    class FileAppendStringCommand : public Command
    {
    public:
        FileAppendStringCommand(Function &func_,
                                SharedCacheStack const &sharedCache = SharedCacheStack(),
                                OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;
    };

}