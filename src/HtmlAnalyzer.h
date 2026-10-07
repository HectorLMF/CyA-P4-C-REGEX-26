#pragma once
#include "HtmlTypes.hpp"
#include <string>

class HtmlAnalyzer {
public:
    explicit HtmlAnalyzer(const std::string &filename);
    HtmlDocument parse();

private:
    std::string filename_;
    void handleLine(const std::string &line, int lineno, HtmlDocument &doc, bool &inComment, std::string &commentBuf, int &commentStart);
    void processTag(const std::string &tagText, int lineno, HtmlDocument &doc);
    void extractAttributes(const std::string &text, Tag &tag);
};
