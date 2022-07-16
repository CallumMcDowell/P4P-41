# Custom script to apply sv2v conversion to each file in the given folder.

import argparse
import logging
import os

### Parse Arguments ###
parser = argparse.ArgumentParser(
    description="Run sv2v.exe on all files ending with '.sv'"
    " within the given folder or subfolders."
)
parser.add_argument(
    "--pkg",
    nargs="+",
    required=True,
    help="Folder in which all *.sv files are packages and should be included"
    " in the conversion of each file specified by '--include'. Also include subfolders.",
)
parser.add_argument(
    "--include",
    nargs="+",
    required=True,
    help="Folders for which all *.sv files should be included in compilation."
    " Also include subfolders.",
)
# parser.add_argument(
#     "--write",
#     required=True,
#     help="Top-level folder to write the converted files to."
#     " Sublevel folder structure will be copied from source.",
# )
parser.add_argument(
    "--sv2v_exe",
    default=os.path.dirname(os.getcwd()),
    help="Folder in which the sv2v.exe executable is. By default will search the"
    " folder containing the folder the script is executing in (and other subfolders).",
)

args = parser.parse_args()
logging.basicConfig(level=logging.INFO)
logging.info("\n\nSTART\n")


### Validate Inputs ###
for pkg in args.pkg:
    try:
        assert os.path.exists(pkg)
    except AssertionError:
        print(f'Error: Cannot open the pkg folder at "{pkg}"')
logging.info(f"Using pkg={args.pkg}.")

for incl in args.include:
    try:
        assert os.path.exists(incl)
    except AssertionError:
        print(f'Error: Cannot open the include folder at "{incl}"')
logging.info(f"Using include={args.include}.")

# try:
#     os.makedirs(args.write, exist_ok=True)
#     assert os.path.exists(args.write)
# except AssertionError:
#     print(f'Error: Cannot open the write folder at "{args.write}"')
# logging.info(f"Using write={args.write}.")

try:
    end = os.path.basename(args.sv2v_exe)
    if end != "sv2v.exe":
        # Must search for file in folder and subfolders
        for root, dirs, files in os.walk(args.sv2v_exe):
            for file in files:
                if file == "sv2v.exe":
                    args.sv2v_exe = os.path.join(root, file)
                    break  # todo: break all nested loops with inner function

    assert os.path.exists(args.sv2v_exe)
    assert os.path.isfile(args.sv2v_exe)
except AssertionError:
    print(f'Error: Cannot find sv2v.exe in "{args.sv2v_exe}"')
logging.info(f"Using sv2v_exe={args.sv2v_exe}.")


### Identify Packages ###
logging.info("Started file conversion...\n")

pkg_sv_dirs = []
pkg_v_dirs = []

for pkg in args.pkg:
    for root, dirs, files in os.walk(pkg):
        for file in files:
            if os.path.splitext(file)[1] == ".sv":
                sv = os.path.join(root, file)
                pkg_sv_dirs.append(sv)

                v = os.path.splitext(sv)[0] + ".v"
                pkg_v_dirs.append(v)

pkg_msg = "\n".join(pkg_sv_dirs)
logging.info(f"Discovered packages:\n{pkg_msg}")

pkg_msg = "\n".join(pkg_v_dirs)
input = input(
    "These generated files will be deleted during the process of conversion:\n"
    f" {pkg_msg}\n"
    "Continue: Y/N ?"
)
if input.lower() != "y":
    logging.warning("Conversion cancelled.")
    exit()


### Perform Conversion ###
sv_dirs = []

for incl in args.include:
    for root, dirs, files in os.walk(incl):
        for file in files:
            if os.path.splitext(file)[1] == ".sv":
                # delete generated pkg files since sv2v cannot overwrite them at time of release v0.0.9
                for pkg_v in pkg_v_dirs:
                    if os.path.exists(pkg_v) and os.path.isfile(pkg_v):
                        logging.debug(f"Deleting generated {pkg_v}")
                        os.remove(pkg_v)

                # convert file and create *.v
                sv = os.path.join(root, file)
                sv_dirs.append(sv)
                logging.info(f"Discovered {file} at {sv}")

                # For individual file conversion:
                # logging.info(f"Converting {file} at {sv}")
                # cmd = os.system(
                #     f'{args.sv2v_exe} {" ".join(pkg_sv_dirs)} {sv} --write=adjacent'
                # )

# Standard conversion:
cmd = os.system(
    f'{args.sv2v_exe} --verbose {" ".join(pkg_sv_dirs)} {" ".join(sv_dirs)} --write=adjacent'
)

# Debugging:
# cmd = os.system(
#     f'{args.sv2v_exe} --verbose {" ".join(pkg_sv_dirs)} {" ".join(sv_dirs)} --write=adjacent --dump-prefix={"C:/Users/CM/Documents/Git/P4P-41/quartus/stock/cv32e40x-reattempt/rtl/generated/temp_"}'
# )

logging.info(f"Finished file conversion!")
