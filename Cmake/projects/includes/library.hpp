#pragma once

class Library {
private:
    int data;
public:
    Library();
    Library(int _data);
    ~Library();
    int getData();
};