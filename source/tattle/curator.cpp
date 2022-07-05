#include "tattle/curator.hpp"
#include "shared/actor.hpp"
#include <fmt/core.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include "shared/tattletalecore.hpp"
#include "tattle/curations/raritycuration.hpp"
#include "tattle/curations/absoluteinterestcuration.hpp"
#include "tattle/curations/tagcuration.hpp"
#include "tattle/curations/catcuration.hpp"
#include "tattle/curations/randomcuration.hpp"
#include <chrono>
#include <fmt/chrono.h>

namespace tattletale
{
    Curator::Curator(const Chronicle &chronicle, const Setting &setting) : chronicle_(chronicle), setting_(setting) {}

    Kernel *Curator::RecursivelyFindUnlikeliestReason(Kernel *to_check, Kernel *current_best)
    {
        if (to_check->GetChance() < current_best->GetChance())
        {
            current_best = to_check;
        }
        auto &reasons = to_check->GetReasons();
        for (auto &reason : reasons)
        {
            current_best = RecursivelyFindUnlikeliestReason(reason, current_best);
        }
        return current_best;
    }

    Kernel *Curator::RecursivelyFindUnlikeliestConsequence(Kernel *to_check, Kernel *current_best, size_t depth) const
    {
        if (!current_best || to_check->GetChance() < current_best->GetChance())
        {
            current_best = to_check;
        }
        if (depth > 0)
        {
            auto &consequences = to_check->GetConsequences();
            for (auto &consequence : consequences)
            {
                current_best = RecursivelyFindUnlikeliestConsequence(consequence, current_best, depth - 1);
            }
        }
        return current_best;
    }

    Resource *Curator::FindBlockingResource(Kernel *kernel) const
    {
        if (kernel->type_ != KernelType::kInteraction)
        {
            return nullptr;
        }
        auto interaction = dynamic_cast<Interaction *>(kernel);
        auto tendency = interaction->GetTendency();
        float lowest_influence = 1.0f;
        Resource *blocking_resource = nullptr;
        for (int type_index = 0; type_index < static_cast<int>(EmotionType::kLast); ++type_index)
        {
            float value = tendency->emotions[type_index];
            EmotionType type = static_cast<EmotionType>(type_index);
            if (value == 0)
            {
                continue;
            }
            auto last_emotion = chronicle_.GetLastEmotionOfType(interaction->tick_, interaction->GetOwner()->id_, type);
            float current_influence = 0;
            if (last_emotion)
            {
                current_influence = last_emotion->GetValue() * value;
            }
            if (current_influence < lowest_influence)
            {
                blocking_resource = last_emotion;
                lowest_influence = current_influence;
            }
        }
        if (tendency->wealth != 0)
        {
            auto last_wealth = chronicle_.GetLastWealth(interaction->tick_, interaction->GetOwner()->id_);
            float current_influence = last_wealth->GetValue() * tendency->wealth;
            if (current_influence < lowest_influence)
            {
                blocking_resource = last_wealth;
                lowest_influence = current_influence;
            }
        }
        return blocking_resource;
    }

    bool Curator::HasCausalConnection(Kernel *start, Kernel *end) const
    {
        std::queue<Kernel *> queue;
        queue.push(start);
        while (queue.size() > 0)
        {
            Kernel *current = queue.front();
            queue.pop();
            for (auto &reason : current->GetReasons())
            {
                if (reason->id_ == end->id_)
                {
                    return true;
                }

                if (reason->tick_ <= end->tick_)
                {
                    queue.push(reason);
                }
            }
        }
        return false;
    }

    std::vector<Kernel *> Curator::FindCausalConnection(Kernel *start, Kernel *end) const
    {
        std::vector<Kernel *> causal_chain;
        RecursivelyFindCausalConnectionBackwards(end, start, causal_chain);
        return causal_chain;
    }

    bool Curator::RecursivelyFindCausalConnectionBackwards(Kernel *root, Kernel *start, std::vector<Kernel *> &out_causal_chain) const
    {
        if (root->id_ == start->id_)
        {
            out_causal_chain.push_back(root);
            return true;
        }
        if (root->tick_ < start->tick_)
        {
            return false;
        }
        for (auto &reason : root->GetReasons())
        {
            if (RecursivelyFindCausalConnectionBackwards(reason, start, out_causal_chain))
            {
                out_causal_chain.push_back(root);
                return true;
            }
        }
        return false;
    }

