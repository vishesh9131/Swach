#include "infra/gstreamer.hpp"
#include <fstream>
#include <gst/app/gstappsink.h>
#include <gst/gst.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace infra
{

bool capture_frame_from_stream(const std::string &rtsp_url, const std::string &output_file_path)
{
    gst_init(nullptr, nullptr);

    std::string pipeline_desc = "rtspsrc location=" + rtsp_url +
                                " latency=0 ! decodebin ! videoconvert ! videoscale ! video/x-raw,format=RGB ! jpegenc "
                                "! appsink name=sink emit-signals=true";

    GError *error = nullptr;
    GstElement *pipeline = gst_parse_launch(pipeline_desc.c_str(), &error);

    if (!pipeline)
    {
        spdlog::error("Failed to create pipeline: {}", error ? error->message : "unknown error");
        if (error)
            g_error_free(error);
        return false;
    }

    GstElement *appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    GstSample *sample = nullptr;
    GstAppSink *sink = GST_APP_SINK(appsink);

    for (int i = 0; i < 20 && !sample; ++i)
    {
        g_signal_emit_by_name(appsink, "pull-sample", &sample);
        if (!sample)
            g_usleep(100000); // wait 100ms
    }

    if (!sample)
    {
        spdlog::error("Failed to pull sample from stream");
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        return false;
    }

    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstMapInfo map;
    if (!gst_buffer_map(buffer, &map, GST_MAP_READ))
    {
        spdlog::error("Failed to map buffer");
        gst_sample_unref(sample);
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        return false;
    }

    std::ofstream out_file(output_file_path, std::ios::binary);
    out_file.write(reinterpret_cast<const char *>(map.data), map.size);
    out_file.close();

    gst_buffer_unmap(buffer, &map);
    gst_sample_unref(sample);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    spdlog::info("Snapshot saved to {}", output_file_path);
    return true;
}

} // namespace infra
