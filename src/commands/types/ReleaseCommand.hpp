//
//  ReleaseCommand.hpp
//  jasl
//
//  Created by Ben Jones on 01/10/15
//  Copyright (c) 2015-present Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"

namespace jasl
{
    class ReleaseCommand : public Command
    {
    public:
        ReleaseCommand(Function &func_,
                       SharedCacheStack const &sharedCache = SharedCacheStack(),
                       OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;

    private:
        bool trySymbolExtraction();
        static bool m_registered;
    };

}
