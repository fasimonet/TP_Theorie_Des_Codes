#include "node.hpp"
#include <iostream>

Node::Node(int i)
    : regis(i), stateout0((i << 1) % 16), stateout1(((i << 1) % 16) + 1)
{
    bitset<R + 1> reg(i << 1);

    out0 = ((reg & G0).count() % 2) + (((reg & G1).count() % 2) << 1);
    reg.set(0, 1);
    out1 = ((reg & G0).count() % 2) + (((reg & G1).count() % 2) << 1);
}

bitset<R + 1> Node::G0 = 25;
bitset<R + 1> Node::G1 = 27;