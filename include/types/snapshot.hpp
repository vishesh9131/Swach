/**
 * @file snapshot.hpp
 * @brief Declaration of the Snapshot class used for managing snapshot metadata.
 *
 * This header defines the Snapshot class which encapsulates information related
 * to a captured snapshot including stream reference, cloud storage metadata, and
 * generation of a unique object ID using Boost UUIDs.
 *
 * @author Mohit Sharma (Coderero)
 * @email  mohit@coderero.dev
 */

#ifndef SNAPSHOT_HPP
#define SNAPSHOT_HPP

#include <cstdint>
#include <string>

/// @brief Contains type definitions used across the project.
namespace types
{

/**
 * @class Snapshot
 * @brief Represents metadata of a snapshot captured from a stream and stored in a cloud bucket.
 *
 * The Snapshot class holds identifying information such as stream ID, bucket ID,
 * timestamp of capture, and generates a unique object ID for tracking the snapshot.
 */
class Snapshot
{
  public:
    /// @brief Unique identifier for the RTSP stream.
    std::string stream_id;

    /// @brief Unique identifier of the cloud bucket where the snapshot is stored.
    std::string bucket_id;

    /// @brief Auto-generated unique object ID for the snapshot in the cloud.
    std::string object_id;

    /// @brief Epoch timestamp representing when the snapshot was captured.
    std::int64_t timestamp;

    /**
     * @brief Constructs a new Snapshot instance and auto-generates an object ID.
     *
     * @param stream_id Identifier for the stream the snapshot originates from.
     * @param bucket_id Identifier for the cloud bucket target.
     * @param timestamp Epoch timestamp of when the snapshot was taken.
     */
    Snapshot(const std::string &stream_id, const std::string &bucket_id, std::int64_t timestamp);

  private:
    /**
     * @brief Generates a unique object ID string composed of snapshot metadata and a UUID.
     *
     * @return A string representing the unique object ID.
     */
    std::string get_object_id() const;
};

} // namespace types

#endif // SNAPSHOT_HPP
