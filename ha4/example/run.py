from subprocess import Popen, PIPE
import sys


def main():
    build = sys.argv[1]
    example_in = "./" + build + "/data/example_in.txt"
    example_out = "./" + build + "/data/example_out.txt"
    executable = "./" + build + "/example"

    get_content = lambda path: [(f.read(), f.close()) for f in [open(path)]][0][0]
    almost_equal = lambda x, y, delta: abs(x - y) < delta

    p1 = Popen(['echo', get_content(example_in)], stdout=PIPE)
    p2 = Popen([executable], stdin=p1.stdout, stdout=PIPE, encoding='utf8')

    my_answer = p2.communicate()[0].strip()
    grind_truth = get_content(example_out).strip()

    assert almost_equal(float(my_answer), float(grind_truth), 1e-10)

    print("zafiksirovali")


if __name__ == "__main__":
    main()
