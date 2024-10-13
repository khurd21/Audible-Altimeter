#ifndef AUDIBLE_ALTIMETER_TIMER_INTERFACE_HPP
#define AUDIBLE_ALTIMETER_TIMER_INTERFACE_HPP

#include <chrono>

namespace altimeter {

class ITimer {
 public:
  ~ITimer() = default;

  struct CallbackData {
    bool (*callback)(void*);
    void* user_data;
  };

  virtual void add_repeating_timer(std::chrono::milliseconds interval,
                                   CallbackData& callback_data) = 0;

  virtual void remove_repeating_timer() = 0;
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_TIMER_INTERFACE_HPP