#pragma once

#include <string>

#include "HtmlTypes.hpp"

/**
 * \brief Analizador simple de HTML que usa expresiones regulares.
 *
 * La clase lee un fichero HTML línea a línea y extrae:
 * - Declaración DOCTYPE
 * - Comentarios (línea y multilínea)
 * - Etiquetas específicas (html, head, title, body, h1, p, a, img)
 * - Atributos en la forma name="value"
 */
class HtmlAnalyzer {
   public:
    /**
     * \brief Construye el analizador para un fichero.
     * \param filename Ruta al fichero HTML de entrada.
     */
    explicit HtmlAnalyzer(const std::string &filename);

    /**
     * \brief Ejecuta el análisis y devuelve la representación interna.
     * \return HtmlDocument con la información extraída.
     */
    HtmlDocument parse();

   private:
    std::string filename_;

    void handleLine(const std::string &line, int lineno, HtmlDocument &doc, bool &inComment,
                    std::string &commentBuf, int &commentStart);

    void processTag(const std::string &tagText, int lineno, HtmlDocument &doc);

    void extractAttributes(const std::string &text, Tag &tag);
};
