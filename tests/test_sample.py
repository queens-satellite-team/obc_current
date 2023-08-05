# Note: pytest will run any files using the naming convention:
# test_*.py or *_test.py

def add(a, b):
    return a + b


def test_addition():
    assert add(2, 3) == 5
