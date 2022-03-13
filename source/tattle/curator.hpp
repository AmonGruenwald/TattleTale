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

    private:
        const Chronicle &chronicle_;
        const Setting &setting_;
    };

} // namespace tattletale
#endif // TATTLE_CURATOR_H