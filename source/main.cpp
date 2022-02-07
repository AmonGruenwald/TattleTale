#include "tale/tale.hpp"
#include "tattle/tattle.hpp"

int main(int argc, char *argv[])
{
    tattle::TattlePrintExample();
    tale::TalePrintExample();
    tale::Kernel::current_number_ = 0;
    return 0; // optional return value
}