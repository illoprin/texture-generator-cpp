
#ifdef lp_random

// Set seed for random. C srand() implementation
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

// Random float value from 0 to 1
float random_real()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
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
#include <type_traits>

namespace my_math
{
    // Number PI
    static const float PI = 3.14159;

    // Clamps value from min to max
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


    /*
    * Converts degrees value to radians value
    */
    template<typename T>
    static float to_radians (const T deg)
    {
        static_assert
        (
            std::is_arithmetic_v<T>,
            "lp_math.to_radians: type T must be a number"
        );

        return ((float)deg) * (PI / 180); 
    }

    /*
    * Converts radians value to degrees value
    */
    template<typename T>
    static float to_deg (const T rad)
    {
        static_assert
        (
            std::is_arithmetic_v<T>,
            "lp_math.to_radians: type T must be a number"
        );

        return ((float)rad) * (180 / PI); 
    }
}

#endif