#include "tale/tale.hpp"
#include "tattle/tattle.hpp"

int main(int argc, char *argv[])
{
    tattletale::Setting setting;
    setting.days_to_simulate = 5;
    auto &chronicle = tattletale::Run(setting);
    tattletale::Curate(chronicle, setting);
    return 0; // optional return value
}