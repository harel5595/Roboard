from alphabet import make_letters
import sys
lines, lengths = make_letters()


def getLineString(line, X, Y, height):
    """
    translates between line tuples (such as they appear in the alphabet file) to line strings which adhere to the format required by Roboard.exe
    (or possibly a different shape, but one command for the robot.)
    :param line: a line tuple. it has a shape character, ("L", "C"...), start coordinate, end coordinate,
     and whether it is visible or just to move the robot someplace (draw on the board or move robot).
    :param X: X of start coordinate
    :param Y: Y of start coordinate
    :param height: height of the text. (which is just the scale factor, since all characters are written with "hight 1")
    :return: the string that can be written to the txt file, so that C++ can actually pass the instructions to the robot
    """
    if line[0] == "L":  # draws line
        return f"L:{round(X + line[1][0] * height, 4)}:{round(Y + line[1][1] * height, 4)}:{round(X + line[2][0] * height, 4)}:{round(Y + line[2][1] * height, 4)}:{'T' if line[3] else 'F'}:\n"
    elif line[0] == "C":  # draws circle
        return f"C:{round(X + line[1][0] * height, 4)}:{round(Y + line[1][1] * height, 4)}:{round(line[2] * height, 4)}:{round(line[3], 4)}:{round(line[4], 4)}:\n"


def compileWord(string, height, X, Y, word_spacing, path):
    """
    this function compiles whole words, by using getLineString
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
            for line in lines[character]:
                myFile.write(getLineString(line, X, Y, height))
            X += lengths[character] * height  # adds kerning

            myFile.write(f"L:{round(X, 4)}:{round(Y, 4)}:{round(X + word_spacing * height, 4)}:{round(Y, 4)}:F:\n") # this adds kerning between letters
            X += word_spacing * height
    return X


def wordLength(word, word_spacing, scale):
    """
    given a scale factor and a single word, returns its length.
    :param word: the word
    :param scale: the scaling factor.
    :return: the resulting length.
    """
    length = 0
    for c in word:
        length += lengths[c] + word_spacing
    return length * scale


def cutIntoLines(text, scale, word_spacing, lineLimit):
    """
    cuts a long text into neat short lines. since our board isn't infinite, we need to have a limit on line length.
    :param text: the text to cut.
    :param scale: the scaling factor of the text.
    :param word_spacing: spacing between letters
    :param lineLimit: the maximum amount of lines, because our board is also limited in the Y direction.
    :return: a list of the resulting lines.
    """
    words = text.split()
    lines = []
    currLine = []
    currLineLength = 0
    for word in words:
        if currLineLength + wordLength(word, scale, word_spacing) > lineLimit: # if with a given word the line exceeds the desired length, start a new line
            lines.append(currLine)
            currLine = []
            currLineLength = 0
        currLine.append(word)
        currLineLength += wordLength(word, scale, word_spacing) + (scale * lengths[' '])
    if currLine: # add last line to the line list
        lines.append(currLine)
    if not lines[0]: # bug fix for when the first line is empty, if the first word is longer than the line length limit
        lines = lines[1:]
    return lines


def compileText(text, scale, X, Y, lineLengthLimit, lineAmountLimit, word_spacing, spacing, path):
    """
    this function bundles everything up, the main function calls it directly.
    :param text: the text to write.
    :param scale: scaling factor of the text.
    :param X: starting X coordinate
    :param Y: starting Y coordinate
    :param lineLengthLimit: limit on the length of the lines, board isnt infinite in Y direction.
    :param lineAmountLimit: limit on the amount of lines, board isn't infinite in X direction.
    :param word_spacing: the amount of spacing between letters.
    :param spacing: the amount of spacing between lines of text.
    :param path: path of the file to write to, to pass to C++.
    :return: None
    """
    with open(path, 'w') as myFile:
        myFile.write("")
    lines = cutIntoLines(text, scale, word_spacing, lineLengthLimit)
    if len(lines) > lineAmountLimit:
        print('Error: too many lines')
        return
    for line in lines:
        newX = compileWord(' '.join(line), scale, X, Y, word_spacing, path)
        with open(path, 'a') as myFile:
            myFile.write(f"L:{round(newX, 4)}:{round(Y, 4)}:{round(X, 4)}:{round(Y - (1 + spacing) * scale, 4)}:F:\n")
        Y -= scale * (1 + spacing)



def main(argv):
    docstring = '''
    NAME
        textCompiler
    
    SYNOPSIS
        python textCompiler.py write_string text_height X Y line_length_limit line_amount word_spacing spacing compile_path

    DESCRIPTION
        this script compiles text accordind to the given parameters

    ARGUMENTS
        write_string
            string of text to compile

        text_height
            height of text on the board (in csntimeters)

        X, Y
            coordinates of starting point of first line (bottom left)

        line_length_limit
            bound of length (in centimeters) of the lines. for some reason this isn't accurate. it still cuts lines according to the number but it is not exactly in centimeters.
            due to time shortage we were not able to denug this in time for submission

        line_amount
            the bound on the total amount of lines. this will still compile but will let you know if you passed the amount of lines
        
        word_spacing
            spacing between letters (in centimeters)

        spacing
            spacing between lines

        compile_path
            path to write compiled file to. this will overwrite anything in that path

    '''
    if argv[2] == "-h":
        print(docstring)
        return
    try:
        compile_string = argv[1]
        height = float(argv[2]) / 100.0
        X = float(argv[3]) / 100.0
        Y = float(argv[4]) / 100.0
        line_length_limit = float(argv[5]) /100.0
        line_amount = int(argv[6])
        word_spacing = float(argv[7]) / 100.0
        spacing = float(argv[8]) / 100.0
        compile_path = argv[9]
        compileText(compile_string, height, X, Y, line_length_limit, line_amount, word_spacing, spacing, compile_path)
        return 0
    except:
        print("usage: python textCompiler write_string text_height X Y line_length_limit line_amount word_spacing spacing compile_path\n\n" + \
              "all measurments are done in centimeters\ntype python textCompiler '' -h for detailed guide")
if __name__ == '__main__':
    main(sys.argv)
