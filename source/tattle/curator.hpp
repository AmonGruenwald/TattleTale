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
        std::shared_ptr<Kernel> RecursivelyFindUnlikeliestReason(std::shared_ptr<Kernel> to_check, std::shared_ptr<Kernel> current_best);
        std::shared_ptr<Kernel> RecursivelyFindUnlikeliestConsequence(std::shared_ptr<Kernel> to_check, std::shared_ptr<Kernel> current_best, size_t depth);
        std::vector<std::shared_ptr<Kernel>> FindCausalConnection(std::shared_ptr<Kernel> start, std::shared_ptr<Kernel> end) const;
        bool RecursivelyFindCausalConnectionBackwards(std::shared_ptr<Kernel> root, std::shared_ptr<Kernel> start, std::vector<std::shared_ptr<Kernel>> &out_causal_chain) const;

    private:
        const Chronicle &chronicle_;
        const Setting &setting_;
    };

} // namespace tattletale
#endif // TATTLE_CURATOR_H