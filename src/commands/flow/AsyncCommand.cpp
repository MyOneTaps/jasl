//
//  AsyncCommand.cpp
//  jasl
//
//  Created by Ben Jones 
//  Copyright (c) 2019 Ben Jones. All rights reserved.
//

#include "AsyncCommand.hpp"
#include "core/CommandInterpretor.hpp"
#include "core/ThreadManager.hpp"
#include <string>

namespace {
    // We're in new scope so add a new cache map to the cache stack
    jasl::SharedCacheStack withNewCache(jasl::SharedCacheStack const &sharedCache) {
        auto cloned = sharedCache->clone();
        cloned->pushCacheMap();
        return cloned;
    }
}

namespace jasl {

    AsyncCommand::AsyncCommand(Function &func_,
                               SharedCacheStack const &sharedCache,
                               OptionalOutputStream const &output)
        : Command(func_, withNewCache(sharedCache), output)
        , m_weHaveFuncs(false)
        , m_commands()
    {
        std::vector<Function> innerFuncs;
        if(VarExtractor::tryAnyCast<std::vector<Function>>(innerFuncs, m_func.paramB)) {
            m_weHaveFuncs = true;
        }
        if(m_weHaveFuncs) {
            CommandInterpretor ci;
            for(auto & f : innerFuncs) {
                m_commands.push_back(ci.funcToCommand(f, sharedCache, output));
            }
        }
    }

    bool AsyncCommand::execute() 
    {
        if(!m_weHaveFuncs) {
            return false;
        }
        return parseCommandsAsync();
    }

    bool AsyncCommand::parseCommandsAsync() 
    {
        ThreadManager::create([=] {
            for(auto & c : m_commands) {
                try {
                    if(c) {
                        c->execute(); 
                    }
                } catch (...) {

                }
            }
        });
        return true;
    }
}