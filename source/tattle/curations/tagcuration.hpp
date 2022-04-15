#ifndef TATTLE_CURATIONS_TAGCURATION_H
#define TATTLE_CURATIONS_TAGCURATION_H

#include "tattle/curations/curation.hpp"

namespace tattletale
{
    class TagCuration : public Curation
    {
    public:
        TagCuration(size_t max_chain_size);
        float CalculateScore(const std::vector<Kernel *> &chain) const override;
        Kernel *GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
        Kernel *GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const override;

    private:
        const size_t max_interaction_count_;
        Kernel *FindNoteworthyEvent(const std::vector<Kernel *> &chain, bool first) const;
        bool IsFluff(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsAngst(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsSexual(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsRelationship(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsHurtComfort(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsFamily(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsFriendship(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
        bool IsLove(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const;
    };
} // namespace tattletale
#endif // TATTLE_CURATIONS_TAGCURATION_H