#include <ostream>
#include "bignumber.hpp"

const int BigNumber::BASE = 100000000;
const int BigNumber::BASE_POWER = 8;

/////////////////////////////////
////////// CONSTRUCTORS /////////
/////////////////////////////////

BigNumber::BigNumber(): digits_(new u_int64_t[1] {0}), num_length_(1), isNegative_(false) {}

BigNumber::BigNumber(u_int64_t* digits, size_t num_length, bool isNegative): digits_(digits), num_length_(num_length), isNegative_(isNegative) {}

BigNumber::BigNumber(int64_t number)
{
    isNegative_ = number < 0;
    if (isNegative_) number = -number;

    size_t number_digit_count = std::to_string(number).size();
    num_length_ = number_digit_count / BASE_POWER + (number_digit_count % BASE_POWER != 0);

    digits_ = new u_int64_t[num_length_];

    for (size_t i = 0; i < num_length_; i++)
    {
        digits_[i] = number % BASE;
        number /= BASE;
    }
}

BigNumber::BigNumber(const std::string& number)
{
    isNegative_ = number[0] == '-';

    num_length_ = (number.size() - isNegative_) / BASE_POWER + ((number.size() - isNegative_) % BASE_POWER != 0);
    digits_ = new u_int64_t[num_length_];

    for (size_t i = 0; i < num_length_ - 1; i++)
    {
        digits_[i] = std::stoull(number.substr(number.size() - (i + 1)*BASE_POWER, BASE_POWER));
    }
    digits_[num_length_ - 1] = std::stoull(number.substr(isNegative_, number.size() - isNegative_ - (num_length_ - 1)*BASE_POWER));
}

BigNumber::BigNumber(const BigNumber& other): num_length_(other.num_length_), isNegative_(other.isNegative_)
{
    digits_ = new u_int64_t[num_length_];
    std::copy(other.digits_, other.digits_ + num_length_, digits_);
}

BigNumber::BigNumber(BigNumber&& other): num_length_(std::move(other.num_length_)), isNegative_(std::move(other.isNegative_))
{
    digits_ = new(other.digits_) u_int64_t[other.num_length_];

    other.num_length_ = 0;
    other.isNegative_ = false;
    other.digits_ = nullptr;
}

/////////////////////////////////
////////// DESCTRUCTOR //////////
/////////////////////////////////

BigNumber::~BigNumber()
{
    delete[] digits_;
}


/////////////////////////////////
//////// EQUALS OPERATORS ///////
/////////////////////////////////

BigNumber& BigNumber::operator=(const BigNumber& other)
{
    if (this == &other)
    {
        return *this;
    }

    u_int64_t* temp = new u_int64_t[other.num_length_];
    delete[] digits_;
    digits_ = temp;

    std::copy(other.digits_, other.digits_ + other.num_length_, digits_);

    num_length_ = other.num_length_;
    isNegative_ = other.isNegative_;

    return *this;
}

BigNumber& BigNumber::operator=(BigNumber&& other)
{
    if (this == &other)
    {
        return *this;
    }

    num_length_ = std::move(other.num_length_);
    isNegative_ = std::move(other.isNegative_);
    delete[] digits_;
    digits_ = other.digits_;

    other.num_length_ = 0;
    other.isNegative_ = false;
    other.digits_ = nullptr;

    return *this;
}

/////////////////////////////////
////// ARIFMETIC OPERATORS //////
/////////////////////////////////

BigNumber BigNumber::operator-() const
{
    u_int64_t* n_digits = new u_int64_t[num_length_];
    for (size_t i = 0; i < num_length_; i++) n_digits[i] = digits_[i];
    
    return BigNumber(n_digits, num_length_, !isNegative_);
}

BigNumber operator+(const BigNumber& bn1, const BigNumber& bn2)
{
    if (bn1.isNegative_ != bn2.isNegative_)
    {
        return bn1.isNegative_ ? operator-(bn2, bn1.operator-()) : operator-(bn1, bn2.operator-());
    }

    // sum of 1st digits can be > BASE. Then need to add new digit
    size_t s_num_length = std::max(bn1.num_length_, bn2.num_length_) + 1;
    u_int64_t* s_digits = new u_int64_t[s_num_length] {};

    for (size_t i = 0; i < s_num_length - 1; i++)
    {
        if (i < bn1.num_length_) s_digits[i] += bn1.digits_[i];
        if (i < bn2.num_length_) s_digits[i] += bn2.digits_[i];
    }

    return BigNumber(s_digits, s_num_length, bn1.isNegative_).normalize_();
}

