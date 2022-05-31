from math import pi

lines = dict()

lines[' '] = [('L', (0, 0), (0.5, 0), False)]

lines['A'] = [('L', (0, 0), (0.25, 1), True),
              ('L', (0.25, 1), (0.5, 0), True),
              ('L', (0.25, 1), (0.125, 0.5), False),
              ('L', (0.125, 0.5), (0.375, 0.5), True),
              ('L', (0.375, 0.5), (0.5, 0), False)]

lines['H'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 0.5), True),
              ('L', (0.5, 0.5), (0.5, 1), False),
              ('L', (0.5, 1), (0.5, 0), True)]

lines['E'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 0.5), True),
              ('L', (0.5, 0.5), (0, 0), False),
              ('L', (0, 0), (0.5, 0), True)]

lines['L'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0), False),
              ('L', (0, 0), (0.5, 0), True)]

lines['O'] = [('L', (0, 0), (0.5, 0.25), False),
              ('C', (0.25, 0.25), 0.25, 0, 2 * pi),
              ('L', (0.5, 0.25), (0.5, 0), False)]

lines['W'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.167, 0), True),
              ('L', (0.167, 0), (0.25, 0.5), True),
              ('L', (0.25, 0.5), (0.333, 0), True),
              ('L', (0.333, 0), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0), False)]

lines['R'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0.666), False),
              ('C', (0, 0.833), 0.167, -0.5 * pi, pi),
              ('L', (0, 1), (0, 0.666), False),
              ('L', (0, 0.666), (0.33, 0), True),
              ('L', (0.33, 0), (0.5, 0), False)]

lines['D'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0), False),
              ('C', (0, 0.5), 0.5, -0.5 * pi, pi),
              ('L', (0, 1), (0.5, 0), False)]

lines['!'] = [('L', (0, 0), (0.25, 1), False),
              ('L', (0.25, 1), (0.25, 0.3), True),
              ('L', (0.25, 0.3), (0.3, 0.15), False),
              ('C', (0.25, 0.15), 0.05, 0, 2 * pi),
              ('L', (0.3, 0.15), (0.5, 0), False)]


def compileWord(string, height, X, Y, path):
    with open(path, "w") as myFile:
        myFile.write(f"L:{X}:{Y}:{X}:{Y}:F:\n")
        for c in string:
            for line in lines[c]:
                if line[0] == "L":
                    myFile.write(f"L:{X + line[1][0]*height}:{Y + line[1][1]*height}:{X + line[2][0]*height}:{Y + line[2][1]*height}:{'T' if line[3] else 'F'}:\n")
                elif line[0] == "C":
                    myFile.write(f"C:{X + line[1][0]*height}:{Y + line[1][1]*height}:{line[2] * height}:{line[3]}:{line[4]}:\n")
            X += 0.5*height
            myFile.write(f"L:{X}:{Y}:{X+0.1*height}:{Y}:F:\n")
            X += 0.1*height
if __name__ == '__main__':
    compileWord("HELLO WORLD!", 0.1,1.0,0.1,"Hello.txt")