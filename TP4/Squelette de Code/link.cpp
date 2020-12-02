#include "link.hpp"

Link::Link() : nberror(0) {}

Link::Link(Link &inlink, Node &state, int i, int inerror)
    : nberror(inlink.nberror + inerror), link(inlink.link), message(inlink.message)
{
    link.push_back(state);
    message.push_back(bitset<K>(i));
}

void NextStep(vector<Node> &states, vector<Link> &links, vector<Link> &newlinks, vector<bitset<N> >::iterator &it) 
{
	for (vector<Link>::iterator linksit = links.begin(); linksit != links.end(); ++linksit)
	{
		newlinks.push_back(Link(*linksit, states[linksit->link.back().stateout0], 0, (*it == linksit->link.back().out0) ? 0 : 1));
		newlinks.push_back(Link(*linksit, states[linksit->link.back().stateout1], 1, (*it == linksit->link.back().out1) ? 0 : 1));
	}
}

void RemoveLink(vector<Link> &newlinks)
{
	bool isLinkAlive = true, isSameLinkBack = false;

	// For all links
	for (vector<Link>::iterator linksit = newlinks.begin(); linksit != newlinks.end(); linksit += isLinkAlive, isLinkAlive = true, isSameLinkBack = false)
	{
		// For all other links remaining
		for (vector<Link>::iterator clinksit = linksit + 1; clinksit != newlinks.end() && !isSameLinkBack; ++clinksit)
		{
			if (linksit->link.back().regis == clinksit->link.back().regis)
			{
				isLinkAlive = linksit->nberror < clinksit->nberror;
				newlinks.erase(isLinkAlive ? clinksit : linksit);
				isSameLinkBack = true;
			}
		}
	}
}