
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

#ifdef lp_functions
// Creates beautiful (not) string from int array
std::string arr_to_str(int* arr, int len) {
	std::string s = "{ ";
	for (int* ptr = arr; ptr < arr + len; ptr++) {
		s += std::to_string(*ptr) + ", ";
	}
	// Delete extra ", " symbols
	if (!s.empty()) {
		s.pop_back();
		s.pop_back();
	}
	s += " }";
	return s;
}
#endif