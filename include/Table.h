#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <ctime>

class Table
{
private:
    int number = -1;
    int total_time_min = 0;
    int recent_start_time_min = -1;
    int hours_to_be_paid = 0;
    std::string customer_id;

public:
    explicit Table(int table_number) : number{table_number} {};

    bool is_available() const;
    int minutes_to_full_hours(int minutes_to_pay);
    bool free_table(const std::tm &end_time);
    int total_minutes_occupied() const;
    bool occupy_table(const std::string &new_customer_id, const std::tm &time);
    int hours_to_pay();
    std::string current_customer();
};

#endif // TABLE_H