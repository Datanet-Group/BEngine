#pragma once
struct TimeStep {
    double accumulator = 0.0;
    const double fixed = 1.0/60.0;
};
