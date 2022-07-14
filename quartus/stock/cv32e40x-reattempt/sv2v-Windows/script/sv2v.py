# Custom script to apply sv2v conversion to each file in the given folder.

import argparse
import logging
import os

### Parse Arguments ###
parser = argparse.ArgumentParser(
    description="Run sv2v.exe on all files ending with '.sv'"
    " within the given folder or subfolders."
)
# parser.add_argument(
#     "include",
#     metavar="I",
#     help="Top-level folder to search from. All subfolders will also be searched.",
# )
parser.add_argument(
    "--include",
    # metavar="I",
    nargs="+",
    required=True,
    help="Folders for which all *.sv files should be included in compilation."
    " Also include subfolders. Packages must be added first!",
)
parser.add_argument(
    "--write",
    # metavar="W",
    required=True,
    help="Top-level folder to write the converted files to."
    " Sublevel folder structure will be copied from source.",
)
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
for incl in args.include:
    try:
        assert os.path.exists(incl)
    except AssertionError:
        print(f'Error: Cannot open the include folder at "{incl}"')
logging.info(f"Using include={args.include}.")

try:
    os.makedirs(args.write, exist_ok=True)
    assert os.path.exists(args.write)
except AssertionError:
    print(f'Error: Cannot open the write folder at "{args.write}"')
logging.info(f"Using write={args.write}.")

try:
    end = os.path.basename(args.sv2v_exe)
    if end != "sv2v.exe":
        # Must search for file in folder and subfolders
        for root, dirs, files in os.walk(args.sv2v_exe):
            for file in files:
                if file == "sv2v.exe":
                    args.sv2v_exe = os.path.join(root, file)
                    break

    assert os.path.exists(args.sv2v_exe)
    assert os.path.isfile(args.sv2v_exe)
except AssertionError:
    print(f'Error: Cannot find sv2v.exe in "{args.sv2v_exe}"')
logging.info(f"Using sv2v_exe={args.sv2v_exe}.")


### Perform Conversion ###
logging.info("Started file conversion...\n")

sv_dirs = []
v_dirs = []

# for incl in args.include:
#     for root, dirs, files in os.walk(incl):
#         for file in files:
#             sv = os.path.join(root, file)
#             relpath = os.path.relpath(root, incl)
#             v = os.path.join(args.write, relpath, os.path.splitext(file)[0] + ".v")

#             if os.path.basename(sv) not in sv_dirs:
#                 sv_dirs.append(sv)
#                 v_dirs.append(v)
#                 logging.debug(f"Found {sv}, will convert as {v}...")
#             else:
#                 logging.debug(f"Found {sv}, ignoring as duplicate...")

# sv2v.exe will search within the folder of each given file, thus we only pass
# one file per directory.

for incl in args.include:
    for root, dirs, files in os.walk(incl):
        # Grab the first *.sv file out of each folder
        for file in files:
            if os.path.splitext(file)[1] == ".sv":
                sv = os.path.join(root, file)
                if sv not in sv_dirs:
                    sv_dirs.append(sv)
                    logging.debug(f"Found {sv}...")
                    break

print(args.include)
print(" ".join(sv_dirs))

# cmd = os.system(f'{args.sv2v_exe} {" ".join(sv_dirs)} --write={" ".join(v_dirs)}')
cmd = os.system(f'{args.sv2v_exe} {" ".join(sv_dirs)} --write=adjacent')
logging.info(f"Finished file conversion!")


# TODO
# - Make a pkg include command for pks that should be added to every conversion
# - Must list all files; split into batches (w/ pkgs appended) to avoid cmd line size limit.
