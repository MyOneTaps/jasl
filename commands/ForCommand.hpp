//
//  ForCommand.hpp
//  jasl
//
//  Created by Ben Jones on 03/10/15
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include <string>
#include <vector>

namespace jasl {

    class ForCommand : public Command
    {
    public:
        ForCommand(Function &func_, 
                   SharedVarCache const &sharedCache = SharedVarCache(),
                   OptionalOutputStream const &output = OptionalOutputStream());
        bool execute() override;
    private:
        bool parseCommands(std::vector<Function> &functions);
        bool processList(ValueArray const &va, std::string const &listSymbol);

        template<typename T>
        bool processArray(T const &array, std::string const & listSymbol);

        bool processString(std::string const &str, 
                           std::string const & stringSymbol);
    };

}