#include "shared/tattletalecore.hpp"
#include "shared/actor.hpp"
#include "shared/kernels/kernel.hpp"

#include <iostream>

namespace tattletale
{
    Kernel::Kernel(
        std::string name, size_t id,
        size_t tick,
        Actor *owner,
        std::vector<Kernel *> reasons,
        KernelType type)
        : name_(name),
          id_(id),
          tick_(tick),
          owner_(owner),
          reasons_(reasons),
          type_(type)
    {
    }

    Kernel::~Kernel(){};
    void Kernel::AddConsequence(Kernel *consequence)
    {
        consequences_.push_back(consequence);
    }
    const std::vector<Kernel *> &Kernel::GetConsequences() const
    {
        return consequences_;
    }

    float Kernel::GetChance() const { return 1.0f; }

    std::string Kernel::GetDetailedDescription() const
    {
        return fmt::format("#{} {} ({}) owned by {}", id_, name_, type_, *owner_);
    }
    const std::vector<Kernel *> &Kernel::GetReasons() const
    {
        return reasons_;
    }
    Actor *Kernel::GetOwner() const
    {
        return owner_;
    }

    std::vector<Actor *> Kernel::GetAllParticipants() const
    {
        std::vector<Actor *> participants;
        participants.push_back(owner_);
        return participants;
    }

    size_t Kernel::GetAbsoluteInterestScore() const
    {
        return 0;
    }

    bool Kernel::IsNegativeReason(Kernel *reason) const
    {
        return false;
    }
    bool Kernel::IsReason(size_t id) const{
        for(auto& reason: reasons_){
            if(reason->id_==id){
                return true;
            }
        }
        return false;
    }

    bool Kernel::IsSameKernelType(Kernel *other) const
    {
        return (type_ == other->type_);
    }
} // namespace tattletale