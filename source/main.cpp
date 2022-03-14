#include "tale/tale.hpp"
#include "tattle/tattle.hpp"

int main(int argc, char *argv[])
{
    tattletale::Setting setting;
    setting.days_to_simulate = 15;
    setting.actor_count = 100;
    setting.actors_per_course = 15;
    tattletale::Random random(setting.seed);
    tattletale::Chronicle chronicle(random);
    tattletale::Tale(chronicle, random, setting);
    tattletale::Tattle(chronicle, setting);
    tattletale::Tale(chronicle, random, setting);
    tattletale::Tattle(chronicle, setting);
    return 0; // optional return value
}