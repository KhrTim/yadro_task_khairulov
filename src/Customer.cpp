#include "../include/Customer.h"

std::string Customer::get_id() const
{
    return id;
}

bool Customer::operator<(const Customer &other) const
{
    return this->id < other.id;
}

bool Customer::operator==(const Customer &other) const
{
    return this->id == other.id;
}

int Customer::get_table_num() const { return table_num; }

void Customer::change_table(int new_table) { table_num = new_table; }

struct CustomerHash
{
    size_t operator()(const Customer &c) const
    {
        return std::hash<std::string>()(c.get_id());
    }
};