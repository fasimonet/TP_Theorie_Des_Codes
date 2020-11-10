#include <iostream>      
#include <fstream>
#include <string>         
#include <bitset>        
#include <vector>

/**
 * Hamming (7,4)
 **/ 
#define N 4
#define HAMMING_7 7

/**
 * Debug macros
 **/ 
#define DEBUG_RF true // Debug Information: Read File
#define DEBUG_HE true // Debug Information: Hamming Encoding

using namespace std; 

/**
 * vector<bitset<N> > readFile(string filename)
 * Read a file in binary and create a vector of bitset wih a width of 4 for each bitset
 * Return a vector bitset
 **/ 
vector<bitset<N> > readFile(string filename)
{
	vector<bitset<N> > content;
	ifstream reader;
	char buffer;
	reader.open(filename.c_str(), ios::binary|ios::in);

	if(DEBUG_RF)
		cout << "Read : \t";

	if(reader != NULL && reader.is_open())
	{
		while(!reader.eof())
		{
			reader.read(&buffer, 1);
			if (buffer == 0) {
				continue;
			}

			bitset<N> bsBufferLSB(buffer);
			bitset<N> bsBufferMSB(buffer>>4);
			
			content.push_back(bsBufferMSB);
			content.push_back(bsBufferLSB);
	
			if(DEBUG_RF)
			{
				cout << " |" << bsBufferMSB.to_string();
				cout << " |" << bsBufferLSB.to_string();
			}

			buffer = 0;
		}
	}
	
	if(DEBUG_RF)
		cout << endl;

	reader.close();
	return content;
} 

/**
 * vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
 * Convert a vector of bitset<4> into a hamming vector of bitset<7>
 **/ 
vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
{
	vector<bitset<HAMMING_7> > encodedBitset;
	
	if(DEBUG_HE)
		std::cout << "Encode : \t";
		
	for(vector<bitset<N> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{
		// Code to modify (sample)		
		bitset<N> inBuffer = *i;
		bitset<HAMMING_7> outBuffer;

		outBuffer[0] = inBuffer[0];
		outBuffer[1] = inBuffer[1];
		outBuffer[2] = inBuffer[2];
		outBuffer[3] = inBuffer[3];
		
		outBuffer[4] = (inBuffer[0] + inBuffer[1] + inBuffer[3]) % 2;
		outBuffer[5] = (inBuffer[0] + inBuffer[2] + inBuffer[3]) % 2;
		outBuffer[6] = (inBuffer[1] + inBuffer[2] + inBuffer[3]) % 2;
		
		if(DEBUG_HE)
			cout << " | " << outBuffer.to_string();
		
		encodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE)
		cout << endl;
	
	return encodedBitset;
}

vector<bitset<HAMMING_7> > HammingDecoding(vector<bitset<HAMMING_7> > encodedBitset)
{
	vector<bitset<HAMMING_7> > decodedBitset;
	
	if(DEBUG_HE)
		std::cout << "Decoding : \t";
		
	for(vector<bitset<HAMMING_7> >::iterator i = encodedBitset.begin(); i != encodedBitset.end();++i)
	{
		// Code to modify (sample)		
		bitset<HAMMING_7> inBuffer = *i;
		bitset<HAMMING_7> outBuffer;

		outBuffer[0] = inBuffer[0];
		outBuffer[1] = inBuffer[1];
		outBuffer[2] = inBuffer[2];
		outBuffer[3] = inBuffer[3];
		
		outBuffer[4] = (inBuffer[0] + inBuffer[1] + inBuffer[3]) % 2;
		outBuffer[5] = (inBuffer[0] + inBuffer[2] + inBuffer[3]) % 2;
		outBuffer[6] = (inBuffer[1] + inBuffer[2] + inBuffer[3]) % 2;

		bool k1Error = outBuffer[4] != inBuffer[4];
		bool k2Error = outBuffer[5] != inBuffer[5];
		bool k3Error = outBuffer[6] != inBuffer[6];
		
		if (k1Error && k2Error && !k3Error) {
			cout << "Error bit 1" << endl;
			outBuffer[0] = !outBuffer[0];
		}
		else if (k1Error && !k2Error && k3Error) {
			cout << "Error bit 2" << endl;
			outBuffer[1] = !outBuffer[1];
		}
		else if (!k1Error && k2Error && k3Error) {
			cout << "Error bit 3" << endl;
			outBuffer[2] = !outBuffer[2];
		}
		else if (k1Error && k2Error && k3Error) {
			cout << "Error bit 4" << endl;
			outBuffer[3] = !outBuffer[3];
		}

		if(DEBUG_HE)
			cout << " | " << outBuffer.to_string();
		
		decodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE)
		cout << endl;
	
	return decodedBitset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     Main                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	vector< bitset<N> > input_data;
	vector< bitset<HAMMING_7> > encode_data; 
	vector< bitset<HAMMING_7> > decode_data;

	// Read data to encode
	input_data = readFile("small.txt");
	
	// Encode by Hamming (7,4) coding
 	encode_data = HammingEncoding(input_data);
 
 	// Inject error
	encode_data.at(0)[0] =  (1 + encode_data.at(0)[0]) % 2;

	// Decode
	decode_data = HammingDecoding(encode_data);
}



