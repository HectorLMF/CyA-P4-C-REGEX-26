#pragma once

#include <ostream>

#include "HtmlTypes.hpp"

/**
 * \brief Generador de informes de salida a partir de HtmlDocument.
 */
class ReportGenerator {
   public:
    /**
     * \brief Genera el informe formateado en el stream de salida.
     * \param doc Documento analizado.
     * \param os Stream donde escribir el informe.
     */
    static void generate(const HtmlDocument &doc, std::ostream &os);
};
