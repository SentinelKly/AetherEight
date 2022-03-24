import sys
import os.path

class Object:
    src_file  = ""
    out_file = ""
    src = ""
    bin = []

    def __init__(self):
        pass
        
final_obj = Object()

#Doesnt Work

#def data_into_buffer(file):
#    with open(file, "r") as bin_file:
#        return bin_file.read()

def print_help():
    print("Usage: ae_asm.py <source file> -o <output file>")
    exit()

def get_command_args(args):
    mode = 0
    for i in args:
        if i == "-o":
            mode = 1
        elif i == "-h":
            print_help()
        elif mode == 0:
            final_obj.src_file = i
        elif mode == 1:
            final_obj.out_file = i
            mode = 0
        else:
            print_help()

def main():
    get_command_args(sys.argv)
    if os.path.isfile(final_obj.src_file) == False:
        print("Error: File not found")
        exit()

    #Doesnt Work

    #final_obj.src = data_into_buffer(final_obj.src_file)
    #print(final_obj.src)

main()