
#ifdef lp_random

// Set seed for random
void set_seed(uint seed)
{
	srand(seed);
}

// Random unsigned int value from 'start' to 'end'
uint random_uniform_unsigned(uint start, uint end)
{
	return rand() % (end - start + 1) + start;
}

u_int8_t random_byte()
{
    return static_cast<u_int8_t>(random_uniform_unsigned(0, 255));
}

// Random float value from 'start' to 'end'
float random_uniform_real(float start, float end)
{
	float range = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return ((end - start) * range) + start;
}

// Random value from 0 to 1
float random_real()
{
	return static_cast<float>( rand() ) / static_cast<float>(RAND_MAX);
}

#include <random>
#include <sstream>

namespace uuid
{
	static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

	static std::string gen_v4()
	{
		std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
	}
}

#endif


#ifdef lp_math
// Clamps value from min to max
#include <type_traits>

template<typename T>
static T clamp(const T value, const T min, const T max)
{
    static_assert
    (
        std::is_arithmetic_v<T>,
        "lp_math.clamp function: type T must be a number"
    );

    return std::min(std::max(value, min), max);
}

#endif