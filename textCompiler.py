from math import pi

lines = dict()
lengths = dict()


lines[' '] = [('L', (0, 0), (0.5, 0), False)]
lengths[' '] = 0.5

lines['A'] = [('L', (0, 0), (0.25, 1), True),
              ('L', (0.25, 1), (0.5, 0), True),
              ('L', (0.25, 1), (0.125, 0.5), False),
              ('L', (0.125, 0.5), (0.375, 0.5), True),
              ('L', (0.375, 0.5), (0.5, 0), False)]
lengths['A'] = 0.5


lines['B'] = [('C', (0, 0.333), 0.333, -0.5 * pi, pi),
              ('C', (0, 0.833), 0.167, -0.5 * pi, pi),
              ('L', (0, 1), (0, 0), True),
              ('L', (0, 0), (0.33, 0), False)]
lengths['B'] = 0.33

lines['C'] = [('L', (0, 0), (0.8535534, 0.8535534), False),
              ('C', (0.5, 0.5), 0.5, 0.25 * pi, 1.5 * pi),
              ('L', (0.25, 0), (0.8535534, 0), False)]
lengths['C'] = 0.8535534

lines['D'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0), False),
              ('C', (0, 0.5), 0.5, -0.5 * pi, pi),
              ('L', (0, 1), (0.5, 0), False)]
lengths['D'] = 0.5

lines['E'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 0.5), True),
              ('L', (0.5, 0.5), (0, 0), False),
              ('L', (0, 0), (0.5, 0), True)]
lengths['E'] = 0.5

lines['F'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 0.5), True),
              ('L', (0.5, 0.5), (0.5, 0), False)]
lengths['F'] = 0.5

lines['G'] = [('L', (0, 0), (0.8535534, 0.8535534), False),
              ('C', (0.5, 0.5), 0.5, 0.25 * pi, 1.75 * pi),
              ('L', (1, 0.5), (0.5, 0.5), True),
              ('L', (0.25, 0), (1, 0), False)]
lengths['G'] = 1

lines['H'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 0.5), True),
              ('L', (0.5, 0.5), (0.5, 1), False),
              ('L', (0.5, 1), (0.5, 0), True)]
lengths['H'] = 0.5

lines['I'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0.25, 1), False),
              ('L', (0.25, 1), (0.25, 0), True),
              ('L', (0.25, 0), (0, 0), False),
              ('L', (0, 0), (0.5, 0), True)]
lengths['I'] = 0.5

lines['J'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0.25), True),
              ('L', (0.5, 0.25), (0, 0.25), False),
              ('C', (0.25, 0.25), 0.25, pi, pi),
              ('L', (0.25, 0.5), (0.5, 0), False)]
lengths['J'] = 0.5

lines['K'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 1), True),
              ('L', (0.5, 1), (0, 0.5), False),
              ('L', (0, 0.5), (0.5, 0), True)]
lengths['K'] = 0.5

lines['L'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0), False),
              ('L', (0, 0), (0.5, 0), True)]
lengths['L'] = 0.5

lines['M'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0.25, 0.5), True),
              ('L', (0.25, 0.5), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0), True)]
lengths['M'] = 0.5

lines['N'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0.5, 0), True),
              ('L', (0.5, 0), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0), False)]
lengths['N'] = 0.5

lines['O'] = [('L', (0, 0), (0, 0.5), False),
              ('C', (0.5, 0.5), 0.5, 0, 2 * pi),
              ('L', (0, 0.5), (1, 0), False)]
lengths['O'] = 1


lines['P'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0.5), False),
              ('C', (0, 0.75), 0.25, -0.5 * pi, pi),
              ('L', (0, 1), (0.167, 0.666), False),
              ('L', (0, 0.666), (0.33, 0), False),
              ('L', (0.33, 0), (0.5, 0), False)]
lengths['P'] = 0.5

lines['Q'] = [('L', (0, 0), (0, 0.5), False),
              ('C', (0.5, 0.5), 0.5, 0, 2 * pi),
              ('L', (0, 0.5), (0.5, 0.5), False),
              ('L', (0.5, 0.5), (1, 0), True)]
lengths['Q'] = 1

lines['R'] = [('L', (0, 0), (0, 1), True),
              ('L', (0, 1), (0, 0.5), False),
              ('C', (0, 0.75), 0.25, -0.5 * pi, pi),
              ('L', (0, 1), (0.167, 0.666), False),
              ('L', (0, 0.666), (0.33, 0), True),
              ('L', (0.33, 0), (0.5, 0), False)]
lengths['R'] = 0.5

# this works, don't touch, was measured in geogebra
lines['S'] = [('L', (0, 0), (0.43, 0.91), False),
              ('C', (0.25, 0.75), 0.25, 0.2304 * pi, 1.2696 * pi),
              ('L', (0.25, 0.5), (0.0364, 0.1200), False),
              ('C', (0.25, 0.25), 0.25, 1.174 * pi, 1.326 * pi),
              ('L', (0.25, 0.5), (0.5, 0), False)]
