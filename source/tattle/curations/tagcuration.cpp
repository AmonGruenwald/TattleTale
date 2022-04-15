
#include "tattle/curations/tagcuration.hpp"
#include <algorithm>
#include <unordered_set>
#include "shared/kernels/interactions/interaction.hpp"
#include "shared/actor.hpp"

namespace tattletale
{

    TagCuration::TagCuration(size_t max_chain_size) : Curation("Tag", max_chain_size), max_interaction_count_(ceil(static_cast<float>(max_chain_size_) / 2.0f)) {}
    float TagCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        Kernel *first_noteworthy_event = nullptr;
        Kernel *second_noteworthy_event = nullptr;

        float lowest_position = 11;

        float fluff_position = 1;
        float angst_position = 3;
        float sexual_position = 4;
        float relationship_position = 5;
        float hurtcomfort_position = 7;
        float family_position = 8;
        float friendship_position = 9;
        float love_position = 11;

        float max_score =
            (lowest_position - (fluff_position - 1)) +
            (lowest_position - (angst_position - 1)) +
            (lowest_position - (sexual_position - 1)) +
            (lowest_position - (relationship_position - 1)) +
            (lowest_position - (hurtcomfort_position - 1)) +
            (lowest_position - (family_position - 1)) +
            (lowest_position - (friendship_position - 1)) +
            (lowest_position - (love_position - 1));

