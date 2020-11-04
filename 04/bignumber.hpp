#pragma once

#include <string>

class BigNumber
{
public:
    static const int BASE;
    static const int BASE_POWER;

    BigNumber();
    BigNumber(u_int64_t* digits, size_t num_length, bool isNegative);
    BigNumber(int64_t number);
    BigNumber(const std::string& number);
    BigNumber(const BigNumber& other);
    BigNumber(BigNumber&& other);

    ~BigNumber();

    BigNumber& operator=(const BigNumber& other);
    BigNumber& operator=(BigNumber&& other);

    BigNumber operator-() const;

    friend BigNumber operator+(const BigNumber& bn1, const BigNumber& bn2);
    BigNumber& operator+=(const BigNumber& other);

    friend BigNumber operator-(const BigNumber& bn1, const BigNumber& bn2);
    BigNumber operator-=(const BigNumber& other);

    friend BigNumber operator*(const BigNumber& bn1, const BigNumber& bn2);
    BigNumber& operator*=(const BigNumber& other);

    friend bool operator==(const BigNumber& bn1, const BigNumber& bn2);
    friend bool operator<(const BigNumber& bn1, const BigNumber& bn2);
    friend bool operator!=(const BigNumber& bn1, const BigNumber& bn2);
    friend bool operator<=(const BigNumber& bn1, const BigNumber& bn2);
    friend bool operator>(const BigNumber& bn1, const BigNumber& bn2);
    friend bool operator>=(const BigNumber& bn1, const BigNumber& bn2);
    
    friend std::ostream& operator<<(std::ostream& stream, const BigNumber& bn);
private:
    u_int64_t* digits_;
    size_t num_length_;
    bool isNegative_ = false;

    const BigNumber& normalize_();
};
