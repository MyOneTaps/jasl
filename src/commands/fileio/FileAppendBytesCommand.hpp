//
//  FileAppendBytesCommand.hpp
//  jasl
//
//  Copyright (c) 2017 Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"

namespace jasl
{
    class FileAppendBytesCommand : public Command
    {
      public:
        FileAppendBytesCommand(Function &func_,
                               SharedCacheStack const &sharedCache = SharedCacheStack(),
                               OptionalOutputStream const &output = OptionalOutputStream());

        virtual ~FileAppendBytesCommand();

        bool execute() override;

        static std::vector<std::string> getCommandNames();

        static bool m_registered;
    };

}
