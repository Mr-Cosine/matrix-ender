"""
Matrix.cpp Testcasing Utility
"""
from typing import List, Union
import numpy as np, os, subprocess
from pathlib import Path
import random
from sympy import Matrix
from time import time

# Config
VERBOSE = False
FAILED_CASES = False

# Project path
BASE_PATH = Path(os.getcwd()).parent.parent # Should be identical to the current project path (matrix-ender)
SRC_PATH = BASE_PATH / "src"
INC_PATH = BASE_PATH / "include"

# Testcase output
TOUT_PATH = BASE_PATH / "tmp"
os.makedirs(TOUT_PATH, exist_ok=True)

# Dependencies
DEPENDENCIES = [
    SRC_PATH / f for f in ["matrix.cpp", "rational.cpp"]
]

def r(msg: str) -> str:
    return f"\033[31m{msg}\033[0m"

def parse(literal: str, converter = float, row_delimiter = ';', column_delimiter = ',') -> List[List]:
    if literal.find(';') != -1 and literal.find(',') != -1:
        return [
            [ converter(entry) for entry in row.strip().split(column_delimiter) ]
            for row in literal.replace('[', '').replace(']', '').strip().split(row_delimiter)
        ]
    return converter(literal)

def stringnify(input: List[List[any]]):
    if (isinstance(input, list)):
        return ';'.join(','.join(row) for row in input)
    else:
        return input

def fprint(matrix: List[List]) -> bool:
    for row in matrix:
        print(row, end='\n' if row != matrix[-1] else '')

def case_of(filepath: Union[str, Path], inputs: List[str], matcher):
    # All testcases should be (Type, Type, Type, ...) -> Type || Type -> Type -> ... -> Type in curry form
    output = subprocess.run([
        str(filepath),
        *inputs
    ], capture_output=True, text=True)

    if output.returncode != 0:
        return [False, output.stderr]
    
    actual = np.ndarray(parse(output.stdout)) if isinstance(parse(output.stdout), list) else float(parse(output.stdout))

    if (len(inputs) == 1):
        expected = matcher(parse(inputs[0], float))
    else:
        expected = matcher([parse(each, float) for each in inputs])
    
    if VERBOSE:
        print(f"\nExpected:\n{expected}\n\nActual:\n{actual}\n")

    # Numeric comparison
    if isinstance(expected, float) and isinstance(actual, float):
        # Numeric comparison
        return [True] if abs(actual - expected) < 1e-7 else [False, actual, expected]
    elif isinstance(expected, np.ndarray) and isinstance(actual, np.ndarray):
        # Array comparison
        if np.allclose(actual, expected, atol=1e-7):
            return [True]
        else:
            return [False, actual, expected]
    else:
        # Type mismatch
        return [False, f"Type mismatch. Expected: {type(expected)} | Actual: {type(actual)}"]

def supplier(input_descriptor: str, larger_matrix: bool = False) -> List[str]:
    result = []
    r = (10, 100) if larger_matrix else (2, 7)
    X = random.randint(*r)
    for each in input_descriptor.split(','):
        if each.startswith("M-"):
            row, col = [x for x in each.replace("M-", "").split('x')]
            if row == 'X' and col == 'X': col = row = X
            else: row = int(row); col = int(col)
            matrix = ""
            for i in range(row):
                for j in range(col):
                    matrix += str(random.randint(1, 100))
                    if (j != col - 1): matrix += ','
                if (i != row - 1): matrix += ';'
            result.append(matrix)
        else:
            result.append(random.randint(1, 100))
    return result

"""
The testcase function
"""
def tester(TEST_MACRO: str, matcher, input_descriptor: str, count: int = 100):
    START = time() * 1000

    # Compile once
    FILEPATH = TOUT_PATH / f"test-{TEST_MACRO}.tmp.out"
    os.chdir(TOUT_PATH)
    compile_cmd = [
        "g++",
        f"-DPYTC={TEST_MACRO}",
        "-std=c++20",
        f"-I{INC_PATH}",
        *DEPENDENCIES,
        SRC_PATH / "matrix_tc_wrapper.cpp",
        f"-o{FILEPATH}"
    ]

    output = subprocess.run(compile_cmd, text=True, capture_output=True)
    if output.returncode != 0:
        print(output.stderr)

    print("=== matrix.cpp test suite ===\n")

    failed = []
    for i in range(count):
        data = supplier(input_descriptor, True)
        result = case_of(FILEPATH, data, matcher)
        print(result)
        if result[0]:
            print(f"Test {i + 1}: Passed")
        else:
            print(r(f"Test {i + 1}: Failed"))
            failed.append([i, [data, result[1:]]])

    print(f"\n=== TEST MACRO: {TEST_MACRO} ===\n\nTotal: {count}\n{r(f"Failed: {len(failed)}")}\n")

    if FAILED_CASES:
        print("=== Failed Cases ===\n")
        if len(failed) != 0:
            for idx, [inputs, outputs] in failed:
                print(f"\n=== At Index: {idx} ===\n")
                print(f"Input:\n{inputs}\n")

                print("Output:\n")
                if len(outputs) == 1:
                    print(r(f"stderr: {outputs[0]}"))
                else:
                    print(f"Expected:\n{outputs[1]}\nActual:\n{outputs[0]}\n")

        else:
            print("None\n")
    
    elapsed = round(time() * 1000 - START, 2)
    print(f"""
=== Runtime Analysis ===
          
{count} tests took {elapsed}ms to complete
Averaging: {round(elapsed/count, 2)}ms / test
""")

    print("=== Test Suite Ends Here ===")

if __name__ == "__main__":
    two_matrices = "M-XxX,M-XxX"
    single_matrix = "M-XxX"
    designated = "M-60x60"

    def add(inputs: List[List[any]]) -> np.ndarray:
        a, b = np.array(inputs[0]), np.array(inputs[1])
        return a + b
    
    def mult(inputs: List[List[any]]) -> np.ndarray:
        a, b = np.array(inputs[0]), np.array(inputs[1])
        return a @ b
    
    def rref(input: List[any]) -> np.ndarray:
        mat, _ = Matrix(np.array(input, dtype=float)).rref()
        return np.array(mat, dtype=float)
    
    def det(input: List[any]) -> np.ndarray:
        return float(Matrix(np.array(input, dtype=float)).det())

    tester(4, det, designated, 10)
