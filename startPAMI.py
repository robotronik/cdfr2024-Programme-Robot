import sys

def print_color(color):
    if color == "YELLOW":
        print("Yellow color")
    elif color == "BLUE":
        print("Blue color")
    else:
        print("Unknown color")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python program.py <color>")
        sys.exit(1)

    color = sys.argv[1]
    print_color(color)