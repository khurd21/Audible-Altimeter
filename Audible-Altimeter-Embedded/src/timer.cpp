#include <hardware/i2c.h>
#include <hardware/timer.h>

#include <Audible-Altimeter-Embedded/timer.hpp>

namespace altimeter {

void Timer::add_repeating_timer(std::chrono::milliseconds interval,
                                CallbackData& callback_data) {
  add_repeating_timer_ms(interval.count(), &Timer::timer_callback,
                         &callback_data, &m_timer);
  m_timer_added = true;
}

void Timer::remove_repeating_timer() {
  if (m_timer_added) {
    cancel_repeating_timer(&m_timer);
    m_timer_added = false;
  }
}

bool Timer::timer_callback(repeating_timer_t* rt) {
  auto data{static_cast<CallbackData*>(rt->user_data)};
  if (data) {
    return data->callback(data->user_data);
  }
  return false;
}

}  // namespace altimeter