BigNumber operator-(const BigNumber& bn1, const BigNumber& bn2)
{
    if (bn1.isNegative_ != bn2.isNegative_) 
    {
        return bn1.isNegative_ ? operator+(bn1.operator-(), bn2).operator-() : operator+(bn1, bn2.operator-());
    }

    if ((bn1.isNegative_ && operator>(bn1, bn2)) || (!bn1.isNegative_ && operator<(bn1, bn2))) 
    {
        return operator-(bn2, bn1).operator-();
    }

    size_t d_num_length = std::max(bn1.num_length_, bn2.num_length_);
    u_int64_t* d_digits = new u_int64_t[d_num_length] {};

    for (size_t i = 0; i < d_num_length; i++)
    {
        if (i < bn2.num_length_)
        {
            if (bn1.digits_[i] >= bn2.digits_[i]) d_digits[i] = bn1.digits_[i] - bn2.digits_[i];
            else
            {
                size_t j = i;
                do
                {
                    j++;
                } while (bn1.digits_[j] == 0);

                bn1.digits_[j] -= 1;
                for (; j > i + 1; j--)
                {
                    bn1.digits_[j - 1] = BigNumber::BASE - 1;
                }

                d_digits[i] = d_digits[i] = bn1.digits_[i] + BigNumber::BASE - bn2.digits_[i];
            }
        }
        else
        {
            d_digits[i] = bn1.digits_[i];
        }
    }
    
    return BigNumber(d_digits, d_num_length, bn1.isNegative_).normalize_();
}

BigNumber operator*(const BigNumber& bn1, const BigNumber& bn2)
{
    size_t m_num_length = bn1.num_length_ + bn2.num_length_;
    u_int64_t* m_digits = new u_int64_t[m_num_length] {};
    bool m_isNegative = (bn1.isNegative_ && !bn2.isNegative_) || (!bn1.isNegative_ && bn2.isNegative_);

    for (size_t i = 0; i < bn1.num_length_; i++)
    {
        for (size_t j = 0; j < bn2.num_length_; j++)
        {
            m_digits[i + j] += bn1.digits_[i] * bn2.digits_[j];
        }
    }
     
    return BigNumber(m_digits, m_num_length, m_isNegative).normalize_();
}

/////////////////////////////////
/////// COMPARE OPERATORS ///////
/////////////////////////////////

bool operator==(const BigNumber& bn1, const BigNumber& bn2)
{
    if (bn1.isNegative_ != bn2.isNegative_) return false;
    if (bn1.num_length_ != bn2.num_length_) return false;

    for (size_t i = 0; i < bn1.num_length_; i++)
    {
        if (bn1.digits_[i] != bn2.digits_[i]) return false;
    }
    
    return true;
}

bool operator<(const BigNumber& bn1, const BigNumber& bn2)
{
    if (bn1.isNegative_ != bn2.isNegative_) return bn1.isNegative_;
    if (bn1.num_length_ != bn2.num_length_)
    {
        if (bn1.isNegative_) return bn1.num_length_ > bn2.num_length_;
        else return bn1.num_length_ < bn2.num_length_;
    }

    for (size_t i = bn1.num_length_; i > 0; i--)
    {
        if (bn1.digits_[i - 1] != bn2.digits_[i - 1])
        {
            if (bn1.isNegative_) return bn1.digits_[i - 1] > bn2.digits_[i - 1];
            else return bn1.digits_[i - 1] < bn2.digits_[i - 1];
        }
    }
    
    return false;
}

bool operator!=(const BigNumber& bn1, const BigNumber& bn2) { return !operator==(bn1, bn2); }
bool operator<=(const BigNumber& bn1, const BigNumber& bn2) { return operator==(bn1, bn2) || operator<(bn1, bn2); }
bool operator>(const BigNumber& bn1, const BigNumber& bn2) { return !operator<=(bn1, bn2); }
bool operator>=(const BigNumber& bn1, const BigNumber& bn2) { return !operator<(bn1, bn2); }

/////////////////////////////////
//////// STREAM OPERATOR ////////
/////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const BigNumber& bn)
{
    if (bn.isNegative_) stream << "-";

    for (size_t i = bn.num_length_; i > 0 ; i--)
    {
        std::string num = std::to_string(bn.digits_[i - 1]);
        
        if ((num.size() < BigNumber::BASE_POWER) && (i < bn.num_length_))
        {
            for (size_t j = 0; j < BigNumber::BASE_POWER - num.size(); j++) stream << "0";
        }
        stream << bn.digits_[i - 1];
    }

    return stream;
}

/////////////////////////////////
//////// PRIVATE FUNCTION ///////
/////////////////////////////////

BigNumber& BigNumber::normalize_()
{
    size_t zero_count = 0;

    for (size_t i = 0; i < num_length_; i++)
    {
        if (digits_[i] == 0) zero_count++;
        else zero_count = 0;
        
        if (digits_[i] / BASE > 0)
        {
            digits_[i + 1] += digits_[i] / BASE;
            digits_[i] %= BASE;
        }
    }

    num_length_ -= zero_count;
    if (num_length_ == 0) num_length_ = 1;

    return *this;
}
