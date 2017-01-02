//
//  FileAppendBytesCommand.cpp
//  jasl
//
//  Copyright (c) 2017 Ben Jones. All rights reserved.
//

#include "FileAppendBytesCommand.hpp"
#include "IOFunctions.hpp"
#include "../../caching/VarExtractor.hpp"
#include <boost/filesystem.hpp>
#include <fstream>

namespace jasl
{

    FileAppendBytesCommand::FileAppendBytesCommand(Function &func_,
                                                   SharedCacheStack const &sharedCache,
                                                   OptionalOutputStream const &output)
    : Command(func_, sharedCache, output)
    {
    }

    bool FileAppendBytesCommand::execute() 
    { 
         std::string filename;
        if(VarExtractor::trySingleStringExtraction(m_func.paramB, filename, m_sharedCache)) {

            ByteArray value;
            if(!VarExtractor::trySingleArrayExtraction(m_func.paramA, value, m_sharedCache, Type::ByteArray)) {
                setLastErrorMessage("file_write_bytes: problem with bytes array");
                return false;
            }
            // open the file and write data
            writeBytes(filename, value, std::ios::binary | std::ios::app);
        } else {
            setLastErrorMessage("file_write_bytes: couldn't parse filename");
            return false;
        }
        return true;
    }
}