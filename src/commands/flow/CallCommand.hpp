//
//  CallCommand.hpp
//  jasl
//
//  Created by Ben Jones on on 10/03/2015.
//  Copyright (c) 2015-present Ben Jones. All rights reserved.
//

#pragma once

#include "commands/Command.hpp"

namespace jasl
{
    class CallCommand : public Command
    {
    public:
        CallCommand(Function &func_,
                    SharedCacheStack const &sharedCache = SharedCacheStack(),
                    OptionalOutputStream const &output = OptionalOutputStream());
        virtual ~CallCommand();
        bool execute() override;
        static std::vector<std::string> getCommandNames();
    private:

        bool parseCommand(std::string const &kind = "block");
        bool tryArrayTypes(std::string const theType);
        void setFunction();

        // The name of the block or returnable being called
        std::string m_functionName;

        bool m_returnable;

        // If returnable, the symbol into which we put the result
        std::string m_returnSymbol;

        // The actual block statement being called
        Function m_functionFunc;

        // The type of the returnable if returnable
        std::string m_returnableType;

        // The symbol of the return statement of the actual function called
        std::string m_returnableSymbol;

        static bool m_registered;

    };
}
