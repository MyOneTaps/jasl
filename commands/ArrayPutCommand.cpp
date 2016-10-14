//
//  ArrayPutCommand.cpp
//  jasl
//
//  Created by Ben Jones on 30/06/15
//  Copyright (c) 2016 Ben Jones. All rights reserved.
//

#include "ArrayPutCommand.hpp"
#include "../VarExtractor.hpp"

namespace jasl
{
    ArrayPutCommand::ArrayPutCommand(Function &func_,
                                     SharedVarCache const &sharedCache,
                                     OptionalOutputStream const &output)
    : Command(func_, sharedCache, output)
    , m_type()
    , m_varName()
    {
        // example
        // put 5 -> q(0);
        (void)m_func.getValueB<std::string>(m_varName, m_sharedCache);

        auto type = sharedCache->getType(m_varName);
        if(!type) {
            setLastErrorMessage("put: couldn't determine type");
        } else if(*type == Type::IntArray) {
            m_type = "int";
        } else if(*type == Type::DoubleArray) {
            m_type = "real";
        } else if(*type == Type::ByteArray) {
            m_type = "byte";
        }  else {
            setLastErrorMessage("put: couldn't determine type");
        }
    }

    bool ArrayPutCommand::execute() 
    {

        if(m_type.empty()) {
            return false;
        }

        int64_t index;
        if(!getIndex(index)) {
            setLastErrorMessage("put: problem extracting index");
            return false;
        }

        // see if int or double array
        if (m_type == "int") {
            int64_t value;
            if (!VarExtractor::trySingleIntExtraction(m_func.paramA, value, m_sharedCache)) {
                setLastErrorMessage("put: problem setting int");
                return false;
            } 

            m_sharedCache->setValueInIntArray(m_varName, index, value);

        } else if (m_type == "real") {
            double value;
            if (!VarExtractor::trySingleDoubleExtraction(m_func.paramA, value, m_sharedCache)) {
                setLastErrorMessage("put: problem setting real");
                return false;
            } 

            m_sharedCache->setValueInDoubleArray(m_varName, index, value);
        } else if (m_type == "byte") {
            uint8_t value;
            if (!VarExtractor::trySingleByteExtraction(m_func.paramA, value, m_sharedCache)) {
                setLastErrorMessage("put: problem setting byte");
                return false;
            } 

            m_sharedCache->setValueInByteArray(m_varName, index, value);
        }
        return true;
    }

    bool ArrayPutCommand::getIndex(int64_t &index) 
    {
        return VarExtractor::trySingleIntExtraction(m_func.paramC, index, m_sharedCache);
    }

}