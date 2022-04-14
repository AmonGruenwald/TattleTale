#ifndef TATTLE_CURATIONS_CURATION_H
#define TATTLE_CURATIONS_CURATION_H

#include "shared/kernels/kernel.hpp"
#include <vector>

namespace tattletale
{
    class Curation
    {
    public:
        virtual float CalculateScore(const std::vector<Kernel *> &chain) const = 0;
        virtual Kernel *GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const = 0;
        virtual Kernel *GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const = 0;
        const std::string name_;

    protected:
        Curation(std::string name, size_t max_chain_size) : name_(name), max_chain_size_(max_chain_size){};
        const size_t max_chain_size_;
    };
} // namespace tattletale
#endif // TATTLE_CURATIONS_CURATION_H