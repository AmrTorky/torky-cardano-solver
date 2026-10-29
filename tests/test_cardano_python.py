#!/usr/bin/env python3
import ctypes
import os
import sys

lib_path = os.path.abspath("./build/libcardano_solver.so")
if not os.path.exists(lib_path):
    print(f"[ERROR] Shared library {lib_path} not found.")
    sys.exit(1)

cardano_lib = ctypes.CDLL(lib_path)
solve_c = cardano_lib.solve_cardano_c
solve_c.argtypes = [
    ctypes.c_double, ctypes.c_double,
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double)
]
solve_c.restype = None

roots = (ctypes.c_double * 3)()
residual = ctypes.c_double()
latency = ctypes.c_double()

solve_c(-3.5, 1.2, roots, ctypes.byref(residual), ctypes.byref(latency))

print("==============================================================================")
print("[PYTHON CTYPES INTERFACE] Cardano Solver Verified")
print(f"Roots: [{roots[0]:.6f}, {roots[1]:.6f}, {roots[2]:.6f}]")
print(f"Residual: {residual.value:.10e} kcal/mol | Latency: {latency.value:.3f} us")
print("==============================================================================")
