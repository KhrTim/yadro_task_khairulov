#include "../include/Table.h"

bool Table::is_available() const { return customer_id.size() == 0; }

int Table::minutes_to_full_hours(int minutes_to_pay)
{
    int res = minutes_to_pay / 60;
    if ((minutes_to_pay - res * 60) > 0)
        res += 1;
    return res;
}

bool Table::free_table(const std::tm &end_time)
{
    int time_point_min = end_time.tm_hour * 60 + end_time.tm_min;
    int day_end_min = 24 * 60;
    int time_diff_min = 0;
    if (time_point_min < recent_start_time_min)
    {
        time_diff_min = day_end_min - recent_start_time_min + time_point_min;
        total_time_min += time_diff_min;
    }
    else
    {
        time_diff_min = time_point_min - recent_start_time_min;
        total_time_min += time_diff_min;
    }
    hours_to_be_paid += minutes_to_full_hours(time_diff_min);
    recent_start_time_min = -1;
    customer_id.clear();
    return true;
}

int Table::total_minutes_occupied() const
{
    return total_time_min;
}

bool Table::occupy_table(const std::string &new_customer_id, const std::tm &time)
{
    if (is_available())
    {
        customer_id = new_customer_id;
        recent_start_time_min = time.tm_hour * 60 + time.tm_min;
        return true;
    }
    else
    {
        return false;
    }
}

int Table::hours_to_pay() { return hours_to_be_paid; }

std::string Table::current_customer() { return customer_id; }