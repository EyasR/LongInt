//University of Washington Bothell - CSSE
// CSS342C : Proffesor Munehiro Fukuda
// Created by Eyas Rashid on 3/14/2019
//
// programing assignment 7: Queues - Application: Very Long Integer
//
// Purpose: Utilize Deque, to implement LongInt Class

#include "longint.h"

// default constructor for LongInt
LongInt::LongInt() {
    // clears digits deque for new variables
    digits.clear();
    negativeInteger = false;

}

LongInt::LongInt(const string str) {
    // clears digits deque
    digits.clear();
    negativeInteger = false;

    // this is used to check for non numeric values between numbers
    bool nonNumericValues = false;
    for (int i = 0; i < str.size(); i++) {
        // checks if string is negative and flags if so.
        if (str[i] == '-' && negativeInteger == false) {
            negativeInteger = true;
            continue;

        }
        // if numeric value
        if (str[i] >= '0' && str[i] <= '9') {
            nonNumericValues = true;
            digits.addBack((char) str[i] - 48);

        } else

            // There is a non-numeric value characters inside
        if (nonNumericValues == true) {
            break;

        }
    }
    removeZeros();
}

// Copy constructor for LongInt
LongInt::LongInt(const LongInt &rhs) {
    // clears digits deque
    digits.clear();
    negativeInteger = false;
    // uses operator= overload
    *this = rhs;
    removeZeros();

}

// Destructor
LongInt::~LongInt() {
    // clears digits deque
    digits.clear();

}

// overloads operator>> input stream
istream &operator>>(istream &input, LongInt &rhs) {
    // clear digits deque
    rhs.digits.clear();
    char read = '0';

    // skips blank spaces
    while (((read = input.get()) != '-') && (read < '0' || read > '9')) {

    }
    // Checks for negative sign
    if (read == '-') {
        rhs.negativeInteger = true;
        read = input.get();

    }
    while (read >= '0' && read <= '9') {
        rhs.digits.addBack(read - 48);
        // reads a new character
        read = input.get();

    }
    rhs.removeZeros();
    return input;
}

// overloads operator<< output stream
ostream &operator<<(ostream &output, const LongInt &rhs) {

    //creates a copy of rhs for printing
    Deque<char> printCopy = rhs.digits;

    // Check if negative
    if (rhs.negativeInteger) {
        output << "-";
    }
    // prints a 0 if deque is empty
    if (printCopy.isEmpty()) {
        output << "0";
        return output;
    }
    // Iterate and print output
    while (printCopy.size() > 0) {
        output << (int) (printCopy.removeFront());
    }
    return output;
}


// overload operator+
LongInt LongInt::operator+(const LongInt &rhs) const {


    // ((-lhs) + rhs)
    if (negativeInteger == true && rhs.negativeInteger == false) {
        LongInt temporary = *this;
        temporary.negativeInteger = false;
        return rhs - temporary;

    }
    // (lhs + (-rhs))
    if (negativeInteger == false && rhs.negativeInteger == true) {
        LongInt temporary = rhs;
        temporary.negativeInteger = false;

        return *this - temporary; // Cases when we can utilize the - operator
    }

    // compare the size of lhs and rhs
    int size = rhs.digits.size() < digits.size() ? rhs.digits.size() : digits.size();
    LongInt answer;

    //temporary lhs and rhs deque's
    Deque<char> tempLhs = digits;
    Deque<char> tempRhs = rhs.digits;

    // used for carrying values
    int carry = 0;

    // Go through the smaller queue
    while (size != 0) {
        int a = (int) tempLhs.removeBack();
        int b = (int) tempRhs.removeBack();
        int result = a + b + carry;
        carry = result / 10;
        answer.digits.addFront((char) (result % 10));
        size--;

    }

    int secondResult = 0;
    // Check if any more elements still exist
    while (tempLhs.size() > 0) {
        // We still need to the latest carry integer
        secondResult = (int) tempLhs.removeBack() + carry;
        answer.digits.addFront((char) (secondResult % 10));
        carry = secondResult / 10;

    }
    while (tempRhs.size() > 0) {
        secondResult = (int) tempRhs.removeBack() + carry;
        answer.digits.addFront((char) (secondResult % 10));
        carry = secondResult / 10;

    }
    //  (lhs + rhs)
    if (negativeInteger == false && rhs.negativeInteger == false) {

        answer.negativeInteger = false;
        if (carry != 0) {
            answer.digits.addFront((char) carry); // Add the remaning carry

        }
        answer.removeZeros();
    }
    // -(lhs+rhs)
    if (negativeInteger == true && rhs.negativeInteger == true) {

        answer.negativeInteger = true;
        if (carry != 0) {
            answer.digits.addFront((char) carry);

        }
        answer.removeZeros();
    }
    return answer;
}

