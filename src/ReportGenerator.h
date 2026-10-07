#pragma once
#include "HtmlTypes.hpp"
#include <ostream>

class ReportGenerator {
public:
    static void generate(const HtmlDocument &doc, std::ostream &os);
};