    std::string Curator::GetTimeDescription(Kernel *start, Kernel *end, bool first_letter_uppercase) const
    {
        bool reversed = (start->tick_ > end->tick_);
        size_t tick_distance = 0;
        if (reversed)
        {
            tick_distance = start->tick_ - end->tick_;
        }
        else
        {
            tick_distance = end->tick_ - start->tick_;
        }
        std::string description = (!reversed ? "quite a lot of time later" : "quite a lot of time earlier");

        size_t days = tick_distance / (setting_.courses_per_day + 1);
        if (tick_distance == 0)
        {
            description = "at the same time";
        }
        else if (tick_distance < (setting_.courses_per_day + 1) / 2)
        {
            description = (!reversed ? "shortly after" : "shortly before");
        }
        else if (tick_distance < setting_.courses_per_day + 1)
        {
            description = (!reversed ? "later that day" : "earlier that day");
        }
        else if (days <= 1)
        {
            description = (!reversed ? "the following day" : "the previous day");
        }
        else if (days <= 7)
        {
            description = "during the same week";
        }
        else if (days <= 14)
        {
            description = (!reversed ? "about two weeks later" : "about two weeks earlier");
        }
        else if (days <= 31)
        {
            description = "during the same month";
        }
        if (first_letter_uppercase)
        {
            description[0] = toupper(description[0]);
        }
        return description;
    }

    std::string Curator::GetChanceDescription(float chance) const
    {
        if (chance < 0.1)
        {
            return "impossibly rare";
        }
        if (chance < 0.2)
        {
            return "rare";
        }
        if (chance < 0.3)
        {
            return "highly unlikely";
        }
        if (chance < 0.4)
        {
            return "unlikely";
        }
        if (chance < 0.5)
        {
            return "slightly unlikely";
        }
        if (chance < 0.6)
        {
            return "normal";
        }
        if (chance < 0.7)
        {
            return "somewhat common";
        }
        if (chance < 0.8)
        {
            return "common";
        }
        if (chance < 0.9)
        {
            return "very common";
        }
        return "completely banal";
    }

    std::string Curator::GenerateStatusDescription(const ActorStatus &start_status, const std::vector<Kernel *> &kernels) const
    {
        std::string description = fmt::format("{}.", *start_status.goal);

        size_t relevant_emotion_count = 0;
        std::string previous_adjective = "";
        for (auto &emotion : start_status.emotions)
        {
            bool relevant = false;
            for (auto &kernel : kernels)
            {
                if (kernel->IsReason(emotion->id_))
                {
                    relevant = true;
                }
            }
            if (relevant)
            {
                std::string adjective = emotion->GetAdjective();

                if (relevant_emotion_count > 0)
                {
                    std::string adjective_description = "";
                    if (adjective != previous_adjective)
                    {
                        adjective_description = fmt::format("{} ", adjective);
                        ;
                    }
                    description += fmt::format(" and {}{}", adjective_description, emotion->GetNameVariant());
                }
                else
                {
                    description += fmt::format(" At the start of the story they were {:p}", *emotion);
                }
                previous_adjective = adjective;
                ++relevant_emotion_count;
            }
        }
        if (relevant_emotion_count > 0)
        {
            description += ".";
        }
        description += fmt::format(" {} was also {} {}.\n", *start_status.wealth->GetOwner(), start_status.wealth->GetAdjective(), start_status.wealth->GetNameVariant());
        return description;
    }
    std::string Curator::GenerateScoreDescription(float score) const
    {
        if (score < 0.1)
        {

            return "very boring";
        }
        if (score < 0.2)
        {
            return "boring";
        }
        if (score < 0.3)
        {
            return "somewhat boring";
        }
        if (score < 0.4)
        {
            return "generic";
        }
        if (score < 0.5)
        {
            return "slightly interesting";
        }
        if (score < 0.6)
        {
            return "interesting";
        }
        if (score < 0.7)
        {
            return "highly interesting";
        }
        if (score < 0.8)
        {
            return "fascinating";
        }
        if (score < 0.9)
        {
            return "highly fascinating";
        }
        return "mindblowingly fascinating";
    }

    std::string Curator::GetResourceReasonDescription(Resource *resource) const
    {
        float value = abs(resource->GetValue());
        if (value > 0.9f)
        {
            return fmt::format("resigning to their fate of {:p}, they just had to", *resource);
        }
        if (value > 0.7f)
        {
            return fmt::format("{:p} compelled them to", *resource);
        }
        if (value > 0.5f)
        {
            return fmt::format("them {:p} led them to", *resource);
        }
        if (value > 0.4f)
        {
            return fmt::format("because they were {:p} they had a feeling that they needed to", *resource);
        }
        if (value > 0.2f)
        {
            return fmt::format("{:p} gave them a slight excuse to", *resource);
        }
        return fmt::format("{:p} gave them a tiny nudge to", *resource);
    }

