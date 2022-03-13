#include "tattle/curator.hpp"
#include <fmt/core.h>

namespace tattletale
{
    Curator::Curator(const Chronicle &chronicle, const Setting &setting) : chronicle_(chronicle), setting_(setting) {}

    std::string Curator::Curate()
    {
        auto interaction = chronicle_.FindUnlikeliestInteraction().lock();
        return fmt::format("{} happened with chance of {}", interaction->ToString(), interaction->GetChance());
    }
} // namespace tattletale