// overload operator-
LongInt LongInt::operator-(const LongInt &rhs) const {

    LongInt answer;

    // when the result is equal to 0;
    if (*this == rhs) {
        answer.negativeInteger = false;
        answer.digits.addBack(0);
        return answer;
    }
    // +lhs -rhs or lhs - (-rhs) or a + b
    if (negativeInteger == false && rhs.negativeInteger == true) {
        LongInt temporary = rhs;
        temporary.negativeInteger = false;
        answer = *this + temporary;
        answer.negativeInteger = false;
        return answer;

    }
    // -lhs - rhs or -( rhs + lhs)
    if (negativeInteger == true && rhs.negativeInteger == false) {
        LongInt temporary = rhs;
        temporary.negativeInteger = false;
        LongInt temporarySecond = *this;
        temporarySecond.negativeInteger = false;
        answer = temporarySecond + temporary;
        answer.negativeInteger = true;
        return answer;

    }
    // temporary deque's
    Deque<char> tempRhs;
    Deque<char> tempLhs;

    // -lhs -rhs or -lhs - (-rhs) or -lhs + rhs
    if (negativeInteger == true && rhs.negativeInteger == true) {
        if (*this >= rhs) {

            answer.negativeInteger = false;
            tempRhs = rhs.digits;
            tempLhs = digits;

        } else {
            answer.negativeInteger = true;
            tempRhs = digits;
            tempLhs = rhs.digits;

        }
    }
    // lhs - rhs
    if (negativeInteger == false && rhs.negativeInteger == false) {
        if (*this < rhs) {
            answer.negativeInteger = true;
            tempRhs = rhs.digits;
            tempLhs = digits;

        } else {
            answer.negativeInteger = false;
            tempRhs = digits;
            tempLhs = rhs.digits;
        }
    }
    int size = tempRhs.size() > tempLhs.size() ? tempRhs.size() : tempLhs.size();
    int carry = 0;
    int result = 0;

    // runs through all the numeric values
    while (size != 0) {

        int a = tempRhs.size() > 0 ? (int) tempRhs.removeBack() : 0;
        int b = tempLhs.size() > 0 ? (int) tempLhs.removeBack() : 0;

        if ((a - b - carry) >= 0) {

            result = a - b - carry;
            carry = 0;

        } else {
            // will result in negative integer add 10 to mitigate
            result = a + 10 - b - carry;
            carry = 1;

        }
        //abs
        answer.digits.addFront((char) (result));
        size--;

    }
    answer.removeZeros();
    return answer;

}

// Overload operator=
const LongInt &LongInt::operator=(const LongInt &rhs) {
    negativeInteger = rhs.negativeInteger;
    digits = rhs.digits;

    return *this;
}

// Overload operator<
bool LongInt::operator<(const LongInt &rhs) const {
    bool result = true;

    // if negativeInteger lhs less than positive rhs
    if (negativeInteger == true && rhs.negativeInteger == false) {
        return result;

    }
    // if positive lhs is greater than negativeInteger rhs
    if (negativeInteger == false && rhs.negativeInteger == true) {
        return !result;

    }
    int lhsSize = digits.size();
    int rhsSize = rhs.digits.size();

    if (lhsSize < rhsSize) {
        // -lhs -rhs  larger size = smaller numeric value
        if (negativeInteger == true && rhs.negativeInteger == true) {
            return !result;

        } else {

            return result;
        }
    }
    if (lhsSize > rhsSize) {
        // -lhs -rhs  larger size = smaller numeric value
        if (negativeInteger == true && rhs.negativeInteger == true) {
            return !result;

        } else {
            return !result;

        }
    }
    if (lhsSize == rhsSize) {

        Deque<char> a = digits;
        Deque<char> b = rhs.digits;

        while (lhsSize > 0) {
            int x = (int) a.removeFront();
            int y = (int) b.removeFront();

            if ((x - y) != 0) {

                if (negativeInteger == true && rhs.negativeInteger == true) {
                    // larger size = smaller numeric value for negative integers
                    return x < y ? !result : result;

                } else {
                    return x < y ? result : !result;

                }
            }
        }
    }
}

// Overload operator<=
bool LongInt::operator<=(const LongInt &rhs) const {
    if (*this < rhs) {
        return true;

    }
    if (*this == rhs) {
        return true;

    }
    return false;
}

// Overload oeprator>
bool LongInt::operator>(const LongInt &rhs) const {

    return !(*this <= rhs);
}

// Overload operator>=
bool LongInt::operator>=(const LongInt &rhs) const {

    return !(*this < rhs);
}

// Overload operator==
bool LongInt::operator==(const LongInt &rhs) const {

    // lhs and rhs are not same negative value
    if (negativeInteger != rhs.negativeInteger) {
        return false;

    }
    // if lhs and rhs are different sizes
    if (digits.size() != rhs.digits.size()) {
        return false;

    }

    Deque<char> lhsComparison = digits;
    Deque<char> rhsComparison = rhs.digits;

    // compare each element of LongInt
    while (rhsComparison.size() > 0) {
        int theResult = (int) lhsComparison.removeFront() - (int) rhsComparison.removeFront();

        if (theResult != 0) {
            return false;
        }
    }
    return true;
}

// Overload of operator!=
bool LongInt::operator!=(const LongInt &rhs) const {
    return !(*this == rhs);
}

// removes all 0's before a long integer. If integer is all 0 keeps one 0
void LongInt::removeZeros() {

    //removes all 0's from front
    while (digits.getFront() == 0) {
        digits.removeFront();

    }
    // if digits deque is empty add back 0
    if (digits.isEmpty()) {
        digits.addBack(0);
        negativeInteger = false;
    }
    return;
}