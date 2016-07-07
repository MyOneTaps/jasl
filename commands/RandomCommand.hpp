//
//  RandomCommand.hpp
//  jasl
//
//  Created by Ben Jones on 07/06/16
//  Copyright (c) 2016 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include <random>

namespace jasl {
    class RandomCommand : public Command
    {
    public:
        RandomCommand(Function &func_,
                      SharedVarCache const &sharedCache = SharedVarCache(),
                      OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;

    private:
        std::string m_type;
        std::string m_varName;
        std::random_device m_r;

    };
}
