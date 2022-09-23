#include "Analyzer.h"
#include "stdlib.h" 
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Missing Input File" << std::endl;
    }

    Analyzer analyzer;
    analyzer.Analyze(std::string(argv[1]));

    analyzer.Print();

    system("PAUSE");

    return 0;
}