        float score = 0;
        if (IsFluff(chain, first_noteworthy_event, second_noteworthy_event))
        {
            score += (lowest_position - (fluff_position - 1));
        }
        else
        {
            if (IsAngst(chain, first_noteworthy_event, second_noteworthy_event))
            {
                score += (lowest_position - (angst_position - 1));
            }
            if (IsSexual(chain, first_noteworthy_event, second_noteworthy_event))
            {
                score += (lowest_position - (sexual_position - 1));
            }
        }
        if (IsRelationship(chain, first_noteworthy_event, second_noteworthy_event))
        {
            score += (lowest_position - (relationship_position - 1));
        }
        if (IsHurtComfort(chain, first_noteworthy_event, second_noteworthy_event))
        {
            score += (lowest_position - (hurtcomfort_position - 1));
        }
        if (IsFamily(chain, first_noteworthy_event, second_noteworthy_event))
        {
            score += (lowest_position - (family_position - 1));
        }
        if (IsFriendship(chain, first_noteworthy_event, second_noteworthy_event))
        {
            score += (lowest_position - (friendship_position - 1));
        }
        if (IsLove(chain, first_noteworthy_event, second_noteworthy_event))
        {
            score += (lowest_position - (love_position - 1));
        }
        return (score / max_score);
    }

    Kernel *TagCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return FindNoteworthyEvent(chain, true);
    }
    Kernel *TagCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return FindNoteworthyEvent(chain, false);
    }

    Kernel *TagCuration::FindNoteworthyEvent(const std::vector<Kernel *> &chain, bool first) const
    {
        Kernel *first_noteworthy_event = nullptr;
        Kernel *second_noteworthy_event = nullptr;
        if (IsFluff(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsAngst(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsSexual(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsRelationship(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsHurtComfort(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsFamily(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsFriendship(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        else if (IsLove(chain, first_noteworthy_event, second_noteworthy_event))
        {
        }
        return (first ? first_noteworthy_event : second_noteworthy_event);
    }

    bool TagCuration::IsFluff(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        size_t fluff_count = 0;
        bool first_noteworth_event_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kInteraction)
            {
                auto interaction = dynamic_cast<Interaction *>(kernel);
                auto prototype = interaction->GetPrototype();
                if (prototype->angst || prototype->sexual)
                {
                    return false;
                }
                if (prototype->fluff)
                {
                    ++fluff_count;
                    if (!first_noteworth_event_found)
                    {
                        first_noteworth_event_found = true;
                        out_first_noteworthy_event = kernel;
                    }
                    out_second_noteworthy_event = kernel;
                }
            }
        }
        if (fluff_count > (static_cast<float>(max_interaction_count_) * 0.5f))
        {
            return true;
        }
        return false;
    }
    bool TagCuration::IsAngst(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        size_t angst_count = 0;
        bool first_noteworth_event_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kInteraction)
            {
                auto interaction = dynamic_cast<Interaction *>(kernel);
                auto prototype = interaction->GetPrototype();
                if (prototype->fluff)
                {
                    return false;
                }
                if (prototype->angst)
                {
                    ++angst_count;
                    if (!first_noteworth_event_found)
                    {
                        first_noteworth_event_found = true;
                        out_first_noteworthy_event = kernel;
                    }
                    out_second_noteworthy_event = kernel;
                }
            }
        }
        if (angst_count > (static_cast<float>(max_interaction_count_) * 0.5f))
        {
            return true;
        }
        return false;
    }
    bool TagCuration::IsSexual(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {

        bool sexual = false;
        bool first_noteworth_event_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kInteraction)
            {
                auto interaction = dynamic_cast<Interaction *>(kernel);
                auto prototype = interaction->GetPrototype();
                if (prototype->sexual)
                {
                    sexual = true;
                    if (!first_noteworth_event_found)
                    {
                        first_noteworth_event_found = true;
                        out_first_noteworthy_event = kernel;
                    }
                    out_second_noteworthy_event = kernel;
                }
            }
        }
        return sexual;
    }
    bool TagCuration::IsRelationship(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        size_t relationship_count = 0;
        bool first_noteworth_event_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kInteraction)
            {
                auto interaction = dynamic_cast<Interaction *>(kernel);
                auto prototype = interaction->GetPrototype();
                int love_index = static_cast<int>(RelationshipType::kLove);
                for (auto &relationship_effect : prototype->relationship_effects)
                {
                    for (auto &[participant, values] : relationship_effect)
                    {
                        if (values[love_index] > 0)
                        {
                            ++relationship_count;
                            if (!first_noteworth_event_found)
                            {
                                first_noteworth_event_found = true;
                                out_first_noteworthy_event = kernel;
                            }
                            out_second_noteworthy_event = kernel;
                        };
                    }
                }
            }
        }
        if (relationship_count > (static_cast<float>(max_interaction_count_) * 0.5f))
        {
            return true;
        }
        return false;
    }
    bool TagCuration::IsHurtComfort(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        bool hurt_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kEmotion)
            {
                auto emotion = dynamic_cast<Emotion *>(kernel);

                if (emotion->GetType() == EmotionType::kHappy)
                {
                    if (!hurt_found)
                    {
                        if (emotion->GetValue() < -0.5)
                        {
                            hurt_found = true;
                            out_first_noteworthy_event = kernel;
                        }
                    }
                    else
                    {
                        if (emotion->GetValue() > 0.5)
                        {
                            out_second_noteworthy_event = kernel;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    bool TagCuration::IsFamily(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        std::unordered_set<std::string> last_names;
        std::unordered_set<std::string> first_names;
        for (auto &kernel : chain)
        {
            for (auto &actor : kernel->GetAllParticipants())
            {
                bool last_name_first_occurence = last_names.insert(actor->last_name_).second;
                bool first_name_first_occurence = first_names.insert(actor->first_name_).second;

                if (!last_name_first_occurence && first_name_first_occurence)
                {
                    out_second_noteworthy_event = kernel;
                    for (auto &other_kernel : chain)
                    {
                        for (auto &other_actor : kernel->GetAllParticipants())
                        {
                            if (other_actor->last_name_ == actor->last_name_)
                            {
                                out_first_noteworthy_event = other_kernel;
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    bool TagCuration::IsFriendship(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        size_t friendship_count = 0;
        bool first_noteworth_event_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kInteraction)
            {
                auto interaction = dynamic_cast<Interaction *>(kernel);
                auto prototype = interaction->GetPrototype();
                int friendship_index = static_cast<int>(RelationshipType::kFriendship);
                for (auto &relationship_effect : prototype->relationship_effects)
                {
                    for (auto &[participant, values] : relationship_effect)
                    {
                        if (values[friendship_index] > 0)
                        {
                            ++friendship_count;
                            if (!first_noteworth_event_found)
                            {
                                first_noteworth_event_found = true;
                                out_first_noteworthy_event = kernel;
                            }
                            out_second_noteworthy_event = kernel;
                        };
                    }
                }
            }
        }
        if (friendship_count > (static_cast<float>(max_interaction_count_) * 0.5f))
        {
            return true;
        }
        return false;
    }
    bool TagCuration::IsLove(const std::vector<Kernel *> &chain, Kernel *&out_first_noteworthy_event, Kernel *&out_second_noteworthy_event) const
    {
        bool love_found = false;
        bool first_noteworth_event_found = false;
        for (auto &kernel : chain)
        {
            if (kernel->type_ == KernelType::kRelationship)
            {
                auto relationship = dynamic_cast<Relationship *>(kernel);
                if (relationship->GetType() == RelationshipType::kLove && relationship->GetValue() > 0.5f)
                {
                    love_found = true;
                    if (!first_noteworth_event_found)
                    {
                        first_noteworth_event_found = true;
                        out_first_noteworthy_event = kernel;
                    }
                    out_second_noteworthy_event = kernel;
                }
            }
        }
        return love_found;
    }
} // namespace tattletale