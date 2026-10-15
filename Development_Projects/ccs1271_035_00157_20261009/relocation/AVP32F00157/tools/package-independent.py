"""Compatibility entry: current delivery shares ONE configuration header."""
from pathlib import Path
import runpy
runpy.run_path(str(Path(__file__).with_name("package.py")))
