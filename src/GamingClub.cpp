#include "../include/GamingClub.h"

bool GamingClub::customer_came(const std::tm &e_time, const std::string &customer_id)
{
    if (customers_in_club.find(customer_id) == customers_in_club.end())
    {
        Customer c(customer_id);
        customers_in_club.emplace(customer_id, std::move(c));
        return true;
    }
    else
    {
        return false;
    }
}

GamingClub::GamingClub(int tables_number, std::tm opening_time, std::tm closing_time, int cost_per_hour) : opening_time{opening_time}, closing_time{closing_time}, cost_per_hour{cost_per_hour}
{
    for (int i = 0; i < tables_number; i++)
    {
        tables.push_back(Table(i + 1));
    }
    char time_str[10];
    std::strftime(time_str, sizeof time_str, "%H:%M", &opening_time);
    logging.push_back(time_str);
}

std::vector<std::string> GamingClub::get_logs() { return logging; }

void GamingClub::process_event(const std::tm &e_time, int e_id, const std::string &customer_id, int table_num = -1)
{
    char time_str[10];
    std::strftime(time_str, sizeof time_str, "%H:%M", &e_time);
    if (e_id == 1)
    {
        customer_entered_club(std::string(time_str), e_time, customer_id);
    }
    else if (e_id == 2)
    {
        customer_got_table(std::string(time_str), e_time, customer_id, table_num);
    }
    else if (e_id == 3)
    {
        put_customer_into_wait_list(std::string(time_str), e_time, customer_id);
    }
    else if (e_id == 4)
    {
        customer_left_club(std::string(time_str), e_time, customer_id);
    }
}

void GamingClub::put_customer_into_wait_list(const std::string &time_str, const std::tm &e_time, const std::string &customer_id)
{
    logging.push_back(std::string(time_str) + " 3 " + customer_id);
    int table_available = -1;
    for (int i = 0; i < tables.size(); i++)
    {
        if (tables[i].is_available())
        {
            table_available = i;
            break;
        }
    }
    if (wait_list.size() == tables.size())
    {
        logging.push_back(std::string(time_str) + " 11 " + customer_id);
    }
    else if (table_available != -1)
    {
        logging.push_back(std::string(time_str) + " 13 ICanWaitNoLonger");
        wait_list.push_back(customer_id);
    }
    else
    {
        wait_list.push_back(customer_id);
    }
}

void GamingClub::customer_left_club(const std::string &time_str, const std::tm &e_time, const std::string &customer_id)
{
    logging.push_back(std::string(time_str) + " 4 " + customer_id);
    if (customers_in_club.find(customer_id) == customers_in_club.end())
    {
        logging.push_back(std::string(time_str) + " 13 ClientUnknown");
    }
    else
    {
        int customer_table_num = customers_in_club[customer_id].get_table_num();
        if (customer_table_num != -1)
        {
            tables[customer_table_num - 1].free_table(e_time);
            if (wait_list.size() > 0)
            {
                auto next_customer = wait_list.front();
                wait_list.pop_front();
                tables[customer_table_num - 1].occupy_table(next_customer, e_time);
                customers_in_club[next_customer].change_table(customer_table_num);
                logging.push_back(std::string(time_str) + " 12 " + next_customer + " " + std::to_string(customer_table_num));
            }
        }
        customers_in_club.erase(customer_id);
    }
}

void GamingClub::customer_got_table(const std::string &time_str, const std::tm &e_time, const std::string &customer_id, int table_num)
{
    logging.push_back(std::string(time_str) + " 2 " + customer_id + " " + std::to_string(table_num));
    if (customers_in_club.find(customer_id) == customers_in_club.end())
    {
        logging.push_back(std::string(time_str) + " 13 ClientUnknown");
    }
    else if (tables[table_num - 1].is_available() && (customers_in_club[customer_id].get_table_num() != table_num))
    {
        if (wait_list.front() == customer_id)
        {
            wait_list.pop_front();
        }
        if (customers_in_club[customer_id].get_table_num() != -1)
        {
            tables[customers_in_club[customer_id].get_table_num() - 1].free_table(e_time);
        }
        customers_in_club[customer_id].change_table(table_num);
        tables[table_num - 1].occupy_table(customer_id, e_time);
    }
    else
    {
        logging.push_back(std::string(time_str) + " 13 PlaceIsBusy");
    }
}

void GamingClub::customer_entered_club(const std::string &time_str, const std::tm &e_time, const std::string &customer_id)
{
    int e_time_min = e_time.tm_hour * 60 + e_time.tm_min;
    int open_time_min = opening_time.tm_hour * 60 + opening_time.tm_min;
    int close_time_min = closing_time.tm_hour * 60 + closing_time.tm_min;

    logging.push_back(time_str + " 1 " + customer_id);
    // if ((e_time_min < open_time_min) && (e_time_min > close_time_min))
    if ((e_time_min > close_time_min) || (e_time_min < open_time_min))
    {
        logging.push_back(time_str + " 13 NotOpenYet");
    }
    else if (!customer_came(e_time, customer_id))
    {
        logging.push_back(time_str + " 13 YouShallNotPass");
    }
}

bool GamingClub::customer_in_wait_list(const std::string &customer_id)
{
    return std::find(wait_list.begin(), wait_list.end(), customer_id) != wait_list.end();
}

void GamingClub::close()
{
    char time_str[10];
    std::strftime(time_str, sizeof time_str, "%H:%M", &closing_time);
    std::vector<std::string> customers_inside;
    for (auto c : customers_in_club)
    {
        customers_inside.push_back(c.first);
    }
    std::sort(customers_inside.begin(), customers_inside.end());
    for (auto c : customers_inside)
    {
        logging.push_back(std::string(time_str) + " 11 " + customers_in_club[c].get_id());
    }
    logging.push_back(std::string(time_str));
    for (int i = 0; i < tables.size(); ++i)
    {
        if (!tables[i].is_available())
            tables[i].free_table(closing_time);

        std::tm table_time = {};
        table_time.tm_hour = tables[i].total_minutes_occupied() / 60;
        table_time.tm_min = tables[i].total_minutes_occupied() - table_time.tm_hour * 60;
        char time_str[10];
        std::strftime(time_str, sizeof time_str, "%H:%M", &table_time);

        logging.push_back(std::to_string(i + 1) + " " + std::to_string(tables[i].hours_to_pay() * cost_per_hour) + " " + std::string(time_str));
    }
}