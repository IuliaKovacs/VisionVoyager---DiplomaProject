#ifndef PERSON_H
#define PERSON_H

#include <string>

using namespace std;

enum class Role 
{
    Secretary,
    Receptionist,
    Building_Staff
};

class Person
{   
private:
    string id; /* "s1"/"s2"/"s3"/"s4"/ */
    string first_name;
    string last_name;
    Role role;

public:
    Person(string i, string f, string l, Role r) : id(i), first_name(f), last_name(l), role(r) { }
    
    Person(string i, string f, string l, string r) : id(i), first_name(f), last_name(l)
    {
         if(r == "Secretary")
        {
            role = Role::Secretary;
        }
        else if(r == "Receptionist")
        {
            role = Role::Receptionist;
        }
        else if(r == "Building Staff")
        {
            role = Role::Building_Staff;
        }
    }

    string get_id()
    {
        return id;
    }
    
    string get_first_name()
    {
        return first_name;
    }

    string get_last_name()
    {
        return last_name;
    }

    string get_role()
    {   
        switch(role) 
        {
            case Role::Secretary: 
                return "Secretary";
            case Role::Receptionist: 
                return "Receptionist";
            case Role::Building_Staff: 
                return "Building Staff";
            default: 
                return "Unknown";
        }
    }
};

#endif //PERSON_H