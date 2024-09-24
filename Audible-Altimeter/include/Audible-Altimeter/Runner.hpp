#ifndef AUDIBLE_ALTIMETER_RUNNER_HPP
#define AUDIBLE_ALTIMETER_RUNNER_HPP

namespace altimeter {

class ITiny2350;

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
  Runner(ITiny2350* tiny_device);

  /**
   * @brief Runs the Runner class.
   */
  void run();

 private:
  ITiny2350* m_tiny_2350 = nullptr;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_RUNNER_HPP
