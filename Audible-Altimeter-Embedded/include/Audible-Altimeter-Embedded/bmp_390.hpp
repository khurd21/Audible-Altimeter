#ifndef AUDIBLE_ALTIMETER_EMBEDDED_BMP_390_HPP
#define AUDIBLE_ALTIMETER_EMBEDDED_BMP_390_HPP

#include <BMP3_SensorAPI/bmp3_defs.h>

#include <Audible-Altimeter/barometric_sensor_interface.hpp>
#include <optional>

namespace altimeter {

class BMP390 : public IBarometricSensor {
 public:
  BMP390();
  std::optional<SensorData> get_sensor_data() override;

 private:
  bmp3_dev m_bmp3;
  bmp3_settings m_settings{0, 0, 0};
};

}  // namespace altimeter

#endif  // AUDIBLE_ALTIMETER_EMBEDDED_BMP_390_HPP