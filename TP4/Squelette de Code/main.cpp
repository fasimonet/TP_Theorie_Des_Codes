#include <iostream>       
#include <fstream>
#include <string>        
#include <vector>
#include <bitset>        
#include <cstdlib>
#include <ctime>
#include <list>
#include "node.hpp"

const int N=2;
const int K=1;
const int R=4;

#define DEBUG

using namespace std; 

////////////////////////////////////////////////////////////
//      template<int bits> bitset<bits> randBitset()      //
//                                                        //
//               Generate random bitset                   //
////////////////////////////////////////////////////////////


template<int bits> bitset<bits> randBitset()
{ 
  bitset<bits> r(rand());
  for(int i = 0; i < bits/16 - 1; i++)
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

vector< bitset<N> > GSM_code(vector< bitset<K> > mess)
{
 int i=0, g0, g1;
 vector< bitset<N> > mess_out;

 bitset<N> cod_out; 
 bitset<R+1> G0(25);
 bitset<R+1> G1(27); 
 bitset<R+1> reg; 
 reg.reset();
 
 #ifdef DEBUG
  cout << "-------------------- Debug Informations (Coding) --------------------" << endl << endl;
  cout << "Initial register ( u(i-4)  u(i-3)  u(i-2)  u(i-1)  u(i)  ): " << reg << endl;
  cout << "Polynom G0       ( g0(i-4) g0(i-3) g0(i-2) g0(i-1) g0(i) ): " << G0 << endl;
  cout << "Polynom G1       ( g1(i-4) g1(i-3) g1(i-2) g1(i-1) g1(i) ): " << G1 << endl << endl;
 #endif

 for (vector<bitset<K> >::iterator it = mess.begin(); it != mess.end(); ++it)
 {
  reg = reg<<1;
  reg.set(0,(*it).count());
 
  g0 = (reg&G0).count()%2;
  g1 = (reg&G1).count()%2;

  cod_out.reset();
  cod_out.set(0,g0);
  cod_out.set(1,g1);

  mess_out.push_back(cod_out);
  
  #ifdef DEBUG
   cout << "Block number: " << ++i << " - In frame: "<< *it << endl; 
   cout << "\t Current status of registers: "<< reg << endl;
   cout << "\t Out : " << cod_out << endl;
  #endif
 }
 #ifdef DEBUG
  cout << "------------------------------------------------------------------" << endl << endl;
 #endif

 return mess_out;
}

/////////////////////////////////////////////////////////////////////////
// vector< bitset<N> >  GSM_transmission(vector< bitset<N> > mess_cod) //
//                                                                     //
//         Simulation of a transmission channel => adding errors       //
/////////////////////////////////////////////////////////////////////////

vector< bitset<N> >  GSM_transmission(vector< bitset<N> > mess_cod)
{
 vector< bitset<N> > mess_tra = mess_cod;

 //TODO: Code here
 
 return mess_tra;
}

//////////////////////////////////////////////////////////////////
// vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra) //
//                                                              //
//     Convolutional decoding of a message (GSM norm)           //
//////////////////////////////////////////////////////////////////

vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra)
{

 vector< bitset<K> > mess_dec;
 //list< vector >
/*
 Node* node = createTree();
 browseTree(node);
*/
 //TODO: Code here
//  for(unsigned int i=0;i<mess_tra.size();++i) {
   
//  }

 return mess_dec;
}

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

void a() {
  cout << "S0 S1 | in | C0 C1 | S'0 S'1" << endl;
  cout << "----------------------------" << endl;

  for (int S1=0; S1 < 2; ++S1) {
    for (int S0=0; S0 < 2; ++S0) {
      for (int input=0; input < 2; ++input) {
        int C0 = (S1 + input) % 2;
        int C1 = (S0 + C0) % 2;
        int S_0 = input;
        int S_1 = S0;

        cout << " " << S0 << "  " << S1 << " |  " << input << " |  " << C0 << "  " << C1 << " |   " << S_0 << "   " << S_1 << endl;
      }
    }
  }


  int message[5][2];
  message[0][0] = 0;
  message[0][1] = 0;
  message[1][0] = 0;
  message[1][1] = 1;
  message[2][0] = 0;
  message[2][1] = 1;
  message[3][0] = 1;
  message[3][1] = 0;
  message[4][0] = 1;
  message[4][1] = 0;

  Node * graph = new Node();

  std::cout << "Messages : ";
  for (int i = 0; i < 1; ++i) {
    for (int j = 0; j < 2; ++j) {
      std::cout << message[i][j];
    }
    std::cout << " ";
    graph->createNextStep();
  }
  std::cout << std::endl;
  graph->end();

  graph->toString();
}

int main()
{
  a();
/*
 int NbMot = 12;

 vector< bitset<K> > mess;
 vector< bitset<N> > mess_cod;
 vector< bitset<N> > mess_tra;
 vector< bitset<K> > mess_dec;

 // Random initialization message
 srand( (unsigned)time( NULL ) );
 for(int i=0;i<NbMot;++i)
  mess.push_back(randBitset<K>());
 for(int i=0;i<R;++i)
  mess.push_back(bitset<K>(0));

 // Coding of the message => mess_cod
 mess_cod = GSM_code(mess);

 // Simulation of a transmission (errors) => mess_tra
 mess_tra = GSM_transmission(mess_cod);

 // Decoding of the transmitted message => mess_dec
 mess_dec = GSM_decode(mess_tra);

 cout << "Source Message   : ";
 for (vector<bitset<K> >::iterator it = mess.begin() ; it != mess.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';

 cout << "Coded Message    : ";
 for (vector<bitset<N> >::iterator it = mess_cod.begin() ; it != mess_cod.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';

 cout << "Received Message : ";
 for (vector<bitset<N> >::iterator it = mess_tra.begin() ; it != mess_tra.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';

 cout << "Decoded Message  : ";
 for (vector<bitset<K> >::iterator it = mess_dec.begin() ; it != mess_dec.end(); ++it)
    cout << ' ' << *it;
  cout << '\n';
  */
}

