"""
This module represents the Consumer.

Computer Systems Architecture Course
Assignment 1
March 2020
"""

from threading import Thread
from time import sleep


class Consumer(Thread):
    """
    Class that represents a consumer.
    """

    def __init__(self, carts, marketplace, retry_wait_time, **kwargs):
        """
        Constructor.

        :type carts: List
        :param carts: a list of add and remove operations

        :type marketplace: Marketplace
        :param marketplace: a reference to the marketplace

        :type retry_wait_time: Time
        :param retry_wait_time: the number of seconds that a producer must wait
        until the Marketplace becomes available

        :type kwargs:
        :param kwargs: other arguments that are passed to the Thread's __init__()

        Other attributes
        type cart_id: Int
        param cart_id: the id of the consumer (a consumer is identified by it's cart)

        type name: Str
        param name: the name of the consumer
        """
        Thread.__init__(self, **kwargs)
        self.carts = carts
        self.marketplace = marketplace
        self.retry_wait_time = retry_wait_time
        self.cart_id = -1   # initial dummy value
        self.name = kwargs['name']


    def run(self):
        # register the consumer (get his id)
        self.cart_id = self.marketplace.new_cart()

        # a consumer may add products to the cart or remove items from the cart
        # or place orders

        for cart in self.carts:
            for action in cart:
                # add a product 'quantity' times to the cart
                if action['type'] == 'add':
                    quantity = 0
                    while quantity < action['quantity']:
                        added = self.marketplace.add_to_cart(self.cart_id, action['product'])

                        if added:
                            quantity += 1
                        else:
                            sleep(self.retry_wait_time)

                # remove a product 'quantity' time from the cart
                elif action['type'] == 'remove':
                    quantity = 0
                    while quantity < action['quantity']:
                        self.marketplace.remove_from_cart(self.cart_id, action['product'])
                        quantity += 1

            # place_order(cart_id) -> return a list of products
            for product in self.marketplace.place_order(self.cart_id):
                print(self.name + ' bought ' + str(product))
