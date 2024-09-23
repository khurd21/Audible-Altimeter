#include <cstdint>
#include <cstddef>

class AudioDriver
{
public:
    virtual void play(std::int16_t* buffer, std::size_t buffer_length) = 0;

    virtual bool is_playing() = 0;
};