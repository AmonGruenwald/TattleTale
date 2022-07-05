#include "tale/tale.hpp"
#include "tattle/tattle.hpp"
#include <fstream>
#include <chrono>
#include <fmt/format.h>
#include <ctime>
#include <time.h>

//#define TRIAL_RUN

void WriteToFile(const std::string &string, const std::string &path)
{
    std::ofstream out_file;
    out_file.open(path, std::ios_base::out); // append instead of overwrite
    out_file << string;
    out_file.close();
}

int main(int argc, char *argv[])
{
    time_t t = time(0); // get time now
    struct tm *now = std::localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S.txt", now);
    std::string path = std::string(buffer);
    strftime(buffer, 80, "%c", now);
    std::string time_string = std::string(buffer);

    std::string result = "=====================================================================\n";
    
    result += time_string;
    result += "\n";

    tattletale::Setting setting;
#ifdef TRIAL_RUN
    setting.days_to_simulate = 10;
    setting.actor_count = 50;
    setting.actors_per_course = 30;
    setting.max_chain_size = 5;
    size_t run_amount = 1;
#else
    setting.days_to_simulate = 30;
    setting.actor_count = 300;
    setting.actors_per_course = 30;
    setting.max_chain_size = 10;
    size_t run_amount = 9;
#endif // TRIAL_RUN

    tattletale::Random random;
    tattletale::Chronicle chronicle(random);

    for (size_t i = 1; i < run_amount+1; ++i)
    {
        std::cout << "\n";
        setting.seed = i;
        random.Seed(setting.seed);
        std::string run_string = "---------------------------------------------------------------------\n";
        run_string += "|                              RUN #";
        run_string += std::to_string(i);
        run_string += "                               |\n";
        run_string += "---------------------------------------------------------------------\n";
        TATTLETALE_PROGRESS_PRINT(run_string);
        result += run_string;
        result += fmt::format("Setting:\n{} ", setting);
        result += "---------------------------------------------------------------------\n";
        tattletale::Tale(chronicle, random, setting);
        result += tattletale::Tattle(chronicle, setting);
        TATTLETALE_PROGRESS_PRINT("---------------------------------------------------------------------\n");
        result += "\n\n";
        WriteToFile(result,path);
    }
    result += "=====================================================================\n\n\n\n";
    WriteToFile(result, path);
    std::cout << result;
    return 0;
}