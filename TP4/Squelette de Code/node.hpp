#ifndef NODE_HPP
#define NODE_HPP

#include <cstddef>

class Node {
    public:
        int depth;
        int state[2];
        int min_hamming_distance;
        Node * prec;

        int C0_0;
        int C1_0;
        Node * suiv_0;

        int C0_1;
        int C1_1;
        Node * suiv_1;

        Node();
        Node(int, int[2], int, Node *);
        void createNextStep();
        void toString();
};

#endif // NODE_HPP