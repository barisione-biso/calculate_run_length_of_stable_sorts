// CPP program to implement run length encoding
#include <bits/stdc++.h>

// include input and output archivers
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// include this header to serialize vectors
#include <boost/serialization/vector.hpp>

using namespace std;
 //taken from https://www.geeksforgeeks.org/run-length-encoding/
void printRLE(const std::vector<uint64_t>& v)
{
    int n = v.size();
    for (int i = 0; i < n; i++) {
 
        // Count occurrences of current character
        int count = 1;
        while (i < n - 1 && v[i] == v[i + 1]) {
            count++;
            i++;
        }
        // Print character and its count
        std::cout << v[i] << " " << count << std::endl;
    }
}
//Driver code
int main(int argc, char **argv)
{
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << "<dataset>" << std::endl;
        return 0;
    }

    std::vector<std::uint64_t> v;
    /*
    std::string input_file= argv[1];
    std::ifstream ifs(input_file);
    v.reserve(81426573);
    std::string str;
    while (ifs.good()) {
        ifs >> str;
        v.emplace_back(str);
    }*/
    std::ifstream sif( argv[1] );
    boost::archive::text_iarchive iarch2(sif);
    iarch2 >> v;

    printRLE(v);
    return 0;
}