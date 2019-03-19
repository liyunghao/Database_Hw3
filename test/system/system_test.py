import sys
import os
import importlib
import subprocess
import filecmp

def setup_output_dir(output_dir):
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)

def execute_testsuite(exe, suite_path, suite_out_path, suite_ans_path):
    correct_count = 0
    suite_name = os.path.basename(suite_path)
    suite_mod_path = os.path.join(suite_path, "suite.py")
    suite = None
    if os.path.exists(suite_mod_path):
        module_path = ".".join(["testcases", suite_name, "suite"])
        mod = importlib.import_module(module_path)
        suite = getattr(mod, "Suite")

    case_files = list()
    for name in os.listdir(suite_path):
        if name.startswith("t") and name.endswith(".txt"):
            case_files.append(name)

    for case in case_files:
        if suite:
            suite.setUp()

        print("Exe case:", case)
        case_path = os.path.join(suite_path, case)
        out_path = os.path.join(suite_out_path, case)
        ans_path = os.path.join(suite_ans_path, case)
        print("output file: ", out_path)
        print("answer file: ", ans_path)
        with open(case_path) as fp:
            content = fp.readlines()
        content.insert(0, ".output {out_path}\n".format(out_path=out_path))
        p = subprocess.Popen([exe], stdin=subprocess.PIPE) 
        for line in content:
            p.stdin.write(line.encode())

        p.stdin.close()
        p.wait()

        if suite:
            suite.tearDown()

        is_result_match = filecmp.cmp(out_path, ans_path)
        if is_result_match:
            correct_count += 1

    return correct_count, len(case_files)

def main():
    if len(sys.argv) == 2:
        target = "all"  
    else:
        target = sys.argv[2:]
    
    file_path = os.path.dirname(__file__)
    testcase_path = os.path.join(file_path, "testcases")
    output_path = os.path.join(file_path, "output")
    answer_path = os.path.join(file_path, "answer")

    if target == "all":
        target = os.listdir(testcase_path)
    
    for test_suite in target:
        if test_suite == "__pycache__":
            continue

        suite_path = os.path.join(testcase_path, test_suite)
        suite_out_path = os.path.join(output_path, test_suite)
        suite_ans_path = os.path.join(answer_path, test_suite)

        if os.path.isdir(suite_path):
            setup_output_dir(suite_out_path)
            ret = execute_testsuite(sys.argv[1], suite_path, suite_out_path, suite_ans_path)
            correct_count, total_count = ret
            print(correct_count, total_count)

if __name__ == "__main__":
    main()