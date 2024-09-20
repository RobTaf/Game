#ifndef Bitmask_hpp
#define Bitmask_hpp
#define N_COMPONENT_TYPES 32

using Bitset = unsigned int;

class Bitmask {
public:
    Bitmask();
    Bitmask(const Bitset& bits);
    ~Bitmask();

    Bitset getMask() const;
    void setMask(const Bitset& value);

    bool matches(const Bitmask& bits, const Bitset& relevant) const;
    bool getBit(const unsigned int& pos) const;
    void turnOnBit(const unsigned int& pos);
    void turnOnBits(const Bitset& bits);
    void clearBit(const unsigned int& pos);
    void toggleBit(const unsigned int& pos);
    void clear();

private:
    Bitset m_bits;
};

#endif