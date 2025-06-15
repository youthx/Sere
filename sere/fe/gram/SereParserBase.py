from antlr4 import *  # type: ignore


class SereParserBase(Parser):
    def CannotBePlusMinus(self) -> bool:
        return True

    def CannotBeDotLpEq(self) -> bool:
        return True
