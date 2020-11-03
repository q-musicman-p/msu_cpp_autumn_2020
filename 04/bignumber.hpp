//#pragma once

#include <string>
#include <iostream>

class BigNumber
{
public:
    BigNumber(): digits_(new u_int64_t[1] {0}), num_length_(1), isNegative_(false) {}

    BigNumber(u_int64_t* digits, size_t num_length, bool isNegative): digits_(digits), num_length_(num_length), isNegative_(isNegative) {}

    BigNumber(int64_t number)
    {
        isNegative_ = number < 0;
        if (isNegative_) number = -number;

        size_t number_digit_count = std::to_string(number).size();
        num_length_ = number_digit_count / BASE_POWER_ + (number_digit_count % BASE_POWER_ != 0);

        digits_ = new u_int64_t[num_length_];

        for (size_t i = 0; i < num_length_; i++)
        {
            digits_[i] = number % BASE_;
            number /= BASE_;
        }
    }

    BigNumber(const std::string& number)
    {
        isNegative_ = number[0] == '-';
        //std::cout << "isNegative = " << isNegative_ << std::endl;

        num_length_ = (number.size() - isNegative_) / BASE_POWER_ + ((number.size() - isNegative_) % BASE_POWER_ != 0);
        digits_ = new u_int64_t[num_length_];

        //std::cout << "num_len = " << num_length_ << std::endl;

        for (size_t i = 0; i < num_length_ - 1; i++)
        {
            digits_[i] = std::stoull(number.substr(number.size() - (i + 1)*BASE_POWER_, BASE_POWER_));
            //std::cout << i+1 << " digit = " << digits_[i] << std::endl;
        }
        digits_[num_length_ - 1] = std::stoull(number.substr(isNegative_, number.size() - (num_length_ - 1)*BASE_POWER_));
        //std::cout << "last digit = " << digits_[num_length_ - 1] << std::endl;
    }

    BigNumber(const BigNumber& other): num_length_(other.num_length_), isNegative_(other.isNegative_)
    {
        digits_ = new u_int64_t[num_length_];
        std::copy(digits_, digits_ + num_length_, other.digits_);
        /*for (size_t i = 0; i < num_length_; i++)
        {
            digits_[i] = other.digits_[i];
        }*/
    }

    BigNumber(BigNumber&& other): num_length_(std::move(other.num_length_)), isNegative_(std::move(other.isNegative_))
    {
        digits_ = new(other.digits_) u_int64_t[other.num_length_];
        
        other.num_length_ = 0;
        other.isNegative_ = false;
        other.digits_ = nullptr;
    }

    ~BigNumber()
    {
        delete[] digits_;
    }

    BigNumber& operator=(const BigNumber& other)
    {
        if (this == &other)
        {
            return *this;
        }

        u_int64_t* temp = new u_int64_t[other.num_length_];
        delete[] digits_;
        digits_ = temp;

        std::copy(digits_, digits_ + other.num_length_, other.digits_);
        /*for (size_t i = 0; i < other.num_length_; i++)
        {
            digits_[i] = other.digits_[i];
        }*/

        num_length_ = other.num_length_;
        isNegative_ = other.isNegative_;

        return *this;
    }

    BigNumber& operator=(BigNumber&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        num_length_ = std::move(other.num_length_);
        isNegative_ = std::move(other.isNegative_);
        digits_ = other.digits_;

        other.num_length_ = 0;
        other.isNegative_ = false;
        other.digits_ = nullptr;
    }

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

    BigNumber operator-() const
    {
        u_int64_t* n_digits = new u_int64_t[num_length_];
        for (size_t i = 0; i < num_length_; i++) n_digits[i] = digits_[i];
        
        return BigNumber(n_digits, num_length_, !isNegative_);
    }
   
private:
    static const int BASE_ = 100;
    static const int BASE_POWER_ = 2;

    u_int64_t* digits_;
    size_t num_length_;
    bool isNegative_ = false;

