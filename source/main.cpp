#include "tale/tale.hpp"
#include "tattle/tattle.hpp"

int main(int argc, char *argv[])
{
    tale::InteractionStore store;
    tattle::TattlePrintExample();
    tale::TalePrintExample();
    return 0; // optional return value
}