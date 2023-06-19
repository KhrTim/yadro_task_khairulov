#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include "../include/Customer.h"
#include "../include/Table.h"
#include "../include/GamingClub.h"



std::vector<std::string> work(std::ifstream &input_file)
{
    std::string line;
    std::tm opening_time = {};
    std::tm closing_time = {};
    int tables_number;
    int price_per_hour;

    std::getline(input_file, line);
    std::istringstream iss_3(line);
    iss_3 >> tables_number;

    std::getline(input_file, line);
    std::istringstream iss_1(line);
    iss_1 >> std::get_time(&opening_time, "%H:%M") >> std::get_time(&closing_time, "%H:%M");

    std::getline(input_file, line);
    std::istringstream iss_2(line);
    iss_2 >> price_per_hour;

    GamingClub gc(tables_number, opening_time, closing_time, price_per_hour);
    int table_num;
    std::tm e_time = {};
    int e_id;
    std::string customer_id;
    while (std::getline(input_file, line))
    {
        std::istringstream iss(line);
        iss >> std::get_time(&e_time, "%H:%M");
        iss >> e_id;
        if (e_id == 2)
        {
            iss >> customer_id >> table_num;
        }
        else
        {
            iss >> customer_id;
        }
        gc.process_event(e_time, e_id, customer_id, table_num);
    }
    gc.close();
    return gc.get_logs();
}

int main(int argc, char* argv[])
{
    std::ifstream input_file(argv[1]);

    std::vector<std::string> logs;
    logs = work(input_file);

    std::ofstream output_file("output.txt");
    for (auto i : logs)
    {
        output_file << i << '\n';
    }

    return 0;
}