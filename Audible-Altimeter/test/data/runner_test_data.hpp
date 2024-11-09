#ifndef TEST_DATA_RUNNER_TEST_DATA_HPP
#define TEST_DATA_RUNNER_TEST_DATA_HPP

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "audio_samples.hpp"
#include "path_to_data_folder.hpp"

namespace altimeter::tests {
namespace detail {

inline const std::unordered_map<std::string, AUDIO_SAMPLE_ID> k_altitudes{
    {"TWO", AUDIO_SAMPLE_ID::TWO},
    {"TWO_POINT_FIVE", AUDIO_SAMPLE_ID::TWO_POINT_FIVE},
    {"THREE", AUDIO_SAMPLE_ID::THREE},
    {"THREE_POINT_FIVE", AUDIO_SAMPLE_ID::THREE_POINT_FIVE},
    {"FOUR", AUDIO_SAMPLE_ID::FOUR},
    {"FOUR_POINT_FIVE", AUDIO_SAMPLE_ID::FOUR_POINT_FIVE},
    {"FIVE", AUDIO_SAMPLE_ID::FIVE},
    {"FIVE_POINT_FIVE", AUDIO_SAMPLE_ID::FIVE_POINT_FIVE},
    {"SIX", AUDIO_SAMPLE_ID::SIX},
    {"SEVEN", AUDIO_SAMPLE_ID::SEVEN},
    {"EIGHT", AUDIO_SAMPLE_ID::EIGHT},
    {"NINE", AUDIO_SAMPLE_ID::NINE},
    {"TEN", AUDIO_SAMPLE_ID::TEN},
    {"ELEVEN", AUDIO_SAMPLE_ID::ELEVEN},
    {"TWELVE", AUDIO_SAMPLE_ID::TWELVE},
    {"THIRTEEN", AUDIO_SAMPLE_ID::THIRTEEN},
    {"FOURTEEN", AUDIO_SAMPLE_ID::FOURTEEN},
    {"FIFTEEN", AUDIO_SAMPLE_ID::FIFTEEN},
};

}  // namespace detail

struct RunnerTestValues {
  int temperature_c{};
  int pressure_pa{};
  int altitude_ft{};
  bool entered_idle_state{};
  bool entered_active_state{};
  std::optional<AUDIO_SAMPLE_ID> sample_id;
};

inline std::vector<std::vector<RunnerTestValues>> get_runner_test_data() {
  std::ifstream ifstream(k_data_folder + std::string("/runner_test_data.json"));

  if (!ifstream) {
    throw std::runtime_error("Unable to open test data json.");
  }
  std::vector<RunnerTestValues> result;
  auto json{nlohmann::json::parse(ifstream)};
  if (json.is_array() && json.at(0).is_array()) {
    json = json.at(0);
  }

  for (const auto& item : json) {
    RunnerTestValues values{
        .temperature_c = item.at("temperature_c"),
        .pressure_pa = item.at("pressure_pa"),
        .altitude_ft = item.at("altitude_ft"),
        .entered_idle_state = item.at("entered_idle_state"),
        .entered_active_state = item.at("entered_active_state"),
        .sample_id =
            item.contains("plays")
                ? std::optional(detail::k_altitudes.at(item.at("plays")))
                : std::nullopt,
    };
    result.push_back(values);
  }
  return {result};
}

}  // namespace altimeter::tests

#endif  // TEST_DATA_RUNNER_TEST_DATA_HPP