from typing import Optional, Dict, List, Any


class SemanticError(Exception):
    def __init__(
        self, message: str, line: Optional[int] = None, column: Optional[int] = None
    ):
        self.line = line
        self.column = column
        location = (
            f" (line {line}, col {column})"
            if line is not None and column is not None
            else ""
        )
        super().__init__(f"Error{location}: {message}")
