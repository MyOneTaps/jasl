//
//  ReverseCommand.hpp
//  jasl
//
//  Created by Ben Jones on 11/03/15
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"

namespace jasl
{
    class ReverseCommand : public Command
    {
      public:
        ReverseCommand(Function &func_,
                       SharedCacheStack const &sharedCache = SharedCacheStack(),
                       OptionalOutputStream const &output = OptionalOutputStream());
        virtual ~ReverseCommand();

        bool execute() override;
        static std::vector<std::string> getCommandNames();
      private:
        static bool m_registered;
    };
}
