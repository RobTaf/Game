#include "Bitmask.hpp"

Bitmask::Bitmask(): m_bits(0)
{
}

Bitmask::Bitmask(const Bitset& bits): m_bits(bits)
{
}

Bitmask::~Bitmask()
{
}

Bitset Bitmask::getMask() const {
    return m_bits;
}

void Bitmask::setMask(const Bitset &value) {
    m_bits = value;
}

bool Bitmask::matches(const Bitmask& bits, const Bitset& relevant) const {
    return relevant ? ((bits.getMask() & relevant) == (m_bits & relevant)) : bits.getMask() == m_bits;
}

bool Bitmask::getBit(const unsigned int& pos) const {
    return ((m_bits & (1 << pos)) != 0);
}

void Bitmask::turnOnBit(const unsigned int& pos) {
    m_bits |= (1 << pos);
}

void Bitmask::turnOnBits(const Bitset& bits) {
    m_bits |= (1 << bits);
}

void Bitmask::clearBit(const unsigned int& pos) {
    m_bits &= ~(1 << pos);
}

void Bitmask::toggleBit(const unsigned int& pos) {
    m_bits ^= (1 << pos);
}

void Bitmask::clear() {
    m_bits = 0;
}
