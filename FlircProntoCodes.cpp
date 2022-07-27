#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>

using namespace std;

long const RawDataCode = 0x0000;
long const CarrierFrequency = 0x006d;
long const Burst1PairCount = 0x0000;

void PrintWord(long word) { cout << hex << setfill('0') << setw(4) << word << " "; }

void PushBits(char n, vector<char> & bits)
{
    bits.push_back((n & 0x8) >> 3);
    bits.push_back((n & 0x4) >> 2);
    bits.push_back((n & 0x2) >> 1);
    bits.push_back((n & 0x1));
}

int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        cerr
            << "Usage: FlircProntoCodes <DeviceCode> <CommandCode>\n"
            << "       DeviceCode:   Hexidecmal code for the Flirc device\n"
            << "       CommandCode:  Hexidecmal code for the Flirc command\n\n"
            << "       Generates the pronto hex codes to send an IR command to the\n"
            << "       Flirc TV USB receiver.\n";
        return 1;
    }

    long deviceCode = strtol(argv[1], nullptr, 16);
    long commandCode = strtol(argv[2], nullptr, 16);

    if (deviceCode < 0 || deviceCode > 0xf)
    {
        cerr << "Invalid device code: " << argv[2];
        return 2;
    }
    if (commandCode < 0 || commandCode > 0xff)
    {
        cerr << "Invalid command code: " << argv[1];
        return 3;
    }

    // Positive values indicate number of frames LED is on, negative values
    // indicate the number of frames the LED is off.
    // Adding the fixed header for each Flirc IR packet. 
    vector<long> edges = {
        +0xba, -0xba,
        +0x20, -0x40,
        +0x20, -0x40,
        +0x20, -0x40,
        +0x20, -0x20,
        +0x20, -0x40,
        +0x20, -0x40,
        +0x20, -0x40,
        +0x20, -0x40,
        -0xba,
    };

    // convert the deviceCode and commandCode to a stream of nibbles
    vector<char> nibbles;
    nibbles.push_back(deviceCode);
    nibbles.push_back(commandCode >> 4);
    nibbles.push_back(commandCode & 0x0f);

    // convert our nibbles to a stream of bits
    vector<char> bits;
    for (char n : nibbles) PushBits(n, bits);

    // add bits to the edge data
    for (char bit : bits)
    {
        edges.push_back(bit == 1 ? -0x20 : +0x20);
        edges.push_back(bit == 1 ? +0x20 : -0x20);
    }

    // convert to pairs of +/- values
    vector<long> edgePairs;
    for (long e : edges)
    {
        if (edgePairs.empty())
        {
            edgePairs.push_back(e);
        }
        else
        {
            if (edgePairs.back() > 0 && e > 0)
                edgePairs.back() += e;
            else if (edgePairs.back() < 0 && e < 0)
                edgePairs.back() += e;
            else
                edgePairs.push_back(e);
        }
    }

    // If last edge is high we will need to add an low edge.
    if (edgePairs.back() > 0) edgePairs.push_back(-1);

    // convert paired edge data to Pronto stye On/Off burst pairs.
    vector<long> burst2Pairs;
    for (long e : edgePairs)
    {
        burst2Pairs.push_back( e < 0 ? -e : e);
    }

    PrintWord(RawDataCode);
    PrintWord(CarrierFrequency);
    PrintWord(Burst1PairCount);
    PrintWord(burst2Pairs.size() / 2);
    for (long w : burst2Pairs) PrintWord(w);

    return 0;
}
