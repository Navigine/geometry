#pragma once

#include <cstdint>

#include <navigine/geometry/utils.h>

namespace navigine::geometry {

class Rng
{
public:
    Rng();
    Rng(uint32_t seed);

    //  Binary Bernoulli distribution:
    //  P{true} = p
    //  P{false} = 1 - p
    bool binary(double p);

    int discrete(int min, int max);

    double uniform(double min, double max);

    double normal(double mean, double deviation);

private:
    uint32_t rand();

    uint32_t seed_;
};

} // namespace navigine::geometry
