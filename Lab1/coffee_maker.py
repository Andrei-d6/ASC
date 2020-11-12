"""
A command-line controlled coffee maker.
"""

import sys

# import antigravity
"""
Implement the coffee maker's commands. Interact with the user via stdin and print to stdout.

Requirements:
    - use functions
    - use __main__ code block
    - access and modify dicts and/or lists
    - use at least once some string formatting (e.g. functions such as strip(), lower(),
    format()) and types of printing (e.g. "%s %s" % tuple(["a", "b"]) prints "a b"
    - BONUS: read the coffee recipes from a file, put the file-handling code in another module
    and import it (see the recipes/ folder)

There's a section in the lab with syntax and examples for each requirement.

Feel free to define more commands, other coffee types, more resources if you'd like and have time.
"""

"""
Tips:
*  Start by showing a message to the user to enter a command, remove our initial messages
*  Keep types of available coffees in a data structure such as a list or dict
e.g. a dict with coffee name as a key and another dict with resource mappings (resource:percent)
as value
"""

# Commands
EXIT = "exit"
LIST_COFFEES = "list"
MAKE_COFFEE = "make"  # !!! when making coffee you must first check that you have enough resources!
HELP = "help"
REFILL = "refill"
RESOURCE_STATUS = "status"
commands = [EXIT, LIST_COFFEES, MAKE_COFFEE, REFILL, RESOURCE_STATUS, HELP]

# Coffee examples
ESPRESSO = "espresso"
AMERICANO = "americano"
CAPPUCCINO = "cappuccino"
COFFES = [ESPRESSO, AMERICANO, CAPPUCCINO]

# Resources examples
WATER = "water"
COFFEE = "coffee"
MILK = "milk"

# Coffee maker's resources - the values represent the fill percents
RESOURCES = {WATER: 100, COFFEE: 100, MILK: 100}

"""
Example result/interactions:

I'm a smart coffee maker
Enter command:
list
americano, cappuccino, espresso
Enter command:
status
water: 100%
coffee: 100%
milk: 100%
Enter command:
make
Which coffee?
espresso
Here's your espresso!
Enter command:
refill
Which resource? Type 'all' for refilling everything
water
water: 100%
coffee: 90%
milk: 100%
Enter command:
exit
"""


# print("I'm a simple coffee maker")
# print("Press enter")
# sys.stdin.readline()
# print("Teach me how to make coffee...please...")

def status():
    """
    Prints the current status
    """
    for key in RESOURCES:
        # print(key + ": " + (str)(RESOURCES[key]) + "%")
        print("%s: %d%%" % (key, RESOURCES[key]))


RECIPES = {}


def read_file(filename):
    """
    Reads the conents of a file
    """
    with open(filename, 'r') as my_file:
        for line in my_file:
            if "=" in line:
                if "water" in line:
                    water_value = line[line.find("=") + 1:]
                    water_value = water_value[:-1]
                if "coffee" in line:
                    coffee_value = line[line.find("=") + 1:]
                    coffee_value = coffee_value[:-1]
                if "milk" in line:
                    milk_value = line[line.find("=") + 1:]
                    milk_value = milk_value[:-1]
    return {WATER: water_value, COFFEE: coffee_value, MILK: milk_value}


def read_recipes():
    """
    Reads the recipes for coffee
    """
    for a_coffee in COFFES:
        RECIPES[a_coffee] = read_file("recipes/" + a_coffee + ".txt")


if __name__ == "__main__":
    command = "start"
    read_recipes()
    # print(RECIPES)
    while command[:len(command) - 1] != "exit":
        print("Enter command:")
        command = sys.stdin.readline()

        if command[:len(command) - 1] == "list":
            print(COFFES)
        elif command[:len(command) - 1] == "status":
            status()
        elif command[:len(command) - 1] == "make":
            print("Which coffee")
            coffee = sys.stdin.readline()
            coffee = coffee[:-1]
            if coffee == "espresso" or coffee == "americano" or coffee == "cappuccino":
                water = (int)(RECIPES[coffee][WATER])
                milk = (int)(RECIPES[coffee][MILK])
                coffee_amount = (int)(RECIPES[coffee][COFFEE])

                if RESOURCES[WATER] >= water and RESOURCES[COFFEE] >= coffee_amount and RESOURCES[MILK] >= milk:
                    RESOURCES[WATER] -= water
                    RESOURCES[COFFEE] -= coffee_amount
                    RESOURCES[MILK] -= milk
                    print("Here's your " + coffee + "!")
                else:
                    print("Can't make " + coffee)
            else:
                print("Wrong type of coffee")
        elif command[:len(command) - 1] == "refill":
            print("Which resource? Type 'all' for refilling everything")
            resource = sys.stdin.readline()

            if resource[:len(resource) - 1] == "all":
                for KEY in RESOURCES:
                    RESOURCES[KEY] = 100
                status()
            elif resource[:len(resource) - 1].lower() == "water":
                RESOURCES[WATER] = 100
                status()
            elif resource[:len(resource) - 1].lower() == "coffee":
                RESOURCES[COFFEE] = 100
                status()
            elif resource[:len(resource) - 1].lower() == "milk":
                RESOURCES[MILK] = 100
                status()
            else:
                print("Invalid resource")
