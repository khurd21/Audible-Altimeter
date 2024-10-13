#ifndef AUDIBLE_ALTIMETER_EMBEDDED_TIMER_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_TIMER_HPP

#include <hardware/i2c.h>
#include <hardware/timer.h>

#include <Audible-Altimeter/timer_interface.hpp>

namespace altimeter {

class Timer : public ITimer {
 public:
  void add_repeating_timer(std::chrono::milliseconds interval,
                           CallbackData& callback_data) override;

  void remove_repeating_timer() override;

 private:
  static bool timer_callback(repeating_timer_t* rt);

  repeating_timer_t m_timer;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_TIMER_HPP