from threading import Thread, Lock
import time
import random

class Philosopher(Thread):
    def __init__(self, name, leftFork, rightFork):
        Thread.__init__(self)
        self.name = name
        self.leftFork = leftFork
        self.rightFork = rightFork

    def run(self):
        if self.name == 'Philosopher1':
            self.rightFork.acquire()
            time.sleep(random.uniform(1,3))
            self.leftFork.acquire()
            print(self.name + " is eating")
            self.leftFork.release()
            self.rightFork.release()
        else:
            self.leftFork.acquire()
            time.sleep(random.uniform(1, 3))
            self.rightFork.acquire()
            print(self.name + " is eating")
            self.rightFork.release()
            self.leftFork.release()

if __name__ == '__main__':
    philosophers = []
    for i in range(5):
        philosophers.append(Philosopher(("Philosopher" + str(i)), Lock(), Lock()))
        philosophers[i].start()

    for i in range(5):
        philosophers[i].join()