    Actor *Curator::FindMostOccuringActor(const std::vector<Kernel *> &kernels, bool &out_more_actors_present) const
    {
        std::set<size_t> checked_actors;
        Actor *highest_actor = nullptr;
        size_t highest_occurence = 0;
        for (auto kernel : kernels)
        {
            auto actors = kernel->GetAllParticipants();
            for (auto &actor : actors)
            {
                size_t occurence = 0;
                if (checked_actors.find(actor->id_) == checked_actors.end())
                {
                    checked_actors.insert(actor->id_);
                    for (auto kernel_to_check : kernels)
                    {
                        auto actors_to_check = kernel_to_check->GetAllParticipants();
                        for (auto &actor_to_check : actors_to_check)
                        {
                            if (actor_to_check->id_ == actor->id_)
                            {
                                ++occurence;
                            }
                        }
                    }
                    if (occurence > highest_occurence)
                    {
                        highest_occurence = occurence;
                        highest_actor = actor;
                    }
                }
            }
        }
        out_more_actors_present = (checked_actors.size() > 1);
        return highest_actor;
    }

    std::string Curator::UseAllCurations()
    {
        TATTLETALE_DEBUG_PRINT("START CURATION");

        const auto &chains = chronicle_.GetEveryPossibleChain(setting_.max_chain_size);

        std::string preamble = "{} Curation:\n\n{}\n\n.....................................................................\n\n";

        std::string narrative = "";

        std::vector<Curation *> curations;
        curations.push_back(new RarityCuration(setting_.max_chain_size));
        curations.push_back(new AbsoluteInterestCuration(setting_.max_chain_size));
        curations.push_back(new TagCuration(setting_.max_chain_size));
        curations.push_back(new RandomCuration(setting_.max_chain_size, chronicle_.GetRandom()));

        for (auto &curation : curations)
        {
            TATTLETALE_DEBUG_PRINT(fmt::format("{} Curation...", curation->name_));
            narrative += fmt::format(preamble, curation->name_, Curate(chains, curation));
        }
        for (auto &curation : curations)
        {
            delete curation;
        }
        curations.clear();

        return narrative;
    }

