#pragma once

#include <bitset>

#include "const.hpp"

using namespace std;

class Node
{
public:
    static bitset<R + 1> G0;
    static bitset<R + 1> G1;

    bitset<R> regis;

    bitset<N> out0;
    bitset<N> out1;

    int stateout0;
    int stateout1;

    Node(int);
};