#include "HtmlAnalyzer.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

HtmlAnalyzer::HtmlAnalyzer(const std::string &filename) : filename_(filename) {}

HtmlDocument HtmlAnalyzer::parse() {
    HtmlDocument doc;
    doc.filename = filename_;
    std::ifstream ifs(filename_);
    if (!ifs) return doc;

    std::string line;
    int lineno = 0;
    bool inComment = false;
    std::string commentBuf;
    int commentStart = -1;

    while (std::getline(ifs, line)) {
        ++lineno;
        handleLine(line, lineno, doc, inComment, commentBuf, commentStart);
    }

    // If file ends while in comment, flush
    if (inComment) {
        Comment c; c.text = commentBuf; c.startLine = commentStart; c.endLine = lineno; doc.addComment(c);
    }

    return doc;
}

void HtmlAnalyzer::handleLine(const std::string &line, int lineno, HtmlDocument &doc, bool &inComment, std::string &commentBuf, int &commentStart) {
    std::string s = line;
    // detect doctype
    std::regex doctype_re(R"(^\s*<!DOCTYPE\s+html[^>]*>\s*$)", std::regex::icase);
    if (std::regex_search(s, doctype_re)) {
        doc.has_doctype = true;
        if (doc.doctypeLine == -1) doc.doctypeLine = lineno;
    }

    // comments handling
    size_t pos = 0;
    while (pos < s.size()) {
        if (!inComment) {
            size_t start = s.find("<!--", pos);
            if (start == std::string::npos) break;
            size_t end = s.find("-->", start+4);
            if (end != std::string::npos) {
                // single-line comment
                Comment c; c.startLine = lineno; c.endLine = lineno;
                c.text = s.substr(start+4, end-(start+4));
                doc.addComment(c);
                pos = end + 3;
            } else {
                // start multiline
                inComment = true;
                commentStart = lineno;
                commentBuf = s.substr(start+4);
                break;
            }
        } else {
            size_t end = s.find("-->");
            if (end != std::string::npos) {
                // end comment
                commentBuf += "\n" + s.substr(0, end);
                Comment c; c.startLine = commentStart; c.endLine = lineno; c.text = commentBuf; doc.addComment(c);
                inComment = false; commentBuf.clear(); pos = end + 3;
            } else {
                commentBuf += "\n" + s;
                break;
            }
        }
    }

    // tags processing (only when not inside comment)
    if (!inComment) {
        std::regex tag_re(R"(<\s*/?\s*([a-zA-Z0-9]+)([^>]*)>)");
        auto begin = std::sregex_iterator(s.begin(), s.end(), tag_re);
        auto endIt = std::sregex_iterator();
        for (auto it = begin; it != endIt; ++it) {
            std::smatch m = *it;
            std::string full = m.str(0);
            std::string name = m.str(1);
            std::string rest = m.str(2);
            std::string tagText = full;
            // process
            Tag t; t.name = name; t.line = lineno;
            // detect closing
            std::regex closing_re(R"(^\s*<\s*/)" );
            if (std::regex_search(full, closing_re)) t.isClosing = true; else t.isClosing = false;
            extractAttributes(rest, t);
            // Only record allowed tags
            std::string lower = name;
            for (auto &c : lower) c = std::tolower(c);
            if (lower == "html" || lower == "head" || lower == "title" || lower == "body" || lower == "h1" || lower == "p" || lower == "a" || lower == "img") {
                doc.addTag(t);
            }
        }
    }
}

void HtmlAnalyzer::extractAttributes(const std::string &text, Tag &tag) {
    std::regex attr_re("([a-zA-Z_:][-a-zA-Z0-9_:.]*)\\s*=\\s*\"([^\"]*)\"");
    auto itBegin = std::sregex_iterator(text.begin(), text.end(), attr_re);
    auto itEnd = std::sregex_iterator();
    for (auto it = itBegin; it != itEnd; ++it) {
        std::smatch m = *it;
        Attribute a; a.name = m.str(1);
        a.value = m.str(2);
        tag.addAttribute(a);
    }
}
