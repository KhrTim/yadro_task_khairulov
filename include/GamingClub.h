#ifndef GAMINGCLUB_H
#define GAMINGCLUB_H

#include <vector>
#include <deque>
#include <ctime>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Table.h"
#include "Customer.h"


class GamingClub
{
    std::vector<Table> tables;
    std::tm opening_time = {};
    std::tm closing_time = {};
    int cost_per_hour{-1};
    std::vector<std::string> logging;
    std::deque<std::string> wait_list;
    std::unordered_map<std::string, Customer> customers_in_club;

    bool customer_came(const std::tm &e_time, const std::string &customer_id);
    void put_customer_into_wait_list(const std::string &time_str, const std::tm &e_time, const std::string &customer_id);
    void customer_left_club(const std::string &time_str, const std::tm &e_time, const std::string &customer_id);
    void customer_got_table(const std::string &time_str, const std::tm &e_time, const std::string &customer_id, int table_num);
    void customer_entered_club(const std::string &time_str, const std::tm &e_time, const std::string &customer_id);
    bool customer_in_wait_list(const std::string &customer_id);

public:
    GamingClub(int tables_number, std::tm opening_time, std::tm closing_time, int cost_per_hour);

    std::vector<std::string> get_logs();
    void process_event(const std::tm &e_time, int e_id, const std::string &customer_id, int table_num);
    void close();
};

#endif //GAMINGCLUB_H