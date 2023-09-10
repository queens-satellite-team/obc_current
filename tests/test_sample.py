# Note: pytest will run any files using the naming convention:
# test_*.py or *_test.py

def method_to_test(a, b):
    return a + b


def sample_test():
    assert method_to_test(2, 3) == 5
