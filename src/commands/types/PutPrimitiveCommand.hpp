//
//  PutPrimitiveCommand.hpp
//  jasl
//
//  Created by Ben Jones on 06/07/16
//  Copyright (c) 2016-present Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"

namespace jasl
{
    class PutPrimitiveCommand : public Command
    {
    public:
        PutPrimitiveCommand(Function &func_,
                            SharedCacheStack const &sharedCache = SharedCacheStack(),
                            OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;
    private:
        std::string m_type;
        std::string m_varName;
        static bool m_registered;
    };

}