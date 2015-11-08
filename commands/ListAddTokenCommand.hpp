//
//  ListAddTokenCommand.hpp
//  jasl
//
//  Created by Ben Jones on 04/10/15
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include <string>

namespace jasl
{
    class ListAddTokenCommand : public Command
    {
    public:
        ListAddTokenCommand(Function &func_,
                            SharedVarCache const &sharedCache = SharedVarCache(),
                            OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;
    private:
        OptionalString getNewStringToken();
        OptionalValueArray getNewVAToken();
        bool tryWithSymbolList(std::string const &varName);
    };

}