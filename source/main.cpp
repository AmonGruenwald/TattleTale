#include "tale/tale.hpp"
#include "tattle/tattle.hpp"

int main(int argc, char *argv[])
{
    tattletale::Setting setting;
    setting.days_to_simulate = 3;
    setting.actor_count = 50;
    setting.actors_per_course = 10;
    tattletale::Random random(setting.seed);
    tattletale::Chronicle chronicle(random);
    tattletale::Tale(chronicle, random, setting);
    TATTLETALE_DEBUG_PRINT(chronicle.GetKernelAmount());
    tattletale::Tattle(chronicle, setting);
    return 0; // optional return value
}