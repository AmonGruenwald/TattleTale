#include "tale/tale.hpp"
#include "tattle/tattle.hpp"
#include <fstream>

void AppendStringToFile(const std::string &string, const std::string &path)
{
    std::ofstream out_file;
    out_file.open(path, std::ios_base::app); // append instead of overwrite
    out_file << string;
}
int main(int argc, char *argv[])
{
    tattletale::Setting setting;
    setting.days_to_simulate = 20;
    setting.actor_count = 50;
    setting.actors_per_course = 15;
    tattletale::Random random(setting.seed);
    tattletale::Chronicle chronicle(random);

    std::string path = "results.txt";
    for (size_t i = 0; i < 10; ++i)
    {
        random.Seed(i);
        tattletale::Tale(chronicle, random, setting);
        std::string result = "";
        result += "===============================================\n";
        result += "||                  RUN # ";
        result += std::to_string(i);
        result += "                  ||\n";
        result += "===============================================\n";
        result += tattletale::Tattle(chronicle, setting);
        result += "\n";
        AppendStringToFile(result, path);
    }
    return 0; // optional return value
}