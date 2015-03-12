//
//  echo_nlCommand.hpp
//  jasl
//
//  Created by Ben Jones 
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"
#include "../LiteralString.hpp"
#include "../VarExtractor.hpp"
#include "../VarCache.hpp"

namespace jasl
{
    class echo_nlCommand : public Command
    {
    public:
        echo_nlCommand(Function &func_,
                      OptionalOutputStream const &output = OptionalOutputStream())
        : Command(func_, output)
        {

        }

        bool execute() override
        {
            if(tryLiteralExtraction()) { return true; }
            if(trySymbolExtraction()) { return true; }
            if(tryNumericExtraction()) { return true; }
            m_errorMessage = "echo: couldn't parse";
            appendToOutputWithNewLine(m_errorMessage);
            return false;
        }

    private:
        bool tryLiteralExtraction() 
        {
            LiteralString literalString;
            if(m_func.getValueA<LiteralString>(literalString)) {
                appendToOutputWithNewLine(literalString.literal);
                return true;
            }
            return false;
        }

        bool trySymbolExtraction()
        {
                        // Now try extracting a symbol
            std::string symbol;
            if(m_func.getValueA<std::string>(symbol)) {
                {
                    auto result = VarExtractor::searchInt(symbol);
                    if(result) {
                        appendToOutputWithNewLine(*result);
                        return true;
                    }
                }
                {
                    auto result = VarExtractor::searchDouble(symbol);
                    if(result) {
                        appendToOutputWithNewLine(*result);
                        return true;
                    }
                }
                {
                    auto result = VarExtractor::searchBool(symbol);
                    if(result) {
                        appendToOutputWithNewLine(*result);
                        return true;
                    }
                }
                {
                    auto result = VarExtractor::searchString(symbol);
                    if(result) {
                        appendToOutputWithNewLine(*result);
                        return true;
                    }
                }
                return true;
            }

            return false;
        }

        bool tryNumericExtraction()
        {
            {
                auto result = VarExtractor::tryToGetADouble(m_func.paramA);
                if(result) {
                    appendToOutputWithNewLine(*result);
                    return true;
                }
            }

            {
                auto result = VarExtractor::trySingleBoolExtraction(m_func.paramA);
                if(result) {
                    appendToOutputWithNewLine(*result);
                    return true;
                }
            }

            return false;

        }
    };
}