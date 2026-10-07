#include "ReportGenerator.h"

#include <iostream>
#include <optional>
#include <algorithm>
#include <cctype>
#include <string>

void ReportGenerator::generate(const HtmlDocument &doc, std::ostream &os) {
    os << "PROGRAM: " << doc.filename << "\n\n";
    os << "DESCRIPTION:\n";
    if (!doc.comments.empty()) {
        // Si el primer comentario está inmediatamente después del DOCTYPE, usarlo como descripción
        if (doc.has_doctype && !doc.comments.empty() &&
            doc.comments.front().startLine == doc.doctypeLine + 1) {
            os << doc.comments.front().text << "\n\n";
        } else if (!doc.comments.empty()) {
            // imprimir el primer comentario como descripción
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

    // MODIFICADO: Se añadió la sección IMAGES y lambdas auxiliares
    // para extraer valores de atributos y extensiones de fichero
    // de las etiquetas <img>.
    // Se imprimen ALT y EXTENSION (incluyendo el punto) para cada imagen.
    // Los helpers son lambdas locales para no alterar las cabeceras.
    //
    // Justificación: cumplir el requisito de listar ALT y la
    // extensión de archivo para cada <img> en una sección IMAGES.
    // La implementación trata nombres de atributos insensible a mayúsculas.
    //
    // Nota: las etiquetas que abarcan varias líneas no son manejadas
    // por este analizador basado en expresiones regulares; considerar
    // un preprocesado si se requiere soporte para etiquetas multilínea.
    //
    // Sección IMAGES: listar cada etiqueta <img> con ALT y EXTENSION
    auto getAttributeValue = [](const Tag &t, const std::string &name) -> std::optional<std::string> {
        for (auto &a : t.attributes) {
            std::string n = a.name;
            std::transform(n.begin(), n.end(), n.begin(), [](unsigned char c) { return std::tolower(c); });
            if (n == name) return a.value;
        }
        return std::nullopt;
    };

    auto file_extension = [](const std::string &path) -> std::string {
        size_t q = path.find_first_of("?#");
        std::string p = (q == std::string::npos) ? path : path.substr(0, q);
        size_t slash = p.find_last_of('/');
        size_t pos = p.find_last_of('.');
        if (pos == std::string::npos) return std::string();
        if (slash != std::string::npos && pos < slash) return std::string();
        return p.substr(pos); // includes the dot
    };

    os << "IMAGES:\n";
    for (auto &t : doc.tags) {
        std::string n = t.name;
        std::transform(n.begin(), n.end(), n.begin(), [](unsigned char c) { return std::tolower(c); });
        if (n != "img") continue;
        os << "[Line " << t.line << "]\n";
        auto alt = getAttributeValue(t, "alt");
        os << "ALT: " << (alt ? *alt : "") << "\n";
        auto src = getAttributeValue(t, "src");
        std::string ext = src ? file_extension(*src) : std::string();
        os << "EXTENSION: " << ext << "\n";
    }

    os << "\n";

    os << "COMMENTS:\n";
    for (auto &c : doc.comments) {
        if (c.startLine == c.endLine)
            os << "[ Line " << c.startLine << "]\n";
        else
            os << "[ Line " << c.startLine << " -" << c.endLine << "]\n";
        os << "<!--" << c.text << "-->\n\n";
    }
}
