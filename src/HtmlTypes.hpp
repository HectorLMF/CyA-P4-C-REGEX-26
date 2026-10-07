#pragma once
#include <string>
#include <vector>

struct Attribute {
    std::string name;
    std::string value;
};

struct Tag {
    std::string name;
    int line = 0;
    bool isClosing = false;
    std::vector<Attribute> attributes;
    void addAttribute(const Attribute &a) { attributes.push_back(a); }
};

struct Comment {
    std::string text;
    int startLine = 0;
    int endLine = 0;
};

struct HtmlDocument {
    std::string filename;
    std::string description;
    bool has_doctype = false;
    int doctypeLine = -1;
    std::vector<Tag> tags;
    std::vector<Comment> comments;

    void addTag(const Tag &t) { tags.push_back(t); }
    void addComment(const Comment &c) { comments.push_back(c); }
    void setDescription(const std::string &d) { description = d; }
};
