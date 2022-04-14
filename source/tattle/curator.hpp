#ifndef TATTLE_CURATOR_H
#define TATTLE_CURATOR_H
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"
#include "tattle/curations/curation.hpp"

namespace tattletale
{
    class Curator
    {

    public:
        Curator(const Chronicle &chronicle, const Setting &setting);
        std::string UseAllCurations();
        std::string Narrativize(const std::vector<Kernel *> &chain, const Curation *curation) const;
        Kernel *RecursivelyFindUnlikeliestReason(Kernel *to_check, Kernel *current_best);
        Kernel *RecursivelyFindUnlikeliestConsequence(Kernel *to_check, Kernel *current_best, size_t depth) const;
        bool HasCausalConnection(Kernel *start, Kernel *end) const;
        std::vector<Kernel *> FindCausalConnection(Kernel *start, Kernel *end) const;
        Resource *FindBlockingResource(Kernel *interaction) const;
        bool RecursivelyFindCausalConnectionBackwards(Kernel *root, Kernel *start, std::vector<Kernel *> &out_causal_chain) const;

        std::string GetTimeDescription(Kernel *start, Kernel *end, bool first_letter_uppercase = true) const;
        std::string GetChanceDescription(float chance) const;
        std::string GenerateScoreDescription(float score) const;
        std::string GetResourceReasonDescription(Resource *resource) const;

        Actor *FindMostOccuringActor(const std::vector<Kernel *> &kernels, bool &out_more_actors_present) const;

    private:
        const Chronicle &chronicle_;
        const Setting &setting_;

        std::vector<Kernel *> FindBestScoringChain(const std::vector<std::vector<Kernel *>> chains, Curation *curation) const;
        std::string Curate(const std::vector<std::vector<Kernel *>> &chains, Curation *curation) const;
    };

} // namespace tattletale
#endif // TATTLE_CURATOR_H