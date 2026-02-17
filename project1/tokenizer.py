import sys
class Tokenizer:
     def __init__(self, input_file, reserved_words, symbols, integer, identifier, end_of_file, error_token):
        # Save token tables / constants
        self.reserved_words = reserved_words
        self.symbols = symbols
        self.INTEGER = integer
        self.IDENTIFIER = identifier
        self.EOF = end_of_file
        self.ERROR = error_token
        # Open the file and keep it open
        self.file = open(input_file, 'r')
        # Current line's tokens and cursor
        self.tokens = []
        self.cursor = 0

        # Load the first non-empty line
        self.tokenizeLine()

     def is_symbol(self, token):
         return token in set(";,=![]&|()+-*<>")
     
     def getToken(self):
        if self.tokens and self.cursor < len(self.tokens):
            token = self.tokens[self.cursor]
            return token[1]
        else:
            return None


     def intValue(self) -> int:
        if self.getToken() == self.INTEGER:
            return int(self.tokens[self.cursor][0])
        else:
           print("Error: Current token is not an integer.")
           sys.exit(1)
     
        

     def idName(self) -> str:
        if self.getToken() == self.IDENTIFIER:
            return self.tokens[self.cursor][0]
        else:
            print ("Error: Current token is not an identifier.")
            sys.exit(1)

     def skipToken(self):
        if self.getToken() in [self.EOF, self.ERROR]:
            return
        else: 
            self.cursor += 1
        if self.cursor >= len(self.tokens):
            self.tokenizeLine()

     def tokenizeLine(self, line=None):
        if line is None:
            line = self.file.readline()
            while line and line.strip() == "":
                line = self.file.readline()
            if not line:
                self.tokens = [(None, self.EOF)]
                self.cursor = 0
                return self.tokens
        self.tokens = []
        self.cursor = 0
        i = 0
        current_token = ""
        while i < len(line):
            while i < len(line) and line[i].isspace():
                i += 1
            if i >= len(line):
                break
            if line[i].isalpha() and line[i].isupper():
                current_token = ""
                current_token += line[i]
                i += 1
                while i < len(line) and ((line[i].isalpha() and line[i].isupper()) or line[i].isdigit()):
                    current_token += line[i]
                    i += 1
                if current_token in self.reserved_words:
                    if i >= len(line) or line[i].isspace() or self.is_symbol(line[i]):
                        self.tokens.append((current_token, self.reserved_words[current_token]))
                    else:
                        self.tokens.append((current_token, self.ERROR))
                else:
                    if i >= len(line) or line[i].isspace() or self.is_symbol(line[i]):
                        self.tokens.append((current_token, self.IDENTIFIER))
                    else:
                        self.tokens.append((current_token, self.ERROR))
                current_token = ""
            elif line[i].isalpha() and line[i].islower():
                current_token = ""
                current_token += line[i]
                i +=1
                while i < len(line) and (line[i].isalpha() and line[i].islower()):
                    current_token += line[i]
                    i += 1
                if current_token in self.reserved_words:
                    if  i >= len(line) or line[i].isspace() or self.is_symbol(line[i]):
                        self.tokens.append((current_token, self.reserved_words[current_token]))
                    else:
                        self.tokens.append((current_token, self.ERROR))
                else:
                    self.tokens.append((current_token, self.ERROR))
                    current_token = ""
                    break
                
            elif line[i].isdigit():
                current_token = ""
                current_token += line[i]
                i +=1
                while i < len(line) and line[i].isdigit():
                    current_token += line[i]
                    i += 1
                if i >= len(line) or line[i].isspace() or self.is_symbol(line[i]):
                    self.tokens.append((current_token, self.INTEGER))
                else:
                    self.tokens.append((current_token, self.ERROR))
                current_token = ""
            else:
                if i + 1 < len(line) and line[i:i+2] in self.symbols:
                    self.tokens.append((line[i:i+2], self.symbols[line[i:i+2]]))
                    i += 2
                elif line[i] in self.symbols:
                    self.tokens.append((line[i], self.symbols[line[i]]))
                    i += 1
                else:
                    self.tokens.append((line[i], self.ERROR))
                    i += 1
                    break
        return self.tokens




def main():
    reserved_words = {"program":1, "begin":2, "end":3, "int":4, "if":5, "then":6, "else":7, "while":8, "loop":9, "read":10, "write":11}
    symbols = {";":12, ",":13, "=":14, "!":15, "[":16, "]":17, "&&":18, "||":19, "(":20, ")":21, "+":22, "-":23, "*":24, "!=":25, "==":26,
               "<":27, ">":28, "<=":29, ">=":30}
    integer=31
    identifier=32
    end_of_file=33
    error_token=34
    input_file = "test.txt"
    tokenizer = Tokenizer(input_file, reserved_words, symbols, integer, identifier, end_of_file, error_token)
    while True:
        token_int = tokenizer.getToken()
        print(token_int)
        if token_int == end_of_file or token_int == error_token:
            break
        tokenizer.skipToken()

if __name__ == "__main__":    main()
