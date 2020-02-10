import sys
import os
import difflib

from subprocess import Popen, PIPE, STDOUT

TEMP_FILE = "_temp.txt"
DIR_PATH = os.path.dirname(os.path.abspath(__file__))
PROJ_PATH = os.path.abspath(os.path.join(DIR_PATH, os.pardir))
EXEC = "compiler"

def run():
    pass

def compare(lines1, lines2, f1, f2):
    for line in difflib.unified_diff(lines1, lines2, fromfile=f1, tofile=f2, lineterm=''):
        print(line)

def read_file(file):
    with open(file, 'r') as f:
        return f.readlines()

def generate():
    files = [x for x in os.listdir(DIR_PATH) if x.endswith(".input")]
    for input in files:
        output = input[:-6] + ".output"
        command = [PROJ_PATH, "/", EXEC, " ", DIR_PATH, "/", input]
        p = Popen(["".join(command)], stdout=PIPE, stderr=PIPE, shell=True)
        out = p.stderr.read().decode("utf-8")

        if not out:
            os.system("cc -o out out.s ./examples/printint.c")
            p = Popen(["./out"], stdout=PIPE, stderr=STDOUT, shell=True)
            with open(DIR_PATH + "/" + output, 'w') as f:
                f.write(p.stdout.read().decode("utf-8"))
        else:
            with open(DIR_PATH + "/" + output, 'w') as f:
                f.write(out)

def main(args):
    files = [x for x in os.listdir(DIR_PATH) if x.endswith(".input")]
    for input in files:
        output = input[:-6] + ".output"

        if not os.path.isfile(DIR_PATH + "/" + output):
            continue

        command = [PROJ_PATH, "/", EXEC, " ", DIR_PATH, "/", input]

        p = Popen(["".join(command)], stdout=PIPE, stderr=PIPE, shell=True)
        out = p.stderr.read().decode("utf-8")

        if not out:
            os.system("cc -o out out.s ./examples/printint.c")
            p = Popen(["./out"], stdout=PIPE, stderr=STDOUT, shell=True)
            out = p.stdout.read().decode("utf-8")

        with open(DIR_PATH + "/" + TEMP_FILE, 'w') as f:
            f.write(out)

        compare(read_file(DIR_PATH + "/" + TEMP_FILE), read_file(DIR_PATH + "/" + output), input, output)

    os.remove(DIR_PATH + "/" + TEMP_FILE)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        main(sys.argv)
    elif sys.argv[1] == "-g":
        generate()
    else:
        print("Unrecognized input options %s" % sys.argv)