//
//  CallCommand.hpp
//  jasl
//
//  Created by Ben Jones on on 10/03/2015.
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//


#include "CallCommand.hpp"
#include "ParamExtractor.hpp"
#include "../GlobalCache.hpp"
#include "../CommandInterpretor.hpp"
#include <vector>

namespace jasl
{
    CallCommand::CallCommand(Function &func_,
                             SharedVarCache const &sharedCache,
                             OptionalOutputStream const &output)
    : Command(func_, sharedCache, output)
    , m_functionName()
    , m_returnable(false)
    , m_returnableType()
    , m_returnableSymbol()
    {
        // get the name of the function
        (void)m_func.getValueA<std::string>(m_functionName, m_sharedCache);

        // check if call statement expecting a returnable in
        // which case we search for returnable rather than block
        // functions
        if(m_func.getValueC<std::string>(m_returnSymbol, m_sharedCache)) {
            m_returnable = true;
        }
        setFunction();

        // set returnable type and symbol
        if(m_returnable) {
            // what is the return type?
            (void)m_functionFunc.getValueA<std::string>(m_returnableType, m_sharedCache);
        }

        // Try and get the name of the return variable if we have one
        if(m_returnableType != "nil" && m_returnableType != "") {
            if(m_returnableType != "array") {
                (void)m_functionFunc.getValueD<std::string>(m_returnableSymbol, m_sharedCache);
            } else {
                (void)m_functionFunc.getValueE<std::string>(m_returnableSymbol, m_sharedCache);
            }
        }
    }

    void CallCommand::setFunction()
    {
        // First get all subroutines
        std::string &scriptString = GlobalCache::script;
        CommandInterpretor ci;
        auto functions = ci.parseStringCollection(scriptString);

        // Find all functions that are 'fn' or 'block'
        decltype(functions) matched;
        for(auto &f : functions) {
            std::string funcName;
            if (m_returnable) {
                if (f.name.find("fn") != std::string::npos) {
                    matched.push_back(f);
                }
            } else {
                if (f.name.find("fn") != std::string::npos ||
                    f.name.find("block") != std::string::npos) {
                    matched.push_back(f);
                }
            }
        }

        // Second, search for block with given name
        auto it = std::find_if(std::begin(matched), std::end(matched), [&](Function &f) {

            std::string functionName;
            if(f.name.find("block") != std::string::npos) {
                (void)f.getValueA<std::string>(functionName, m_sharedCache);
            } else if(f.name.find("fn") != std::string::npos) {

                std::string type;
                (void)f.getValueA<std::string>(type, m_sharedCache);
                if(type != "array") {
                    (void)f.getValueB<std::string>(functionName, m_sharedCache);
                } else {
                    (void)f.getValueC<std::string>(functionName, m_sharedCache);
                }
            }
            return functionName == m_functionName;
        });

        if (it != std::end(matched)) {
            m_functionFunc = *it;
        }
    }

    bool CallCommand::execute()
    {
        // try to extract input arguments, if there are any
        ValueArray array;
        if(m_func.getValueB<ValueArray>(array, m_sharedCache)) {
            extractAndUpdateParams(array, m_sharedCache, GlobalCache::bigCache);
        }

        return parseCommand(m_returnable ? "fn" : "block");
    }

    bool CallCommand::parseCommand(std::string const &kind)
    {
        CommandInterpretor ci;
        (void)ci.interpretFunc(m_functionFunc, m_sharedCache, m_outputStream);

        // now set result of function if returnable type
        if(kind == "fn") {

            // now set the returned variable in the m_returnSymbol
            // and erase the original function result
            // Note, values reurned from the function are on
            // the global stack so need to access from GlobalCache
            if(m_returnableType == "int") {
                int64_t value;
                (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::Int);
                m_sharedCache->setVar(m_returnSymbol, value, Type::Int);
            } else if(m_returnableType == "real") {
                double value;
                (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::Double);
                m_sharedCache->setVar(m_returnSymbol, value, Type::Double);
            } else if(m_returnableType == "string") {
                std::string value;
                (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::String);
                m_sharedCache->setVar(m_returnSymbol, value, Type::String);
            } else if(m_returnableType == "bool") {
                bool value;
                (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::Bool);
                m_sharedCache->setVar(m_returnSymbol, value, Type::Bool);
            } else if(m_returnableType == "list") {
                ValueArray value;
                (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::ValueArray);
                m_sharedCache->setVar(m_returnSymbol, value, Type::ValueArray);
            } else if(m_returnableType == "byte") {
                uint8_t value;
                (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::Byte);
                m_sharedCache->setVar(m_returnSymbol, value, Type::Byte);
            }  else if(m_returnableType == "nil") {
                // don't do anything for return type 'nil'
            } else if (m_returnableType == "array") {

                std::string subType;
                (void)m_functionFunc.getValueB<std::string>(subType, m_sharedCache);
                if(subType == "int") {
                    IntArray value;
                    (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::IntArray);
                    m_sharedCache->setVar(m_returnSymbol, value, Type::IntArray);
                } else if(subType == "real") {
                    DoubleArray value;
                    (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::DoubleArray);
                    m_sharedCache->setVar(m_returnSymbol, value, Type::DoubleArray);
                } else if(subType == "byte") {
                    ByteArray value;
                    (void)GlobalCache::getVar_(m_returnableSymbol, value, Type::ByteArray);
                    m_sharedCache->setVar(m_returnSymbol, value, Type::ByteArray);
                }
            } else {
                setLastErrorMessage("call returnable: unknown return type");
                return false;
            }

            // to introduce some notion of variable locality,
            // erase the variable generated by the function itself
            if(m_returnableType != "nil") {
                GlobalCache::eraseValue(m_returnableSymbol);
            }

        }

        return true;
    }
}

