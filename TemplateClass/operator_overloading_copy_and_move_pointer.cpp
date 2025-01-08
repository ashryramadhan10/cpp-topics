#include <iostream>
#include <string.h>

class Person {
private:
    // variables
    int id;
    char* name {nullptr};

public:

    // constructors
    Person();
    Person(int id, const char* name);
    Person(const Person& person);
    Person(Person&& person);

    // destructor
    ~Person();

    // operator overloadings
    Person& operator=(const Person& person);
    Person& operator=(Person&&) noexcept;
};

// constructors implementation
Person::Person(): id{0}, name{nullptr} {}

Person::Person(int id, const char* name) {
    this->id = id;

    this->name = new char[strlen(name)+1];
    memcpy(this->name, name, sizeof(*name)*strlen(name)+1);

    printf("Overloading: Person(%d, %s) created!\n", this->id, this->name);
}

Person::Person(const Person& person): Person(person.id, person.name) {
    printf("Copy: Person(%d, %s) created!\n", this->id, this->name);
}

Person::Person(Person&& person) {
    this->id = person.id;

    this->name = new char[strlen(name)+1];
    memcpy(this->name, name, sizeof(*name)*strlen(name)+1);
    person.name = nullptr;

    printf("Move: Person(%d, %s) created!\n", this->id, this->name);
}

Person::~Person() {
    delete [] this->name;
    this->name = nullptr;
}

Person& Person::operator=(const Person& person) {
    if (this == &person) {
        return *this;
    } else {
        this->id = person.id;

        delete [] this->name;
        this->name = nullptr;

        this->name = new char[strlen(person.name)+1];
        memcpy(this->name, person.name, sizeof(*person.name)*strlen(person.name)+1);

        printf("New Data: Person(%d, %s) copied!\n", this->id, this->name);

        return *this;
    }
}

Person& Person::operator=(Person&& person) noexcept {
    if (this == &person) {
        return *this;
    } else {
        this->id = person.id;

        delete [] this->name;
        this->name = person.name;
        person.name = nullptr;

        printf("New Data: Person(%d, %s) moved!\n", this->id, this->name);

        return *this;
    }
}

char* createString(const char* str) {
    int strLen = strlen(str);
    char* newStr = new char[strLen+1];
    memcpy(newStr, str, sizeof(char)*strLen+1);

    return newStr;
}

int main(int argc, char* argv[]) {
    
    char* p1_name = createString("Person 1");
    Person p1 {1, p1_name};

    char* p2_name = createString("Person 2");
    Person p2 {2, p2_name};

    // copy assignment operator
    p2 = p1;

    // move assignment operator
    p2 = Person{1, "Person 2 Updated"};

    delete p1_name;
    delete p2_name;
    
    return 0;
}