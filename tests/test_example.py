"""pytest automatically discovers and runs all files
starting test_ or ending with _test.
"""
import sys
import os

# allows us to import methods from a parent directory
parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(parent_dir)

from example_methods import return_int


def test_return_int():
    """example of testing 1 input case for the method 'return_int'
    compare the result of the method with the expected value and 'assert'
    their equality """
    assert return_int(3) == 3