    std::string Curator::Narrativize(const std::vector<Kernel *> &chain, const Curation *curation) const
    {
        std::set<size_t> named_actors;
        Actor::named_actors_ = &named_actors;

        Kernel *first_noteworthy_event = curation->GetFirstNoteworthyEvent(chain);
        Kernel *second_noteworthy_event = curation->GetSecondNoteworthyEvent(chain);
        bool more_than_one_actor_present = false;
        auto protagonist = FindMostOccuringActor(chain, more_than_one_actor_present);
        bool noteworthy_for_protag = false;
        bool ignore_protag = false;
        for (auto &participant : second_noteworthy_event->GetAllParticipants())
        {
            if (participant->id_ == protagonist->id_)
            {
                noteworthy_for_protag = true;
            }
        }
        if (!noteworthy_for_protag)
        {
            ignore_protag = true;
        }

        noteworthy_for_protag = false;
        for (auto &participant : first_noteworthy_event->GetAllParticipants())
        {
            if (participant->id_ == protagonist->id_)
            {
                noteworthy_for_protag = true;
            }
        }
        if (!noteworthy_for_protag)
        {
            ignore_protag = true;
        }

        if (!first_noteworthy_event || !second_noteworthy_event)
        {
            return "Narrativization failed. No noteworthy events were created.";
        }

        std::string acquaintance_description = (more_than_one_actor_present ? " and their acquaintances" : "");

        std::string description = "";

        float score = curation->CalculateScore(chain);
        std::string score_description = GenerateScoreDescription(score);

        auto normal_interaction = chronicle_.FindMostOccuringInteractionPrototypeForActorBeforeTick(protagonist->id_, chain[0]->tick_);
        if (normal_interaction)
        {
            description += fmt::format("Normally one would find {} {:p} but this time s", *protagonist, *normal_interaction);
        }
        else
        {
            description += "S";
        }
        bool only_one_noteworthy_event = first_noteworthy_event->id_ == second_noteworthy_event->id_;
        std::string event_count_description = fmt::format((only_one_noteworthy_event ? "omething {}" : "ome {} events"), score_description);
        description += fmt::format("{} happened around them{} that I want to tell you about.\n", event_count_description, acquaintance_description);

        auto protagonist_start_status = chronicle_.FindActorStatusDuringTick(protagonist->id_, chain[0]->tick_);
        std::string status_description = GenerateStatusDescription(protagonist_start_status, chain);
        description += fmt::format("But first let's take a quick look at our protagonist. {}\n", status_description);

        description += fmt::format("One interesting thing that happened was {} {:p}", *(first_noteworthy_event->GetOwner()), *first_noteworthy_event);
        if (!only_one_noteworthy_event)
        {
            auto time_description = GetTimeDescription(first_noteworthy_event, second_noteworthy_event, false);

            auto blocking_resource = FindBlockingResource(second_noteworthy_event);
            std::string blocking_description = "";
            if (blocking_resource)
            {
                blocking_description = fmt::format(" despite {:p},", *blocking_resource);
            }
            description += fmt::format(", but this was not even the most noteworthy thing that happened because {}{} {} was{} found {:p}{}.\n\n",
                                       time_description,
                                       blocking_description,
                                       *(second_noteworthy_event->GetOwner()),
                                       (first_noteworthy_event->GetOwner()->id_==second_noteworthy_event->GetOwner()->id_?" also":""),
                                       *second_noteworthy_event,
                                       (second_noteworthy_event->IsSameSpecificType(first_noteworthy_event) ? " again" : ""));
        }
        else
        {
            description += ".\n\n";
        }

        description += "So how exactly did this happen?\n";
        auto previous_kernel = chain[0];
        description += fmt::format("The whole sequence of events started when {}", *previous_kernel);
        auto previous_reasons = previous_kernel->GetReasons();
        if (previous_reasons.size() > 0)
        {
            bool no_reason_yet=true;
            for (size_t reason_index = 0; reason_index < previous_reasons.size(); ++reason_index)
            {
                auto &reason = previous_reasons[reason_index];
                if(reason->IsSameSpecificType(previous_kernel)){
                    continue;
                }
                if (no_reason_yet)
                {
                    if (reason->type_ == KernelType::kInteraction)
                    {
                        description += fmt::format(", because {}", *reason);
                    }
                    else
                    {
                        description += fmt::format(", because {} was {:p}", *previous_kernel->GetOwner(), *reason);
                    }
                    no_reason_yet = false;
                }
                else
                {
                    description += fmt::format(" and was also {:p}", *reason);
                }
            }
        }
        for (size_t index = 1; index < chain.size(); ++index)
        {
            Kernel *kernel = chain[index];
            if (kernel->IsSameSpecificType(previous_kernel))
            {
                previous_reasons = kernel->GetReasons();
                previous_kernel = kernel;
                continue;
            }
            if (kernel->type_ == KernelType::kInteraction)
            {
                if (index <= 2)
                {
                    if (chain[index]->IsSameSpecificType(chain[0]))
                    {
                        ++index;
                        continue;
                    }
                }
                int offset =0;
                while(index+offset+2< chain.size() && chain[index+offset+2]->IsSameSpecificType(chain[index+offset+1])){
                    ++offset;
                }
                if (index +offset+ 2 < chain.size())
                {
                    if (chain[index + offset + 2]->IsSameSpecificType(kernel))
                    {
                        index+=offset;
                        ++index;
                        continue;
                    }
                }
                bool protag_included = false;
                for (auto &participant : kernel->GetAllParticipants())
                {
                    if (participant->id_ == protagonist->id_)
                    {
                        protag_included = true;
                    }
                }
                if (!protag_included && !ignore_protag)
                {
                    {
                        ++index;
                        continue;
                    }
                }
                description += ".\nThis ";
                for (auto &reason : kernel->GetReasons())
                {
                    if (reason->id_ != previous_kernel->id_ && !reason->IsSameSpecificType(kernel))
                    {
                        if (reason->GetOwner()->id_ == kernel->GetOwner()->id_)
                        {
                            description += fmt::format("and them {:p} ", *reason);
                        }
                        else
                        {
                            description += fmt::format("and {} {:p} ", *(reason->GetOwner()), *reason);
                        }
                    }
                }
                description += fmt::format("made {} {:a}", *(kernel->GetOwner()), *kernel);
            }
            else
            {
                description += " which in turn ";

                bool compound_reason = false;
                for (auto &previous_reason : previous_reasons)
                {
                    if (previous_reason->IsSameSpecificType(kernel) && previous_reason->GetOwner()->id_ == kernel->GetOwner()->id_)
                    {
                        compound_reason = true;
                    }
                }
                float reduced_value = 0;
                if (previous_kernel->type_ == KernelType::kInteraction)
                {
                    for (auto &consequence : previous_kernel->GetConsequences())
                    {
                        if (consequence->IsSameSpecificType(kernel))
                        {
                            for (auto &reason : consequence->GetReasons())
                            {
                                if (reason->IsSameSpecificType(kernel))
                                {
                                    reduced_value = dynamic_cast<Resource *>(reason)->GetValue() - dynamic_cast<Resource *>(consequence)->GetValue();
                                }
                            }
                        }
                    }
                }
                std::string other_participant = "";
                std::string trajectory = "made";
                std::string trajectory_accessory = (reduced_value>0 ? " less" : " more");
                if (kernel->type_ == KernelType::kRelationship )
                {
                    other_participant = fmt::format(" for {}", *kernel->GetAllParticipants()[1]);
                    trajectory = (reduced_value>0 ? "reduced" : "increased");
                    trajectory_accessory = "'s";
                }
                if(kernel->type_ == KernelType::kResource){
                    trajectory = (reduced_value>0 ? "reduced" : "increased");
                    trajectory_accessory = "'s";
                }
                if(reduced_value!=0){
                description += fmt::format("{} {}{} {}{}{}",
                                           trajectory,
                                           *(kernel->GetOwner()),
                                           trajectory_accessory,
                                           kernel->name_,
                                           other_participant,
                                           (compound_reason ? " even more" : ""));
                }else{
                    description += fmt::format("confirmed {} in {:p}",*(kernel->GetOwner()),*kernel);
                }
            }
            previous_reasons = kernel->GetReasons();
            previous_kernel = kernel;
            while(index+1<chain.size() && chain[index+1]->IsSameSpecificType(kernel)){
                ++index;
            }
        }

        Actor::named_actors_ = nullptr;
        description+=".";
        return description;
    }

