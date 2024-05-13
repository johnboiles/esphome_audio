#pragma once

#ifdef USE_ESP_IDF

#include "adf_audio_element.h"
#include <raw_stream.h>
#include "esphome/core/helpers.h"

namespace esphome {
namespace esp_adf {

enum class ADFCodec : uint8_t {AAC = 0, AMR, FLAC, MP3, OGG, OPUS, WAV};

class Track {
public:
  std::string uri{""};
  optional<ADFCodec> codec;
  optional<int> sampling_rate;
  optional<int> bit_depth;
  optional<int> channels;
};


class ADFPipelineSourceElement : public ADFPipelineElement {
 public:
  AudioPipelineElementType get_element_type() const { return AudioPipelineElementType::AUDIO_PIPELINE_SOURCE; }
};

class HTTPStreamReaderAndDecoder : public ADFPipelineSourceElement {
 public:
  void set_stream_uri(const std::string&  new_url);
  const std::string get_name() override { return "HTTPStreamReader"; }

  bool prepare_elements(bool initial_call) override;
  bool pause_elements(bool initial_call) override;

  void set_track(Track track){}
  void prepare_track(Track track){}

  void set_fixed_settings(bool value){ this->fixed_settings_ = value; }

 protected:
  bool init_adf_elements_() override;
  void clear_adf_elements_() override;
  void reset_() override;
  bool preparing_step_();

  void sdk_event_handler_(audio_event_iface_msg_t &msg);
  void cfg_event_handler_(audio_event_iface_msg_t &msg);

  bool fixed_settings_{false};
  bool audio_settings_reported_{false};

  PipelineElementState desired_state_{PipelineElementState::UNINITIALIZED};
  std::string current_url_{"https://dl.espressif.com/dl/audio/ff-16b-2c-44100hz.mp3"};
  audio_element_handle_t http_stream_reader_{};
  audio_element_handle_t decoder_{};
};


class PCMSource : public ADFPipelineSourceElement {
 public:
  const std::string get_name() override { return "PCMSource"; }
  int stream_write(char *buffer, int len);
  bool has_buffered_data() const;

 protected:
  bool init_adf_elements_() override;
  audio_element_handle_t adf_raw_stream_writer_;
};

}  // namespace esp_adf
}  // namespace esphome
#endif
