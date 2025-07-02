#include "infra/gstreamer.hpp"
#include <chrono>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char *argv[])
{
    // Silence unused warning if needed
    (void)argc;
    (void)argv;

    std::string rtsp_url = "rtsp://127.0.0.1:8554/stream";
    std::string output_dir = "./tmp";

    // Create directory if it doesn't exist
    std::filesystem::create_directories(output_dir);

    // Generate timestamped filename
    auto now = std::chrono::system_clock::now();
    auto unix_ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    std::string output_path = output_dir + "/snapshot_" + std::to_string(unix_ts) + ".jpg";

    spdlog::info("Attempting to capture frame from: {}", rtsp_url);

    bool success = infra::capture_frame_from_stream(rtsp_url, output_path);

    if (!success)
    {
        spdlog::error("Snapshot capture failed");
        return 1;
    }

    spdlog::info("Snapshot successfully written to: {}", output_path);
    return 0;
}
