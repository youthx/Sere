# main.py

import sys
import os
import time
import logging
import pathlib
import argparse

from sere.fe.gram.compile import compile
from sere.be.pipeline import (
    compile_source,
    emit_artifacts,
)  # assume emit_artifacts handles obj/asm/ir


def init_logging(debug: bool) -> None:
    logging.basicConfig(
        level=logging.DEBUG if debug else logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        handlers=[logging.StreamHandler(sys.stdout)],
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Sere Compiler")
    parser.add_argument("source", type=pathlib.Path, help="Source .sere file")
    parser.add_argument(
        "-o", "--output", type=pathlib.Path, help="Output basename (no extension)"
    )
    parser.add_argument(
        "--rebuild", action="store_true", help="Force regenerate ANTLR parser"
    )
    parser.add_argument(
        "--target",
        choices=["ir", "obj", "asm"],
        default="obj",
        help="Output target format (default: obj)",
    )
    parser.add_argument("--debug", action="store_true", help="Enable verbose logging")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    init_logging(args.debug)

    if args.rebuild:
        logging.info("Rebuilding ANTLR grammar...")
        compile()

    if not args.source.exists():
        logging.error(f"Source file not found: {args.source}")
        sys.exit(1)

    try:
        with open(args.source, "r") as f:
            source_code = f.read()

        logging.info("Compiling source...")
        start_time = time.perf_counter()

        module_ir = compile_source(source_code, filename=str(args.source))

        duration = time.perf_counter() - start_time

        # Determine output basename
        if args.output:
            output_basename = args.output.with_suffix("")  # strip extension if any
        else:
            output_basename = args.source.with_suffix("")
        artifacts = emit_artifacts(module_ir, basename=str(output_basename))

        if args.target == "ir":
            output_file = output_basename.with_suffix(".ll")
            with open(output_file, "w") as out:
                out.write(artifacts["ir"])
            logging.info(f"Emitted LLVM IR to: {output_file}")

        elif args.target in ("obj", "asm"):
            # Use your emit_artifacts function, but allow filtering output
            # We'll emit all and then delete the unused if you want,
            # or just emit specific files (adjust emit_artifacts if needed)

            # Inform user which files were emitted
            if args.target == "obj":
                output_file = output_basename.with_suffix(".o")
                with open(output_file, "wb") as out:
                    out.write(artifacts["obj"])

                logging.info(f"Emitted object file: {output_basename}.o")
            elif args.target == "asm":
                output_file = output_basename.with_suffix(".s")
                with open(output_file, "w") as out:
                    out.write(artifacts["asm"])

                logging.info(f"Emitted assembly file: {output_basename}.s")

        else:
            logging.error(f"Unknown target: {args.target}")

        logging.info(f"Compilation completed in {duration:.3f}s")

    except Exception:
        logging.exception("Compilation failed")
        sys.exit(1)


if __name__ == "__main__":
    main()
