//
//  ListSetTokenCommand.hpp
//  jasl
//
//  Created by Ben Jones on 18/03/15
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"

namespace jasl
{
    class ListSetTokenCommand : public Command
    {
    public:
        ListSetTokenCommand(Function &func_,
                            SharedVarCache const &sharedCache = SharedVarCache(),
                            OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;
    private:
        OptionalInt getIndex();
        OptionalString getNewStringToken();
        OptionalValueArray getNewVAToken();
        bool tryWithRawList(std::string const &varName);
        bool tryWithSymbolList(std::string const &varName);
    };

}