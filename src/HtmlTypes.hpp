#pragma once
#include <string>
#include <vector>

/**
 * \brief Par nombre-valor de un atributo HTML.
 */
struct Attribute {
    std::string name; ///< Nombre del atributo
    std::string value; ///< Valor del atributo
};

/**
 * \brief Representa una etiqueta HTML encontrada en el documento.
 */
struct Tag {
    std::string name; ///< Nombre de la etiqueta (p.ej. "a", "img")
    int line = 0; ///< Línea donde aparece
    bool isClosing = false; ///< Si es etiqueta de cierre
    std::vector<Attribute> attributes; ///< Atributos asociados
    
    /**
     * \brief Añade un atributo a la etiqueta.
     */
    
    void addAttribute(const Attribute &a) { attributes.push_back(a); }
};

/**
 * \brief Comentario HTML, puede ocupar varias líneas.
 */
struct Comment {
    std::string text; ///< Contenido del comentario
    int startLine = 0; ///< Línea inicial
    int endLine = 0; ///< Línea final
};

/**
 * \brief Estructura que almacena los resultados del análisis.
 */
struct HtmlDocument {
    std::string filename; ///< Nombre del fichero analizado
    std::string description; ///< Descripción (primer comentario tras DOCTYPE si existe)
    bool has_doctype = false; ///< True si se detectó <!DOCTYPE html>
    int doctypeLine = -1; ///< Línea donde aparece el DOCTYPE
    std::vector<Tag> tags; ///< Lista de etiquetas encontradas
    std::vector<Comment> comments; ///< Lista de comentarios
    /**
     * \brief Añade una etiqueta al documento.
     */
    void addTag(const Tag &t) { tags.push_back(t); }

    /**
     * \brief Añade un comentario al documento.
     */
    void addComment(const Comment &c) { comments.push_back(c); }

    /**
     * \brief Asigna la descripción del documento.
     */
    void setDescription(const std::string &d) { description = d; }
};
