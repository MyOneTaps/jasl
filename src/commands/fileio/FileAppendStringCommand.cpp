//
//  FileAppendStringCommand.cpp
//  jasl
//
//  Copyright (c) 2017 Ben Jones. All rights reserved.
//

#include "FileAppendStringCommand.hpp"
#include "IOFunctions.hpp"
#include "caching/VarExtractor.hpp"
#include <boost/filesystem.hpp>
#include <fstream>
#include <string>

namespace jasl
{

    FileAppendStringCommand::FileAppendStringCommand(Function &func_,
                                                     SharedCacheStack const &sharedCache,
                                                     OptionalOutputStream const &output)
    : Command(func_, sharedCache, output)
    {
    }

    bool FileAppendStringCommand::execute() 
    { 
        std::string filename;
        if(VarExtractor::trySingleStringExtraction(m_func.paramB, filename, m_sharedCache)) {

            std::string value;
            if(!VarExtractor::trySingleStringExtraction(m_func.paramA, value, m_sharedCache)) {
                setLastErrorMessage("file_write_bytes: problem with string");
                return false;
            }
            // open the file and write data
            writeBytes(filename, 
                       reinterpret_cast<char const*>(&value.front()), 
                       value.size(),
                       std::ios::binary | std::ios::app);
        } else {
            setLastErrorMessage("file_write_bytes: couldn't parse filename");
            return false;
        }
        return true;
    }
}