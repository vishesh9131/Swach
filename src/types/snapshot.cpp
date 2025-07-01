/**
 * @file snapshot.cpp
 * @brief Implementation of the Snapshot class declared in snapshot.hpp.
 *
 * This file contains the definitions for the Snapshot class, including its constructor
 * and member functions such as `get_object_id`, which generates a unique identifier
 * for each snapshot using Boost UUIDs.
 *
 * @author Mohit Sharma (Coderero)
 * @email  mohit@coderero.dev
 */
#include "types/snapshot.hpp"

#include <boost/uuid.hpp>
#include <ostream>
#include <sstream>

using namespace types;

Snapshot::Snapshot(const std::string &stream_id, const std::string &bucket_id, std::int64_t timestamp)
{
    this->stream_id = stream_id;
    this->bucket_id = bucket_id;
    this->timestamp = timestamp;

    this->object_id = this->get_object_id();
}

std::string Snapshot::get_object_id() const
{
    boost::uuids::random_generator gen;
    boost::uuids::uuid uid_raw = gen();
    std::string id = boost::uuids::to_string(uid_raw).substr(24, 35);

    std::ostringstream oss;

    oss << "snapshot-" << stream_id << "-" << bucket_id << "-" << timestamp << "-" << id;

    return oss.str();
}
