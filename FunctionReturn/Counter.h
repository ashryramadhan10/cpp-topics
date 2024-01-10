class Counter {
private:
    int n;

public:
    Counter(): n{0} {}
    Counter(const int _n): n{_n} {}
    ~Counter() {}

    int getCounter() const {
        return this->n;
    }

    Counter& operator++() {
        ++this->n;
        return *this;
    }

    friend std::ostream& operator << (std::ostream& os, const Counter& counter);
};