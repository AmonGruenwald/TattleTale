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
        bool HasCausalConnection(Kernel *start, Kernel *end) const;
        std::vector<Kernel *> FindCausalConnection(Kernel *start, Kernel *end) const;
        Resource *FindBlockingResource(Interaction *interaction) const;
        bool RecursivelyFindCausalConnectionBackwards(Kernel *root, Kernel *start, std::vector<Kernel *> &out_causal_chain) const;
        std::string GetTimeDescription(Kernel *start, Kernel *end, bool first_letter_uppercase = true) const;
        std::string GetInterestScoreDescription(float score_ratio) const;
        std::string GetChanceDescription(float chance) const;
        std::string GetResourceReasonDescription(Resource *resource) const;
        Actor *FindMostOccuringActor(const std::vector<Kernel *> &kernels, bool &out_more_actors_present) const;

        Kernel *FindLowestNonZeroAbsoluteInterestKernel(const std::vector<Kernel *> &kernels) const;
        Kernel *FindHighestNonZeroAbsoluteInterestKernel(const std::vector<Kernel *> &kernels) const;

    private:
        std::string RarityCuration();
        std::string AbsoluteInterestsCuration();
        const Chronicle &chronicle_;
        const Setting &setting_;
    };

} // namespace tattletale
#endif // TATTLE_CURATOR_H