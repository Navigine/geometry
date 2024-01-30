#include <algorithm>
#include <cmath>
#include <limits>

#include <navigine/geometry/random.h>

namespace math = std;

namespace navigine::geometry {

namespace {

const uint32_t DEFAULT_SEED = 54301;

} // namespace

Rng::Rng()
    : seed_(DEFAULT_SEED)
{}

Rng::Rng(uint32_t seed)
    : seed_(seed)
{
    if (!seed || seed >= 0x7FFFFFFF) {
        throw std::runtime_error("Invalid rng seed");
    }
}

bool Rng::binary(double p)
{
    return uniform(0, 1) <= p;
}

int Rng::discrete(int min, int max)
{
    uint32_t count = std::abs(max - min) + 1;
    return min + static_cast<int>(rand() % count);
}

double Rng::uniform(double min, double max)
{
    double r = static_cast<double>(rand()) / 0x7FFFFFFF;
    return min + (max - min) * r;
}

double Rng::normal(double mean, double stddev)
{
    // Using Box-Muller transform:
    // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

    double u1 = uniform(0, 1);
    double u2 = uniform(0, 1);

    u1 = std::max(u1, std::numeric_limits<double>::epsilon());

    return mean + stddev * math::sqrt(-2 * math::log(u1)) * math::cos(2 * M_PI * u2);
}

uint32_t Rng::rand()
{
    // Using Lehmer random number generator with M = 2^31 - 1, A = 16807
    // https://en.wikipedia.org/wiki/Lehmer_random_number_generator
    // Calculating: seed_ = (seed_ * A) (mod M)

    // This implementation is very fast, because there is no division

    uint32_t lo = 16807 * (seed_ & 0xFFFF);
    uint32_t hi = 16807 * (seed_ >> 16);

    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;

    // value % M = ((2^31) * hi + lo) % M
    //           = (M * hi + hi + lo) % M
    //           = (hi + lo) % M

    if (lo > 0x7FFFFFFF) {
        lo -= 0x7FFFFFFF;
    }

    return seed_ = lo;
}

} // namespace navigine::geometry
