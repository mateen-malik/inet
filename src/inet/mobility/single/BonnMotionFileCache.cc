//
// Copyright (C) 2005 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "inet/mobility/single/BonnMotionFileCache.h"

#include <fstream>
#include <sstream>

namespace inet {

const BonnMotionFile::Line *BonnMotionFile::getLine(int nodeId) const
{
    LineList::const_iterator it = lines.begin();
    for (int i = 0; i < nodeId && it != lines.end(); i++)
        it++;
    return (it == lines.end()) ? nullptr : &(*it);
}

BonnMotionFileCache *BonnMotionFileCache::inst;

BonnMotionFileCache *BonnMotionFileCache::getInstance()
{
    if (!inst)
        inst = new BonnMotionFileCache;
    return inst;
}

void BonnMotionFileCache::deleteInstance()
{
    if (inst) {
        delete inst;
        inst = nullptr;
    }
}

const BonnMotionFile *BonnMotionFileCache::getFile(const char *filename)
{
    // if found, return it from cache
    auto it = cache.find(std::string(filename));
    if (it != cache.end())
        return &(it->second);

    // load and store in cache
    BonnMotionFile& bmFile = cache[filename];
    parseFile(filename, bmFile);
    return &bmFile;
}

void BonnMotionFileCache::parseFile(const char *filename, BonnMotionFile& bmFile)
{
    std::ifstream in(filename, std::ios::in);
    if (in.fail())
        throw cRuntimeError("Cannot open file '%s'", filename);

    std::string line;
    while (std::getline(in, line)) {
        bmFile.lines.push_back(BonnMotionFile::Line());
        BonnMotionFile::Line& vec = bmFile.lines.back();

        std::stringstream linestream(line);
        double d;
        while (linestream >> d)
            vec.push_back(d);
    }
    in.close();
}

} // namespace inet

