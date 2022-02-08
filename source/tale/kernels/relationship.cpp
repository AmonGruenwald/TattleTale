#include "tale/kernels/relationship.hpp"

#include <iostream>

namespace tale
{
    Relationship::Relationship(std::string name, std::vector<std::weak_ptr<Kernel>> reasons) : Kernel(name, reasons){};
    std::string Relationship::ToString()
    {
        return "I am Relationship.\n";
    }
} // namespace tale