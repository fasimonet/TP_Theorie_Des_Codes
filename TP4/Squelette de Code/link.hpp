#pragma once

#include <bitset>
#include <vector>

#include "const.hpp"
#include "node.hpp"

using namespace std;

class Link
{
public:
	int nberror;
	vector<Node> link;
	vector<bitset<K> > message;
	
	Link();
	Link(Link &, Node &, int, int);
};


void NextStep(vector<Node> &, vector<Link> &, vector<Link> &, vector<bitset<N> >::iterator &);
void RemoveLink(vector<Link> &);