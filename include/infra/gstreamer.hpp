#include <string>

namespace infra
{
/**
 * @brief Captures a snapshot from an RTSP stream using GStreamer
 * @param rtsp_url The RTSP stream URL
 * @param output_file_path The local file path where the snapshot will be saved (JPEG/PNG)
 * @return true if successful, false otherwise
 */
bool capture_frame_from_stream(const std::string &rtsp_url, const std::string &output_file_path);
} // namespace infra
