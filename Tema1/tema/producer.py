"""
This module represents the Producer.

Computer Systems Architecture Course
Assignment 1
March 2020
"""

from threading import Thread
from time import sleep

class Producer(Thread):
    """
    Class that represents a producer.
    """

    def __init__(self, products, marketplace, republish_wait_time, **kwargs):
        """
        Constructor.

        @type products: List()
        @param products: a list of products that the producer will produce

        @type marketplace: Marketplace
        @param marketplace: a reference to the marketplace

        @type republish_wait_time: Time
        @param republish_wait_time: the number of seconds that a producer must
        wait until the marketplace becomes available

        @type kwargs:
        @param kwargs: other arguments that are passed to the Thread's __init__()


        Other attributes
        type producer_id: Int
        param producer_id: the id of the producer
        """
        Thread.__init__(self, **kwargs)
        self.products = products
        self.marketplace = marketplace
        self.republish_wait_time = republish_wait_time
        self.producer_id = -1 # initial dummy value

    def run(self):
        # register the producer (get his id)
        self.producer_id = self.marketplace.register_producer()

        while 1:
            for product, quantity, produce_time in  self.products:
                # the amount that has been produced for the current product
                produced_quantity = 0
                # semaphore for checking if the current product has been produced but not published
                produced = False

                while produced_quantity < quantity:
                    # the semaphore is acquired here so that the producer
                    # can wait the production time
                    self.marketplace.producers_locks[self.producer_id].acquire()
                    if not produced:
                        sleep(produce_time)
                    published = self.marketplace.publish(self.producer_id, product)
                    self.marketplace.producers_locks[self.producer_id].release()

                    if published:
                        # the product has benn published, we can produce the next product
                        produced_quantity += 1
                        produced = False
                    else:
                        # the product has been produced, but not published
                        produced = True
                        sleep(self.republish_wait_time)
