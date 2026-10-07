#include <fstream>
#include <iostream>

#include "HtmlAnalyzer.h"
#include "ReportGenerator.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input.html output.txt\n";
        return 1;
    }
    std::string input = argv[1];
    std::string output = argv[2];

    HtmlAnalyzer analyzer(input);
    HtmlDocument doc = analyzer.parse();

    std::ofstream ofs(output);
    if (!ofs) {
        std::cerr << "Cannot open output file\n";
        return 2;
    }
    ReportGenerator::generate(doc, ofs);
    return 0;
}