lengths['S'] = 0.5

lines['T'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0.25, 1), False),
              ('L', (0.25, 1), (0.25, 0), True),
              ('L', (0.25, 0), (0.5, 0), False)]
lengths['T'] = 0.5

lines['U'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0, 0.25), True),
              ('C', (0.25, 0.25), 0.25, pi, pi),
              ('L', (0.5, 0.25), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0), False)]
lengths['U'] = 0.5

lines['V'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.25, 0), True),
              ('L', (0.25, 0), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0), False)]
lengths['V'] = 0.5

lines['W'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.167, 0), True),
              ('L', (0.167, 0), (0.25, 0.5), True),
              ('L', (0.25, 0.5), (0.333, 0), True),
              ('L', (0.333, 0), (0.5, 1), True),
              ('L', (0.5, 1), (0.5, 0), False)]
lengths['W'] = 0.5

lines['X'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.5, 0), True),
              ('L', (0.5, 0), (0.5, 1), False),
              ('L', (0.5, 1), (0, 0), True),
              ('L', (0, 0), (0.5, 0), False)]
lengths['X'] = 0.5

lines['Y'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.25, 0.5), True),
              ('L', (0.25, 0.5), (0.5, 1), True),
              ('L', (0.5, 1), (0.25, 0.5), False),
              ('L', (0.25, 0.5), (0.25, 0), True),
              ('L', (0.25, 0), (0.5, 0), False)]
lengths['Y'] = 0.5

lines['Z'] = [('L', (0, 0), (0, 1), False),
              ('L', (0, 1), (0.5, 1), True),
              ('L', (0.5, 1), (0, 0), True),
              ('L', (0, 0), (0.5, 0), True)]
lengths['Z'] = 0.5

lines['!'] = [('L', (0, 0), (0.25, 1), False),
              ('L', (0.25, 1), (0.25, 0.3), True),
              ('L', (0.25, 0.3), (0.3, 0.15), False),
              ('C', (0.25, 0.15), 0.05, 0, 2 * pi),
              ('L', (0.3, 0.15), (0.5, 0), False)]

def getLineString(line, X, Y, height):
    if line[0] == "L":  # draws line
            return f"L:{round(X + line[1][0] * height, 4)}:{round(Y + line[1][1] * height, 4)}:{round(X + line[2][0] * height, 4)}:{round(Y + line[2][1] * height, 4)}:{'T' if line[3] else 'F'}:\n"
    elif line[0] == "C":  # draws circle
        return f"C:{round(X + line[1][0] * height, 4)}:{round(Y + line[1][1] * height, 4)}:{round(line[2] * height, 4)}:{round(line[3], 4)}:{round(line[4], 4)}:\n"



def compileWord(string, height, X, Y, path):
    with open(path, "a") as myFile:
        myFile.write(f"L:{X}:{Y}:{X}:{Y}:F:\n")  # brings robot to start
        for c in string:
            for line in lines[c]:
                # print(getLineString(line, X, Y, height))
                myFile.write(getLineString(line, X, Y, height))
            X += lengths[c] * height  # adds kerning
            myFile.write(f"L:{round(X,4)}:{round(Y,4)}:{round(X + 0.1 * height,4)}:{round(Y,4)}:F:\n")
            X += 0.1 * height
    return X

def wordLength(word, scale):
    length = 0
    for c in word:
        length += lengths[c] + 0.1
    return length * scale

def cutIntoLines(text, scale, lineLimit):
    words = text.split()
    lines = []
    currLine = []
    currLineLength = 0
    for word in words:
        if currLineLength + wordLength(word, scale) > lineLimit:
            lines.append(currLine)
            currLine = []
            currLineLength = 0
        currLine.append(word)
        currLineLength += wordLength(word, scale)
    return lines

def compileText(text, scale, X, Y, lineLengthLimit, lineAmountLimit, spacing, path):
    with open(path, 'w') as myFile:
        myFile.write("")
    lines = cutIntoLines(text, scale, lineLengthLimit)
    if len(lines) > lineAmountLimit:
        print('Error: too many lines')
        return
    for line in lines:
        print(' '.join(line))
        newX = compileWord(' '.join(line), scale, X, Y, path)
        with open(path, 'a') as myFile:
            myFile.write(f"L:{round(newX, 4)}:{round(Y, 4)}:{round(X, 4)}:{round(Y - (1 + spacing) * scale, 4)}:F:\n")
        Y -= scale * (1 + spacing)

if __name__ == '__main__':
    text = 'HELLO THIS IS A LONG TEXT TO TRY TO CUT INTO SHORTER LINES'
    print(cutIntoLines(text, 1, 8))
    #compileWord('HELLO', 1, 10, 40, 'MyTrialFile.txt') # 8, 10, 0.5,
    compileText(text, 1, 10, 40, 8, 10, 0.5, 'MyTrialFile.txt') #
