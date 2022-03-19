#ifndef TATTLE_CURATOR_H
#define TATTLE_CURATOR_H
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"

namespace tattletale
{
    class Curator
    {

    public:
        Curator(const Chronicle &chronicle, const Setting &setting);
        std::string Curate();
        Kernel *RecursivelyFindUnlikeliestReason(Kernel *to_check, Kernel *current_best);
        Kernel *RecursivelyFindUnlikeliestConsequence(Kernel *to_check, Kernel *current_best, size_t depth);
        std::vector<Kernel *> FindCausalConnection(Kernel *start, Kernel *end) const;
        Resource *FindBlockingResource(Interaction *interaction) const;
        bool RecursivelyFindCausalConnectionBackwards(Kernel *root, Kernel *start, std::vector<Kernel *> &out_causal_chain) const;
        std::string GetTimeDescription(Kernel *start, Kernel *end) const;
        std::string GetChanceDescription(float chance) const;
        std::string GetResourceReasonDescription(Resource *resource) const;

    private:
        std::string RarityCuration();
        const Chronicle &chronicle_;
        const Setting &setting_;
    };

} // namespace tattletale
#endif // TATTLE_CURATOR_H