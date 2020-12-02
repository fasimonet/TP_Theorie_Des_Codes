#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <ctime>

#include "const.hpp"
#include "node.hpp"
#include "link.hpp"

#define DEBUG

using namespace std;

////////////////////////////////////////////////////////////
//      template<int bits> bitset<bits> randBitset()      //
//                                                        //
//               Generate random bitset                   //
////////////////////////////////////////////////////////////

template <int bits>
bitset<bits> randBitset()
{
	bitset<bits> r(rand());
	for (int i = 0; i < bits / 16 - 1; i++)
	{
		r <<= 16;
		r |= bitset<bits>(rand());
	}
	return r;
}

////////////////////////////////////////////////////////////
// vector< bitset<N> > GSM_code(vector< bitset<K> > mess) //
//                                                        //
//     Convolutional coding of a message (GSM norm)       //
////////////////////////////////////////////////////////////

vector<bitset<N> > GSM_code(vector<bitset<K> > mess)
{
	int i = 0, g0, g1;
	vector<bitset<N> > mess_out;

	bitset<N> cod_out;
	bitset<R + 1> G0(25);
	bitset<R + 1> G1(27);
	bitset<R + 1> reg;
	reg.reset();

#ifdef DEBUG
	cout << "-------------------- Debug Informations (Coding) --------------------" << endl
		 << endl;
	cout << "Initial register ( u(i-4)  u(i-3)  u(i-2)  u(i-1)  u(i)  ): " << reg << endl;
	cout << "Polynom G0       ( g0(i-4) g0(i-3) g0(i-2) g0(i-1) g0(i) ): " << G0 << endl;
	cout << "Polynom G1       ( g1(i-4) g1(i-3) g1(i-2) g1(i-1) g1(i) ): " << G1 << endl
		 << endl;
#endif

	for (vector<bitset<K> >::iterator it = mess.begin(); it != mess.end(); ++it)
	{
		reg = reg << 1;
		reg.set(0, (*it).count());

		g0 = (reg & G0).count() % 2;
		g1 = (reg & G1).count() % 2;

		cod_out.reset();
		cod_out.set(0, g0);
		cod_out.set(1, g1);

		mess_out.push_back(cod_out);

#ifdef DEBUG
		cout << "Block number: " << ++i << " - In frame: " << *it << endl;
		cout << "\t Current status of registers: " << reg << endl;
		cout << "\t Out : " << cod_out << endl;
#endif
	}
#ifdef DEBUG
	cout << "------------------------------------------------------------------" << endl
		 << endl;
#endif

	return mess_out;
}

/////////////////////////////////////////////////////////////////////////
// vector< bitset<N> >  GSM_transmission(vector< bitset<N> > mess_cod) //
//                                                                     //
//         Simulation of a transmission channel => adding errors       //
/////////////////////////////////////////////////////////////////////////

vector<bitset<N> > GSM_transmission(vector<bitset<N> > mess_cod)
{
	//TODO: Code here

	vector<bitset<N> > mess_tra = mess_cod;

	// Add errors randomly
	for (vector<bitset<N> >::iterator it = mess_tra.begin(); it != mess_tra.end(); ++it)
		if (rand() % 3 == 1)
			(*it)[1] = 1;

	return mess_tra;
}

//////////////////////////////////////////////////////////////////
// vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra) //
//                                                              //
//     Convolutional decoding of a message (GSM norm)           //
//////////////////////////////////////////////////////////////////

vector<bitset<K> > GSM_decode(vector<bitset<N> > mess_tra)
{
	//TODO: Code here
	
	vector<bitset<K> > mess_dec;
	vector<Node> states;
	vector<Link> links, newlinks;
	Link p;

	// Create Nodes
	for (int i = 0; i < 16; ++i)
		states.push_back(Node(i));

	// Add first Node link
	p.link.push_back(states[0]);
	links.push_back(p);

	// Create all graph
	for (vector<bitset<N> >::iterator it = mess_tra.begin(); it != mess_tra.end(); ++it, links = newlinks, newlinks.clear())
	{		
		NextStep(states, links, newlinks, it);
		RemoveLink(newlinks);
	}

	// Decode message
	for (vector<Link>::iterator linksIt = links.begin(); linksIt != links.end(); ++linksIt)
		if (linksIt->link.back().regis == states[0].regis)
			mess_dec = linksIt->message;

	return mess_dec;
}

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
	int NbMot = 12;

	vector<bitset<K> > mess;
	vector<bitset<N> > mess_cod;
	vector<bitset<N> > mess_tra;
	vector<bitset<K> > mess_dec;

	// Random initialization message
	srand((unsigned)time(NULL));
	for (int i = 0; i < NbMot; ++i)
		mess.push_back(randBitset<K>());
	for (int i = 0; i < R; ++i)
		mess.push_back(bitset<K>(0));

	// Coding of the message => mess_cod
	mess_cod = GSM_code(mess);

	// Simulation of a transmission (errors) => mess_tra
	mess_tra = GSM_transmission(mess_cod);

	// Decoding of the transmitted message => mess_dec
	mess_dec = GSM_decode(mess_tra);

	cout << "Source Message   : ";
	for (vector<bitset<K> >::iterator it = mess.begin(); it != mess.end(); ++it)
		cout << ' ' << *it;
	cout << '\n';

	cout << "Coded Message    : ";
	for (vector<bitset<N> >::iterator it = mess_cod.begin(); it != mess_cod.end(); ++it)
		cout << ' ' << *it;
	cout << '\n';

	cout << "Received Message : ";
	for (vector<bitset<N> >::iterator it = mess_tra.begin(); it != mess_tra.end(); ++it)
		cout << ' ' << *it;
	cout << '\n';

	cout << "Decoded Message  : ";
	for (vector<bitset<K> >::iterator it = mess_dec.begin(); it != mess_dec.end(); ++it)
		cout << ' ' << *it;
	cout << '\n';
}
