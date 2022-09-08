import sys
import os
import tempfile
from textCompiler import compileText

docstring = '''
	NAME
		roboard_script
	SYNOPSIS
		python roboard_script [-s|-f|-c write_string] [-lc|-sc callibration_path] [-v|-vv|-VV] [OPTIONS]...
	
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

		-lc callibration_path
			if you have a file containing a previous relevant callibration (for example, after using the -s option)
			you can skip the callibration phase and just use the measurments from that file.

		-sc callibration_path
			if you wish to store the callibration measurements for later use, use this flag with the desired path
		
		if none of lc|sc are used, the callibration phase will not be skipped, but the measurements will not be saved anywhere.
		
		-v|-vv|-VV
			verbose options

		--font-size h
			set maximal height of letters to h (in centimeters)

		--line-length-limit w
			set the maximum length of a line to w (in centimeters) 
			if a word has length greater than w, an exception wil be thrown
		
		--letter-spacing spacing
			set the spacing between letters to be spacing (as a percentage of the height of the letters)

		--line-spacing spacing
			set the line spacing to be spacing (as a percentage of the height of the letters)

		-percision p
			set the amount of points along every curve to p. this essentially trades speed for percision.

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

		-d
			debug option. runs normally but prints the command line to roboard instead of running it

	EXAMPLES
		Roboard -s "HELLO WORLD!"
		Roboard -s "Hi, my name is Roaboard. I can write anything on the board :)" -sc "roboard_callibration.txt"
		Roboard -f "write_text.txt" -lc "roboard_callibration.txt" --font-size 10 --line-length-limit 30
	
	IMPORTANT
		this script assumes that you follow the correc foramt. any deveation will cause unexpected behavoir
	'''

######################
###### DEFAULTS ######
######################

DEFAULT_FONT_HEIGHT = 7
DEFAULT_LINE_SPACING = 0.4
DEFAULT_LETTER_SPACING = 0.2
DEFAULT_FIRST_LINE_X = -5
DEFAULT_FIRST_LINE_Y = 30
DEFAULT_LINE_LENGTH_LIMIT = 50
DEFAULT_LINE_AMOUNT_LIMIT = 4
DEFAULT_PERCISION = 100
DEFAULT_CALLIBRATION_OPTION, STORE_CALLIBRATION_OPTION, LOAD_CALLIBRATION_OPTION = range(3)

def usage(error_string):
	print(error_string + "\n\n")
	print(docstring)

def main(argv):
	
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
	if sum(("-s" in argv, "-f" in argv, "-c" in argv)) != 1:
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
	if "-lc" in argv and "-sc" in argv:
		usage("you may use at most one of -lc|-sc.")
		return -1

	if not ("-lc" in argv or "-sc" in argv):
		callibration_option = DEFAULT_CALLIBRATION_OPTION
	else:
		callibration_option = STORE_CALLIBRATION_OPTION if "-sc" in argv else LOAD_CALLIBRATION_OPTION
	callibration_path = get_operand("-lc") or get_operand("-sc")
	# set rest of parameteres according to the command line
	font_height = get_operand("-h", default=DEFAULT_FONT_HEIGHT, convert=float)
	line_spacing = get_operand("--line-spacing", default=DEFAULT_LINE_SPACING, convert=float)
	letter_spacing = get_operand("--letter-spacing", default=DEFAULT_LETTER_SPACING, convert=float)
	first_line_x = get_operand("--first-line-X", default=DEFAULT_FIRST_LINE_X, convert=float)
	first_line_y = get_operand("--first-line-Y", default=DEFAULT_FIRST_LINE_Y, convert=float)
	line_length_limit = get_operand("--line-length-limit", default=DEFAULT_LINE_LENGTH_LIMIT, convert=float)
	line_amount_limit = get_operand("--line-amount-limit", default=DEFAULT_LINE_AMOUNT_LIMIT, convert=int)
	percision = get_operand("-percision", default=DEFAULT_PERCISION, convert=float)
	
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
			fd, compilation_path = tempfile.mkstemp()
		compileText(write_string, font_height/100.0, first_line_x/100.0, first_line_y/100.0,\
					line_length_limit/100.0, line_amount_limit, letter_spacing, line_spacing, compilation_path)
	

	##############################################
	############ robot movement phase ############
	##############################################

	#generatin the exection string
	## callibration
	if callibration_option == LOAD_CALLIBRATION_OPTION:
		callibration_option_string = f'-l "{callibration_path}"'
	elif callibration_option == STORE_CALLIBRATION_OPTION:
		callibration_option_string = f'-s "{callibration_path}"'
	else:
		callibration_option_string = ''

	## verbose options
	verbose_string = '-VV' if '-VV' in argv else\
					 '-vv' if '-vv' in argv else\
					 '-v' if '-v' in argv else ''

	# use Roboard.cpp to move robot
    operation = print if "-d" in argv else os.system
	operation(f'Roboard.exe -f "{compilation_path}" {callibration_option_string} -n {percision} {verbose_string} -starting-point 0.2445 0.1996 0.6027'+\
		  (f' -c-points 0.5750 0.1682 0.3920 0.5750 0.1682 0.6920 0.2450 0.1682 0.3920' if callibration_option in [STORE_CALLIBRATION_OPTION, DEFAULT_CALLIBRATION_OPTION] else ''))

	if not skip_compile and not "--save-compilation" in argv:
		os.close(fd)
		# os.unlink(compilation_path)


if __name__ == "__main__":
	main(sys.argv)
