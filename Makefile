CXX ?= g++
CXXFLAGS = -O3 -std=c++20 -march=native -Wall -Wextra -Iinclude -pthread

TARGET = build/torky-cardano-cli
BENCH_BIN = build/cardano_benchmark
SHARED_LIB = build/libcardano_solver.so

all: $(TARGET) $(BENCH_BIN) $(SHARED_LIB)

$(TARGET): src/torky_cli_kernel.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/torky_cli_kernel.cpp -o $(TARGET)

$(BENCH_BIN): src/solver/cardano_benchmark.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/solver/cardano_benchmark.cpp -o $(BENCH_BIN)

$(SHARED_LIB): src/bindings/py_cardano.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -fPIC -shared src/bindings/py_cardano.cpp -o $(SHARED_LIB)

test: all
	./$(TARGET)
	./$(BENCH_BIN) -3.5 1.2
	python3 tests/test_cardano_python.py
	./run_audit.sh

clean:
	rm -rf build local_verification_certificate.json
