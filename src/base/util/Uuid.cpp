#include "Uuid.h"
#include "UniqueIdGenerator.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace BW;

std::string Uuid::generateUUID()
{
    boost::uuids::uuid bst_uuid = boost::uuids::random_generator()();
    std::string uuid = boost::uuids::to_string(bst_uuid);
    uuid.erase(std::remove(uuid.begin(), uuid.end(), '-'), uuid.end());
    return uuid;
}

uint64_t Uuid::getNextUUID()
{
    static UniqueIdGenerator uidGen;
    return uidGen.next();
}