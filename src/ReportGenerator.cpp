#include "ReportGenerator.h"

#include <iostream>

void ReportGenerator::generate(const HtmlDocument &doc, std::ostream &os) {
    os << "PROGRAM: " << doc.filename << "\n\n";
    os << "DESCRIPTION:\n";
    if (!doc.comments.empty()) {
        // If first comment immediately after DOCTYPE, use as description
        if (doc.has_doctype && !doc.comments.empty() &&
            doc.comments.front().startLine == doc.doctypeLine + 1) {
            os << doc.comments.front().text << "\n\n";
        } else if (!doc.comments.empty()) {
            // print first comment as description
            os << doc.comments.front().text << "\n\n";
        }
    } else {
        os << "\n";
    }

    os << "STRUCTURE:\n";
    os << "HTML : " << (doc.has_doctype ? "True" : "False") << "\n";
    // HEAD/BODY detection
    bool hasHead = false, hasBody = false;
    for (auto &t : doc.tags) {
        std::string n = t.name;
        for (auto &c : n) c = std::tolower(c);
        if (n == "head") hasHead = true;
        if (n == "body") hasBody = true;
    }
    os << "HEAD : " << (hasHead ? "True" : "False") << "\n";
    os << "BODY : " << (hasBody ? "True" : "False") << "\n";
    os << "DOCTYPE : " << (doc.has_doctype ? "HTML5" : "Unknown") << "\n\n";

    os << "TAGS:\n";
    for (auto &t : doc.tags) {
        os << "[ Line " << t.line << "] " << (t.isClosing ? "/" : "") << t.name << "\n";
    }
    os << "\nATTRIBUTES:\n";
    for (auto &t : doc.tags) {
        if (!t.attributes.empty()) {
            os << "[ Line " << t.line << "] " << t.name << "\n";
            for (auto &a : t.attributes) {
                os << a.name << " = \"" << a.value << "\"\n";
            }
            os << "\n";
        }
    }

    os << "COMMENTS:\n";
    for (auto &c : doc.comments) {
        if (c.startLine == c.endLine)
            os << "[ Line " << c.startLine << "]\n";
        else
            os << "[ Line " << c.startLine << " -" << c.endLine << "]\n";
        os << "<!--" << c.text << "-->\n\n";
    }
}
