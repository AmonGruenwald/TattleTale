#include "tale/tale.hpp"
#include "tattle/tattle.hpp"
#include <fstream>
#include <chrono>
#include <fmt/format.h>
#include <ctime> 

void AppendStringToFile(const std::string &string, const std::string &path)
{
    std::ofstream out_file;
    out_file.open(path, std::ios_base::app); // append instead of overwrite
    out_file << string;
}
int main(int argc, char *argv[])
{
    tattletale::Setting setting;
    setting.days_to_simulate = 7;
    setting.actor_count = 30;
    setting.actors_per_course = 30;
    setting.max_chain_size = 5;
    tattletale::Random random(setting.seed);
    tattletale::Chronicle chronicle(random);

    std::string path = "results.txt";
    size_t run_amount = 1;


    std::string result = "===============================================\n";
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    result+=std::string(std::ctime(&time));
    result+="\n";
    for (size_t i = 0; i < run_amount; ++i)
    {
        setting.seed = i;
        random.Seed(setting.seed);
        tattletale::Tale(chronicle, random, setting);
        result += "-----------------------------------------------\n";
        result += "|                   RUN # ";
        result += std::to_string(i);
        result += "                   |\n";
        result += "-----------------------------------------------\n";
        result+=fmt::format("Setting:\n{}", setting);
        result += "-----------------------------------------------\n";
        result += tattletale::Tattle(chronicle, setting);
        result += "\n\n";
    }
    result += "===============================================\n\n\n\n";
    AppendStringToFile(result, path);
    std::cout << result;
    return 0; 
}