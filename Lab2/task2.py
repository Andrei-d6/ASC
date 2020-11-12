"""
    Basic thread handling exercise:

    Use the Thread class to create and run more than 10 threads which print their name and a random
    number they receive as argument. The number of threads must be received from the command line.

    e.g. Hello, I'm Thread-96 and I received the number 42

"""

from threading import Thread
from random import seed, randint

class MyTHread(Thread):
    """My thread class"""
    def __init__(self, nr, msg):
        Thread.__init__(self)
        self.nr = nr
        self.msg = msg

    def run(self):
        print("Hello, I'm Thread " + (str)(self.nr) + " and I received the number " + (str)(self.msg))

seed(1)
thread_list = []
for i in range(10):
    thread_list.append(MyTHread(i, randint(0, 100)))
    thread_list[i].start()


for i in range(10):
    thread_list[i].join()