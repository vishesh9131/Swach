#include "types/snapshot.hpp"
#include "utils/logger.hpp"

#include <chrono>

int main(int argc, const char *argv[])
{
    auto now = std::chrono::system_clock::now();
    auto unix_ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    auto logger = utils::Logger::logger();

    /*
     * Just to silence the warning
     */
    (void)argc;
    (void)argv;
    (void)unix_ts;

    auto snapshot = new types::Snapshot("cam_id_01", "swatch_ml", unix_ts);

    logger->info("Snapshot created with ID: {}", snapshot->object_id);

    delete snapshot;

    return 0;
}
