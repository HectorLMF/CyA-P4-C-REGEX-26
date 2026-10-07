# p04_html_analyzer

Analizador HTML sencillo en C++ (práctica 4). Construido con clases separadas en `.h` y `.cpp`.

Build:

```bash
make
```

Run example:

```bash
make run-example
```

Usage:

```bash
./p04_html_analyzer input.html output.txt
```

Estructura:

- `src/HtmlTypes.hpp` — modelos `Tag`, `Attribute`, `Comment`, `HtmlDocument`.
- `src/HtmlAnalyzer.h/.cpp` — parser principal usando `std::regex`.
- `src/ReportGenerator.h/.cpp` — genera informe de salida.
- `src/main.cpp` — CLI.

Descripción de clases y métodos
--------------------------------

- `HtmlDocument` (src/HtmlTypes.hpp): representa el documento analizado.
	- Atributos: `filename`, `description`, `has_doctype`, `doctypeLine`, `tags`, `comments`.
	- Métodos: `addTag(const Tag&)`, `addComment(const Comment&)`, `setDescription(const std::string&)`.

- `Tag` (src/HtmlTypes.hpp): representa una etiqueta HTML encontrada.
	- Atributos: `name`, `line`, `isClosing`, `attributes`.
	- Métodos: `addAttribute(const Attribute&)`.

- `Attribute` (src/HtmlTypes.hpp): par nombre/valor de un atributo HTML.
	- Atributos: `name`, `value`.

- `Comment` (src/HtmlTypes.hpp): comentario HTML (puede ser multilínea).
	- Atributos: `text`, `startLine`, `endLine`.

- `HtmlAnalyzer` (src/HtmlAnalyzer.h/.cpp): clase responsable del análisis.
	- Constructor: `HtmlAnalyzer(const std::string &filename)` inicializa con la ruta del fichero.
	- `HtmlDocument parse()` — Ejecuta el análisis completo y devuelve un `HtmlDocument` con:
		- Detección de `<!DOCTYPE html>`.
		- Extracción de comentarios (línea y multilínea).
		- Detección de etiquetas específicas (`html`, `head`, `title`, `body`, `h1`, `p`, `a`, `img`).
		- Extracción de atributos en la forma `name="value"` usando `std::regex`.
	- Métodos privados auxiliares: `handleLine(...)`, `processTag(...)`, `extractAttributes(...)`.

- `ReportGenerator` (src/ReportGenerator.h/.cpp): genera el informe de salida.
	- `static void generate(const HtmlDocument &doc, std::ostream &os)` — escribe el resumen con:
		- Información general (`PROGRAM`, `DESCRIPTION`).
		- Estructura básica (`HTML`, `HEAD`, `BODY`, `DOCTYPE`).
		- Lista de `TAGS` con línea y si son de cierre.
		- `ATTRIBUTES` por etiqueta con pares `name = "value"`.
		- `COMMENTS` con rango de líneas y texto.

Notas sobre limitaciones
-----------------------

- El analizador está diseñado para los objetivos de la práctica y utiliza `std::regex`; no es un parser HTML completo.
- Solo se extraen atributos escritos con comillas dobles (`name="value"`). Atributos con comillas simples o sin comillas no se detectan.
- Solo se procesan las etiquetas indicadas en el enunciado; otras etiquetas se ignoran pero no provocan error.

