"""
Matrix.cpp Testcasing Utility
"""
from typing import List
import numpy as np, os, subprocess
from pathlib import Path
import random

BASE_PATH = Path(os.getcwd()) / "src"
DEPENDENCIES = [
    BASE_PATH / f for f in ["matrix.cpp", "rational.cpp"]
]

def r(msg: str) -> str:
    return f"\033[31m{msg}\033[0m"

def parse_matrix(literal: str, converter = float, row_delimiter = ';', column_delimiter = ',') -> List[List]:

    return [
        [ converter(entry) for entry in row.strip().split(column_delimiter) ]
        for row in literal.replace('[', '').replace(']', '').strip().split(row_delimiter)
    ]

def fprint(matrix: List[List]) -> bool:
    for row in matrix:
        print(row, end='\n' if row != matrix[-1] else '')

def case_of(MACRO: str, inputs: List[str], matcher):
    compile_cmd = [
        "g++",
        f"-DPYTC={MACRO}",
        *[f"-DINPUT{i}={data}" for i, data in enumerate(inputs)],
        *DEPENDENCIES
    ]
    subprocess.run(compile_cmd)
    output = subprocess.run(BASE_PATH / "a.out", shell=True, text=True, capture_output=True)

    # All testcases should be (Type, Type, Type, ...) -> Type || Type -> Type -> ... -> Type in curry form
    actual = np.array(parse_matrix(output.stdout))
    expected = matcher(inputs)

    if (actual == expected):
        return True
    else:
        return False

def supplier(input_descriptor: str) -> List[str]:
    result = []
    for each in input_descriptor.split(','):
        if each.startswith("M-"):
            [ row, col ] = [int(x) for x in each.replace("M-", "").split('x')]
            matrix = ""
            for i in range(row):
                for j in range(col):
                    matrix += str(random.randint(1, 100))
                    if (j != col - 1): matrix += ','
                if (i != row - 1): matrix += ';'
            result.append(result)
        else:
            result.append(random.randint(1, 100))
    return result


def tester(TEST_MACRO: str, matcher, input_descriptor: str, count: int = 100):
    for i in range(count):
        failed_case = []
        data = supplier(input_descriptor)
        result = case_of(TEST_MACRO, data)
        if result:
            print(f"Test {i + 1}: Passed")
        else:
            print(r(f"Test {i + 1}: Failed"))
            failed_case.append([i, data])
    print(f"=== TEST MACRO: {TEST_MACRO} ===\nTotal: {count}\n{r(f"Failed: {len(failed_case)}")}")

    for idx, data in failed_case:
        print(f"=== At Index: {idx} ===\nInput: {data}")

    print("=== Test Suite Ends Here ===")

fprint(parse_matrix("1,2,3;5,6,7"))
