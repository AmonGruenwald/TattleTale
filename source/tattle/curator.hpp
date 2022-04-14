#ifndef TATTLE_CURATOR_H
#define TATTLE_CURATOR_H
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"

namespace tattletale
{
    struct NarrativeFunctions
    {
        NarrativeFunctions(
            float (*calculate_score)(const std::vector<Kernel *> &, size_t),
            std::string (*generate_score_description)(float),
            Kernel *(*get_first_noteworthy_event)(const std::vector<Kernel *> &),
            Kernel *(*get_second_noteworthy_event)(const std::vector<Kernel *> &))
            : calculate_score(calculate_score),
              generate_score_description(generate_score_description),
              get_first_noteworthy_event(get_first_noteworthy_event),
              get_second_noteworthy_event(get_second_noteworthy_event) {}
        float (*calculate_score)(const std::vector<Kernel *> &, size_t);
        std::string (*generate_score_description)(float);
        Kernel *(*get_first_noteworthy_event)(const std::vector<Kernel *> &);
        Kernel *(*get_second_noteworthy_event)(const std::vector<Kernel *> &);
    };
    class Curator
    {

    public:
        Curator(const Chronicle &chronicle, const Setting &setting);
        std::string Curate();
        std::string Narrativize(const std::vector<Kernel *> &chain, size_t max_chain_size, const NarrativeFunctions &narrative_functions) const;
        Kernel *RecursivelyFindUnlikeliestReason(Kernel *to_check, Kernel *current_best);
        Kernel *RecursivelyFindUnlikeliestConsequence(Kernel *to_check, Kernel *current_best, size_t depth) const;
        bool HasCausalConnection(Kernel *start, Kernel *end) const;
        std::vector<Kernel *> FindCausalConnection(Kernel *start, Kernel *end) const;
        Resource *FindBlockingResource(Interaction *interaction) const;
        bool RecursivelyFindCausalConnectionBackwards(Kernel *root, Kernel *start, std::vector<Kernel *> &out_causal_chain) const;
        std::string GetTimeDescription(Kernel *start, Kernel *end, bool first_letter_uppercase = true) const;
        std::string GetChanceDescription(float chance) const;
        std::string GetResourceReasonDescription(Resource *resource) const;
        Actor *FindMostOccuringActor(const std::vector<Kernel *> &kernels, bool &out_more_actors_present) const;

    private:
        const Chronicle &chronicle_;
        const Setting &setting_;

        std::string RarityCuration(const std::vector<std::vector<Kernel *>> &chains) const;
        std::vector<Kernel *> CurateForScore(const std::vector<std::vector<Kernel *>> chains, size_t max_chain_size, float (*scoring_function)(const std::vector<Kernel *> &, size_t)) const;

        std::string AbsoluteInterestsCuration(const std::vector<std::vector<Kernel *>> &chains, size_t max_chain_size) const;
        static float CalculateAbsoluteInterestScore(const std::vector<Kernel *> &chain, size_t max_chain_size);
        static std::string GenerateAbsoluteInterestScoreDescription(float score_ratio);
        static Kernel *GetFirstNoteworthyAbsoluteInterestEvent(const std::vector<Kernel *> &kernels);
        static Kernel *GetSecondNoteworthyAbsoluteInterestEvent(const std::vector<Kernel *> &kernels);
    };

} // namespace tattletale
#endif // TATTLE_CURATOR_H