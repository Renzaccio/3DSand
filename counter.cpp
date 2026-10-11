struct Counter
{
private:
    int value;
    int stepVal;

public:
    Counter() : Counter(0, 1) {}
    Counter(int start, int stepInit) { this->value = start; this->stepVal = stepInit; }

    int operator () ()
    {
        int oldVal = this->value;
        this->step();
        return oldVal;
    }

    int get() const { return this->value; }

    void step() { this->value += this->stepVal; }
};