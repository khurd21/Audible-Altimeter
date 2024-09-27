#ifndef AUDIBLE_ALTIMETER_RUNNER_HPP
#define AUDIBLE_ALTIMETER_RUNNER_HPP

namespace altimeter {

class IDeviceDescription;
;

/**
 * @brief
 * The primary runner for the Audible Altimeter. Collects all implemented
 * interfaces from the embedded library and utilizes them to build the program.
 */
class Runner {
 public:
  /**
   * @brief Constructs the Runner class.
   */
  Runner(IDeviceDescription* tiny_device);

  /**
   * @brief Runs the Runner class.
   */
  void run();

 private:
  IDeviceDescription* m_tiny_2350 = nullptr;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_RUNNER_HPP
