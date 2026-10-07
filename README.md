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
