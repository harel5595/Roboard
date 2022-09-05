#!/bin/python3
import sys
import os
from textCompiler import compileText


DEFAULT_FONT_HEIGHT = 10
DEFAULT_LINE_SPACING = 0.5
DEFAULT_FIRST_LINE_X = 0
DEFAULT_FIRST_LINE_Y = 30
DEFAULT_LINE_LENGTH_LIMIT = 40
DEFAULT_LINE_AMOUNT_LIMIT = 4
DEFAULT_CALLIBRATION_OPTION, STORE_CALLIBRATION_OPTION, LOAD_CALLIBRATION_OPTION = range(2)

def usage(error_string):
	print(error_string + "\n\n")
	print("usage!")

def main(argv):
	'''
	NAME
		Roboard
	SYNOPSIS
		Roboard [-s|f write_string] [-lc|sc callibration_path] [OPTIONS]...
	
	DESCRIPTION
		This function incapsulates the functionality of the Roboard (TM) module. given a 
		write_string, it sends the appropriate commands to the robot, and writes that text 
		the board. There are several callibration options and text-editing options.
	
	OPTIONS
		-s write_string
			the string you wish to write on the board
		
		-f write_string
			the path to a text file containg the string you wish to write on the board
		
		-c compilation_path
			the path to a complied text file of robot command lines

		--save-compliation compilation_path
			use this flag if you want to save the compilation product to a file at compilation_path. the
			intended use is without the -c option

		-lc callibartion_path
			if you have a file containing a previous relevant callibration (for example, after using the -s option)
			you can skip the callibration phase and just use the measurments from that file.

		-sc callibration_path
			if you wish to store the callibration measurements for later use, use this flag with the desired path
		
		if none of lc|sc are used, the callibration phase will not be skipped, but the measurements will not be saved anywhere.
		
		--font-size h
			set maximal height of letters to h (in centimeters)

		--line-length-limit w
			set the maximum length of a line to w (in centimeters) 
			if a word has length greater than w, an exception wil be thrown
		
		--line-spacing spacing
			set the line spacing to be spacing (as a percentage of the height of the letters)

		--line-amount-limit L
			set the maximum amount of lines to L. this doesn't actually alter the output, but if your text doesn't
			comply with this restriction, an exception will be thrown and the robot will not perform any commands.
		
		--first-line-X x
			sets the X coordinates of the bottom left point of the first letter in the first line to x.

		--first-line-Y y
			sets the Y coordinates of the bottom left point of the first letter in the first line to y.

		both first-line-X and first-line-Y use coordinates in the plane detirmined at callibration. the 
		unit length is 1 centimeter and the orgin is at the bottom left measurment. if the callibraion 
		is done propely the x axis is parallel to the ground and point to the right, and the y axis is 
		perpendicular to it and points upwards.

	EXAMPLES
		Roboard -s "HELLO WORLD!"
		Roboard -s "Hi, my name is Roaboard. I can write anything on the board :)" -sc "roboard_callibration.txt"
		Roboard -f "write_text.txt" -lc "roboard_callibration.txt" --font-size 10 --line-length-limit 0.3
	'''

	def get_operand(flag, default=None, convert=None):
		'''
		this function returns the operand of a flag. if this couldn't retreive the operand, it returns
		the default value.
		'''
		try:
			return convert(argv[argv.index(flag)+1]) if convert else argv[argv.index(flag)+1]
		except:
			return default

	##############################################
	############# parse command line #############
	##############################################
	
	# set write string:
	if sum("-s" in argv, "-f" in argv, "-c" in argv) != 1:
		usage("you must use exactly one of -s|-f|-c.")
		return -1
	if "-s" in argv:
		skip_compile = False
		write_string = get_operand("-s")
		if not write_string:
			usage("must provide write_string")
			return -1
	if "-f" in argv:
		skip_compile = False
		if not (file_path := get_operand("-f")):
			usage("must provide write_string file path")
			return -1
		with open(file_path, "r") as my_file:
			write_string = my_file.read()
	if "-c" in argv:
		skip_compile = True
		compilation_path = get_operand("-c")
		if not compilation_path:
			usage("must provide compilation file path")
			return -1


	# set callibration option
	if not ("-lc" in argv or "-sc" in argv):
		callibration_option = DEFAULT_CALLIBRATION_OPTION
	if "-lc" in argv and "-sc" in argv:
		usage("you may use at most one of -lc|-sc.")
		return -1
	callibration_option = STORE_CALLIBRATION_OPTION if "-sc" in argv else LOAD_CALLIBRATION_OPTION

	# set rest of parameteres according to the command line
	font_height = get_operand("-h", default=DEFAULT_FONT_HEIGHT, convert=float)
	line_spacing = get_operand("--line-spacing", default=DEFAULT_LINE_SPACING, convert=float)
	first_line_x = get_operand("--first-line-X", default=DEFAULT_FIRST_LINE_X, convert=float)
	first_line_y = get_operand("--first-line-Y", default=DEFAULT_FIRST_LINE_Y, convert=float)
	line_length_limit = get_operand("--line-length-limit", default=DEFAULT_LINE_LENGTH_LIMIT, convert=float)
	line_amount_limit = get_operand("--line-amount-limit", default=DEFAULT_LINE_AMOUNT_LIMIT, convert=int)
	
	
	##############################################
	############# compile text phase #############
	##############################################
	if not skip_compile:
		if "--save-compilation" in argv:
			compilation_path = get_operand("--save-compilation")
			if not compilation_path:
				usage("must provide a compilation path to save compilation output to")
				return -1
		else:
			fd, compilation_path = mkstmp()
		compileText(write_string, font_height, first_line_x, first_line_y,\
					line_length_limit, line_amount_limit, line_spacing, compilation_path)
	

	##############################################
	############ robot movement phase ############
	##############################################

	# use roboard.cpp to move robot

	if not "--save-compilation" in argv:
		os.unlink(fd)


if __name__ == "__main__":
	main(sys.argv)
