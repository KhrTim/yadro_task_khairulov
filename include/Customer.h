#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer
{
    std::string id;
    int table_num = -1;

public:
    Customer() {}
    explicit Customer(const std::string &id) : id{id} {}

    std::string get_id() const;
    bool operator<(const Customer &other) const;
    bool operator==(const Customer &other) const;
    int get_table_num() const;
    void change_table(int new_table);
};

#endif // CUSTOMER_H