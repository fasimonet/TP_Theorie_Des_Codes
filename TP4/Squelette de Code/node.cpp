#include "node.hpp"
#include <iostream>       

Node::Node() {
    depth = 0;

    state[0] = 0;
    state[1] = 0;
    min_hamming_distance = 0;
    prec = NULL;
    
    C0_0 = -1;
    C1_0 = -1;
    suiv_0 = NULL;

    C0_1 = -1;
    C1_1 = -1;
    suiv_1 = NULL;
}

Node::Node(int depth, int state[2], int min_hamming_distance, Node * prec) : depth(depth), min_hamming_distance(min_hamming_distance), prec(prec) {
    this->state[0] = state[0];
    this->state[1] = state[1];

    C0_0 = -1;
    C1_0 = -1;
    suiv_0 = NULL;

    C0_1 = -1;
    C1_1 = -1;
    suiv_1 = NULL;
}

void Node::createNextStep() {
    if (suiv_0 == NULL && suiv_1 == NULL) {
        int tmp[2];

        C0_0 = (state[1] + 0) % 2;
        C1_0 = (state[0] + C0_0) % 2;
        tmp[0] = 0;
        tmp[1] = state[0];
        suiv_0 = new Node(depth+1, tmp, min_hamming_distance, this);

        C0_1 = (state[1] + 1) % 2;
        C1_1 = (state[0] + C0_1) % 2;
        tmp[0] = 1;
        tmp[1] = state[0];
        suiv_1 = new Node(depth+1, tmp, min_hamming_distance, this);
    }
    else {
        suiv_0->createNextStep();
        suiv_1->createNextStep();
    }
}

void Node::toString() {
    std::string tabulation = "";
    for (int i=0; i < depth; ++i) {
        tabulation += "\t";
    }
    std::cout << tabulation << state[0] << state[1] << " 0/" << C0_0 << C1_0 << std::endl;

    if (suiv_0 != NULL) {
        suiv_0->toString();
    }

    std::cout << tabulation << state[0] << state[1] << " 1/" << C0_1 << C1_1 << std::endl;
    if (suiv_1 != NULL) {
        suiv_1->toString();
    }
}
