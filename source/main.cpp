#include "tale/tale.hpp"
#include "tattle/tattle.hpp"

int main(int argc, char *argv[])
{
    tattletale::Setting setting;
    tattletale::Run(setting);
    tattle::TattlePrintExample();
    return 0; // optional return value
}