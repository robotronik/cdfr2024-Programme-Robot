import sys
import os
def print_color(color):
    if color == "YELLOW":
        print("Yellow color")
    elif color == "BLUE":
        print("Blue color")
    else:
        print("Unknown color")
def trouver_fichier(start_path, filename):
    for root, dirs, files in os.walk(start_path):
        if filename in files:
            return os.path.join(root, filename)
    return None

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python program.py <color>")
        sys.exit(1)

    color = sys.argv[1]
    print_color(color)
    resultat=trouver_fichier(os.getcwd(),"UDPServer.py")
    print(resultat)
    os.system("python3 "+str(resultat)+(" 0" if color == "BLUE" else " 1"))