    const BigNumber& normalize_()
    {
        size_t zero_count = 0;

        for (size_t i = 0; i < num_length_; i++)
        {
            if (digits_[i] == 0) zero_count++;
            else zero_count = 0;
            
            //std::cout << "digit" << i << " = " << digits_[i] << '\n';
            if (digits_[i] / BASE_ > 0)
            {
                //std::cout << "digit" << i + 1 << " = " << digits_[i+1] << '\n';
                digits_[i + 1] += digits_[i] / BASE_;
                //std::cout << "digit" << i + 1 << " after = " << digits_[i+1] << '\n';
                digits_[i] %= BASE_;
                //std::cout << "digit" << i << " = after " << digits_[i] << '\n';
            }
        }

        num_length_ -= zero_count;
        if (num_length_ == 0) num_length_ = 1;

        return *this;
    }
};

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
    //std::cout << "b1_len = " << bn1.num_length_ << ", b2_len = " << bn2.num_length_ << std::endl;
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

std::ostream& operator<<(std::ostream& stream, const BigNumber& bn)
{
    if (bn.isNegative_) stream << "-";

    for (size_t i = bn.num_length_; i > 0 ; i--)
    {
        std::string num = std::to_string(bn.digits_[i - 1]);
        //std::cout << "num.size = " << num.size() << ", num = " << num << '\n';
        if ((num.size() < BigNumber::BASE_POWER_) && (i < bn.num_length_))
        { 
            for (size_t j = 0; j < BigNumber::BASE_POWER_ - num.size(); j++) stream << "0";
        }
        stream << bn.digits_[i - 1];
    }

    return stream;
}

BigNumber operator+(const BigNumber& bn1, const BigNumber& bn2)
{
    //std::cout << "operator+" << std::endl;
    if (bn1.isNegative_ != bn2.isNegative_)
    {
        return bn1.isNegative_ ? operator-(bn2, bn1.operator-()) : operator-(bn1, bn2.operator-());
    }
    //std::cout << "similiar sign" << std::endl;
    // sum of 1st digits can be > BASE_. Then need to add new digit
    size_t s_num_length = std::max(bn1.num_length_, bn2.num_length_) + 1;
    u_int64_t* s_digits = new u_int64_t[s_num_length];
    //std::cout << "no bad alloc" << std::endl;
    for (size_t i = 0; i < s_num_length - 1; i++)
    {
        //std::cout << "s_d = " << s_digits[i];
        if (i < bn1.num_length_) s_digits[i] += bn1.digits_[i];
        if (i < bn2.num_length_) s_digits[i] += bn2.digits_[i];
        //std::cout << ", s_d_after = " << s_digits[i] << std::endl;
    }

    return BigNumber(s_digits, s_num_length, bn1.isNegative_).normalize_();
}

BigNumber operator-(const BigNumber& bn1, const BigNumber& bn2)
{
    if (bn1.isNegative_ != bn2.isNegative_) 
    {
        return bn1.isNegative_ ? operator+(bn1.operator-(), bn2).operator-() : operator+(bn1, bn2.operator-());
    }

    //std::cout << "b1 = " << bn1 << ", b2 = " << bn2 << std::endl;
    //std::cout << "b1 < b2 = " << operator<(bn1, bn2) << ", b2 < b1 = " << operator>(bn2, bn1) <<   std::endl;
    //std::cout << "Before cond" << std::endl;
    if ((bn1.isNegative_ && operator>(bn1, bn2))) return operator-(bn2, bn1).operator-();
    //std::cout << "After cond" << std::endl;
    size_t d_num_length = std::max(bn1.num_length_, bn2.num_length_);
    u_int64_t* d_digits = new u_int64_t[d_num_length];

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
                    bn1.digits_[j - 1] = BigNumber::BASE_ - 1;
                }

                d_digits[i] = d_digits[i] = bn1.digits_[i] + BigNumber::BASE_ - bn2.digits_[i];
            }
        }
        else
        {
            d_digits[i] = bn1.digits_[i];
        }
    }
    
    return BigNumber(d_digits, d_num_length, bn1.isNegative_).normalize_();
}