    std::string Curator::Curate(const std::vector<std::vector<Kernel *>> &chains, Curation *curation) const
    {
        auto chain = FindBestScoringChain(chains, curation);
        if (chain.size() < 0)
        {
            return fmt::format("{} Curation failed. No valid Kernels were created.", curation->name_);
        }
        return Narrativize(chain, curation);
    }

    std::vector<Kernel *> Curator::FindBestScoringChain(const std::vector<std::vector<Kernel *>> chains, Curation *curation) const
    {
        float highest_score = 0.0f;
        std::vector<Kernel *> highest_chain;

        #ifdef TATTLETALE_PROGRESS_PRINT_OUTPUT
        size_t count = 0;
        size_t chain_amount = chains.size();
        auto t1 = std::chrono::steady_clock::now();
        std::chrono::nanoseconds sum = std::chrono::nanoseconds(0);
        size_t duration_count = 0;
        std::string spaces = " ";
        for(int i = 0; i< (std::string("Absolute Interest").length()-curation->name_.length());++i){
            spaces+=" ";
        }
        #endif //TATTLETALE_PROGRESS_PRINT_OUTPUT

        for (size_t index = 0; index < chains.size(); ++index)
        {
            float score = curation->CalculateScore(chains[index]);
            if (score > highest_score)
            {
                highest_score = score;
                highest_chain = chains[index];
            }

#ifdef TATTLETALE_PROGRESS_PRINT_OUTPUT
            ++count;
            if (count >= 1000 || index == chain_amount - 1)
            {
                count = 0;
                auto t2 = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
                ++duration_count;
                t1 = t2;
                sum += duration;
                auto average = sum / duration_count;
                auto max_duration = (chain_amount/1000.0f)*average;

                double progress = static_cast<double>(sum.count())/static_cast<double>(max_duration.count());

                std::string progress_string ="[";

                for (size_t i = 0; i < 30; ++i)
                {
                    if (i == 13)
                    {
                        if(progress<0.1){
                            progress_string += "0";
                        }
                        if(progress<1.0){
                            progress_string += "0";
                        }
                        progress_string += std::to_string(static_cast<int>(progress * 100));
                        progress_string += "%";
                    }
                    else if (i < 16 && i >= 13)
                    {
                    }
                    else if (i < 30 * progress)
                    {
                        progress_string += "|";
                    }
                    else
                    {
                        progress_string += " ";
                    }
                }
                progress_string +="]";
                
                auto time_left = std::chrono::floor<std::chrono::seconds>(max_duration-sum);
                TATTLETALE_PROGRESS_PRINT(fmt::format("| {} Scoring:{}{} {:%M:%S}|",curation->name_,spaces, progress_string, (time_left.count()>0?time_left:std::chrono::floor<std::chrono::seconds>(sum))));
            }
#endif //TATTLETALE_PROGRESS_PRINT_OUTPUT
        }
#ifdef TATTLETALE_PROGRESS_PRINT_OUTPUT
        std::cout << "\n";
#endif // TATTLETALE_PROGRESS_PRINT_OUTPUT
        return highest_chain;
    }
} // namespace tattletale
