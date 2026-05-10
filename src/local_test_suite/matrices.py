"""
Matrix.cpp Testcasing Utility
"""
from typing import List
import numpy as np, os, subprocess
from pathlib import Path

TESTCASING_MACRO = "PYTC"
BASE_PATH = Path(os.getcwd()) / "src"
DEPENDENCIES = [
    BASE_PATH / f for f in ["matrix.cpp", "rational.cpp"]
]

def parse_matrix(literal: str, converter = float, row_delimiter = ';', column_delimiter = ',') -> List[List]:

    return [
        [ converter(entry) for entry in row.strip().split(column_delimiter) ]
        for row in literal.replace('[', '').replace(']', '').strip().split(row_delimiter)
    ]

def fprint(matrix: List[List]) -> None:
    for row in matrix:
        print(row, end='\n' if row != matrix[-1] else '')

def case_of(MACRO: str, matcher):
    compile_cmd = [
        "g++",
        f"-D{TESTCASING_MACRO}",
        f"-D{MACRO}",
        *DEPENDENCIES
    ]
    subprocess.run(compile_cmd)
    output = subprocess.run(BASE_PATH / "a.out", shell=True, text=True, capture_output=True)
    matrix = np.array(parse_matrix(output.stdout))
    

fprint(parse_matrix("1,2,3;5,6,7"))