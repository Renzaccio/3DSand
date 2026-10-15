#pragma once

template<int stepVal = 1> struct Counter
{
private:
    int value;

public:
    Counter() : Counter(0) {}
    Counter(int start) { this->value = start; }

    int operator () ()
    {
        int oldVal = this->value;
        this->step();
        return oldVal;
    }

    int get() const { return this->value; }

    void step() { this->value += stepVal; }
};