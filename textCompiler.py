from alphabet import make_letters

lines, lengths = make_letters()


def getLineString(line, X, Y, height):
    """
    this function writes one line, that is, a single segment instruction
    (or possibly a different shape, but one command for the robot.)
    :param line: a line object. it has a shape character, ("L", "C"...), start coordinate, end coordinate,
     and whether it is visible or just to move the robot someplace (draw on the board or move robot).
    :param X: X of start coordinate
    :param Y: Y of start coordinate
    :param height: height of the text.
    :return: the string that can be written to the txt file, so that C++ can actually pass the instructions to the robot
    """
    if line[0] == "L":  # draws line
        return f"L:{round(X + line[1][0] * height, 4)}:{round(Y + line[1][1] * height, 4)}:{round(X + line[2][0] * height, 4)}:{round(Y + line[2][1] * height, 4)}:{'T' if line[3] else 'F'}:\n"
    elif line[0] == "C":  # draws circle
        return f"C:{round(X + line[1][0] * height, 4)}:{round(Y + line[1][1] * height, 4)}:{round(line[2] * height, 4)}:{round(line[3], 4)}:{round(line[4], 4)}:\n"


def compileWord(string, height, X, Y, path):
    """
    this function writes whole words, by using getLineString
    :param string: the string to write
    :param height: given height of the letters, this is so that theoretically we can even write different height lines.
    :param X: x coordinate of start
    :param Y: y coordinate of start
    :param path: path of file to write to.
    :return: the X where we end the line.
    """
    with open(path, "a") as myFile:
        myFile.write(f"L:{X}:{Y}:{X}:{Y}:F:\n")  # brings robot to start
        for character in string:
            # if 0.35<X < 0.7:
            #   X += 0.35
            for line in lines[character]:
                # print(getLineString(line, X, Y, height))
                myFile.write(getLineString(line, X, Y, height))
            X += lengths[character] * height  # adds kerning

            myFile.write(f"L:{round(X, 4)}:{round(Y, 4)}:{round(X + 0.1 * height, 4)}:{round(Y, 4)}:F:\n")
            X += 0.1 * height
    return X


def wordLength(word, scale):
    """
    given a scale and a single word, returns its length.
    :param word: the word
    :param scale: the scaling factor.
    :return: the resulting length.
    """
    length = 0
    for c in word:
        length += lengths[c] + 0.1
    return length * scale


def cutIntoLines(text, scale, lineLimit):
    """
    cuts a long text into neat short lines. since our board isn't infinite, we need to have a limit on line length.
    :param text: the text to cut.
    :param scale: the scaling factor of the text.
    :param lineLimit: the maximum amount of lines, because our board is also limited in the X direction.
    :return: a list of the resulting lines.
    """
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
    if currLine:
        lines.append(currLine)
    return lines


def compileText(text, scale, X, Y, lineLengthLimit, lineAmountLimit, spacing, path):
    """
    this function bundles everything up, the main function calls it directly.
    :param text: the text to write.
    :param scale: scaling factor of the text.
    :param X: starting X coordinate
    :param Y: starting Y coordinate
    :param lineLengthLimit: limit on the length of the lines, board isnt infinite in Y direction.
    :param lineAmountLimit: limit on the amount of lines, board isn't infinite in X direction.
    :param spacing: the amount of spacing between letters.
    :param path: path of the file to write to, to pass to C++.
    :return: None
    """
    with open(path, 'w') as myFile:
        myFile.write("")
    lines = cutIntoLines(text, scale, lineLengthLimit)
    if len(lines) > lineAmountLimit:
        print('Error: too many lines')
        return
    for line in lines:
        # print(' '.join(line))
        newX = compileWord(' '.join(line), scale, X, Y, path)
        with open(path, 'a') as myFile:
            myFile.write(f"L:{round(newX, 4)}:{round(Y, 4)}:{round(X, 4)}:{round(Y - (1 + spacing) * scale, 4)}:F:\n")
        Y -= scale * (1 + spacing)


if __name__ == '__main__':
    text = 'HELLO FROM THE OTHER SIDE I MUST HAVE CALLED A THOUSAND TIMES'
    print(cutIntoLines(text, 0.05, 0.4))
    # compileWord('HELLO', 1, 10, 40, 'MyTrialFile.txt') # 8, 10, 0.5,
    compileText(text, scale=0.05, X=-0.05, Y=0.23, lineLengthLimit=0.4, lineAmountLimit=10, spacing=0.01,
                path='hello.txt')  #
