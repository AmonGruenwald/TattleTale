#include "tale/kernels/emotion.hpp"

#include <iostream>

namespace tale
{
    Emotion::Emotion(std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(reasons){};
    std::string Emotion::ToString()
    {
        return "I am an Emotion.\n";
    }
} // namespace tale