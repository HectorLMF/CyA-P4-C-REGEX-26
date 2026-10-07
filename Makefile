CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -I./src
SRCDIR = src
BUILDDIR = build
TARGET = p04_html_analyzer
REPORTS_DIR = reports
EXAMPLES_DIR = examples

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

run-example: all
	./$(TARGET) examples/pagina.html pagina_report.txt

reports: all
	@mkdir -p $(REPORTS_DIR)
	for f in $(EXAMPLES_DIR)/*.html; do \
		base=$$(basename $$f .html); \
		./$(TARGET) $$f $(REPORTS_DIR)/$$base"_report.txt"; \
	done

package: reports
	@mkdir -p dist
	tar -czf dist/$(TARGET)_submission.tar.gz src examples README.md Makefile $(REPORTS_DIR)

