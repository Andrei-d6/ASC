"""
Coffee Factory: A multiple producer - multiple consumer approach

Generate a base class Coffee which knows only the coffee name
Create the Espresso, Americano and Cappuccino classes which inherit the base class knowing that
each coffee type has a predetermined size.
Each of these classes have a get message method

Create 3 additional classes as following:
    * Distributor - A shared space where the producers puts coffees and the consumers takes them
    * CoffeeFactory - An infinite loop, which always sends coffees to the distributor
    * User - Another infinite loop, which always takes coffees from the distributor

The scope of this exercise is to correctly use threads, classes and synchronization objects.
The size of the coffee (ex. small, medium, large) is chosen randomly everytime.
The coffee type is chosen randomly everytime.

Example of output:

Consumer 65 consumed espresso
Factory 7 produced a nice small espresso
Consumer 87 consumed cappuccino
Factory 9 produced an italian medium cappuccino
Consumer 90 consumed americano
Consumer 84 consumed espresso
Factory 8 produced a strong medium americano
Consumer 135 consumed cappuccino
Consumer 94 consumed americano
"""
from random import seed, randint
from threading import Semaphore, Thread, Lock

BUFFSIZE = 3
STOP = False
empty = Semaphore(value = 0)
full = Semaphore(value = BUFFSIZE)
producerLock = Lock()
consumerLock = Lock()

def get_size():
    coffee_size = randint(0,3)
    if coffee_size == 0:
        return 'small'
    elif coffee_size == 1:
        return 'medium'
    else:
        return 'large'

def get_coffee_name():
    name = randint(0,3)
    if name == 0:
        return 'americano'
    elif name == 1:
        return 'cappuccino'
    else:
        return 'espresso'

class Coffee:
    """ Base class """
    def __init__(self):
        pass

    def get_name(self):
        """ Returns the coffee name """
        return self.name

    def get_size(self):
        """ Returns the coffee size """
        return self.size


class ExampleCoffee:
    """ Espresso implementation """
    def __init__(self, size):
        pass

    def get_message(self):
        """ Output message """
        raise NotImplementedError

class Espresso(Coffee):
    """ Espresso implementation """
    def __init__(self, size):
        self.size = size
        self.name = 'Espresso'

    def get_message(self):
        """ Output message """
        print("Here is your " + str(self.size) + " " + self.name)

class Americano(Coffee):
    """ Espresso implementation """
    def __init__(self, size):
        self.size = size
        self.name = 'Americano'

    def get_message(self):
        """ Output message """
        print("Here is your " + str(self.size) + " " +  self.name)


class Cappucino(Coffee):
    """ Espresso implementation """
    def __init__(self, size):
        self.size = size
        self.name = 'Cappucino'

    def get_message(self):
        """ Output message """
        print("Here is your " + str(self.size) + " " +  self.name)

class Distributor():
    buff = []

    def __init__(self):
        self.consumed = 0

    def produce(self, coffee):
        self.buff.append(coffee)

    def consume(self):
        self.buff[0].get_message()
        self.consumed += 1
        if self.consumed > 1000:
            global STOP
            STOP = True
        self.buff.pop(0)


class CoffeeFactory(Thread):
    def __init__(self, id):
        Thread.__init__(self)
        self.id = id

    def run(self):
        while not STOP:
            full.acquire()
            producerLock.acquire()
            coffee_name = get_coffee_name()
            if coffee_name == 'espresso':
                coffee = Espresso(get_size())
            elif coffee_name == 'americano':
                coffee = Americano(get_size())
            else:
                coffee = Cappucino(get_size())
            buffer.produce(coffee)
            producerLock.release()
            empty.release()

class User(Thread):
    def __init__(self, id):
        Thread.__init__(self)
        self.id = id

    def run(self):
        while not STOP:
            empty.acquire()
            consumerLock.acquire()
            buffer.consume()
            consumerLock.release()
            full.release()


if __name__ == '__main__':
    seed(1)
    buffer = Distributor()
    producer_list = []
    nr_producers = 2
    for i in range(nr_producers):
        producer_list.append(CoffeeFactory(i))
        producer_list[i].start()

    consumer_list = []
    nr_consumers = 3
    for i in range(nr_consumers):
        consumer_list.append(User(i))
        consumer_list[i].start()

    for i in range(nr_producers):
        producer_list[i].join()

    for i in range(nr_consumers):
        consumer_list[i].join()