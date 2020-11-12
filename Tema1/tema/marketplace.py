"""
This module represents the Marketplace.

Computer Systems Architecture Course
Assignment 1
March 2020
"""
from threading import Lock


class Marketplace:
    """
    Class that represents the Marketplace. It's the central part of the implementation.
    The producers and consumers use its methods concurrently.
    """
    def __init__(self, queue_size_per_producer):
        """
        Constructor

        :type queue_size_per_producer: Int
        :param queue_size_per_producer: the maximum size of a queue associated with each producer

        Other attributes

        type producer_id: Int
        param producer_id: the id of a producer

        type cart_id: Int
        param cart_id: the id of the cart of a consumer

        type items: list of dictionaries [{product1: quantity, product2: quantity},
                                           {product3: quantity}]
        param items: an organized structure for the items available in the marketplace
                      a dictionary represents the products of a producer
                      items[producer_id] - the products of the producer with producer_id
                      an entry in the dictionary - product: quantity


        type carts: dictionary {consumer_id: [product1, product2]}
        param carts: a mapping between the cart of a consumer and his products
                      carts[cart_id] - the list of products of the consumer with cart_id


        type producer_queue: dictionary {}
        param producer_queue: a mapping between a producer_id and the number of produced products

        type register_producer_lock: Lock
        param register_producer_lock: a means of controlling the registration of producers
                                       (only one producer should register at a time)

        type register_consumer_lock: Lock
        param register_consumer_lock: a means of controlling the registration of consumers
                                       (only one consumer should register at a time)

        type producers_locks: dictionary {producer_id: Lock}
        param producers_locks: a mapping between a producer_id and a Lock

        type carts_locks: dictionary {cart_id: Lock}
        param carts_locks: a mapping between a cart_id and a Lock
        """

        self.queue_size_per_producer = queue_size_per_producer
        self.producer_id = 0    # the registration id for producers starts from 0
        self.cart_id = 0        # the registration id for consumers starts from 0
        self.items = []
        self.carts = {}
        self.producer_queue = {}

        self.register_producer_lock = Lock()
        self.register_consumer_lock = Lock()
        self.producers_locks = {}

    def register_producer(self):
        """
        Returns an id for the producer that calls this.
        """
        with self.register_producer_lock:
            producer_id = self.producer_id
            self.items.append({})   # a new producer -> a new dictionary
            self.producer_queue[producer_id] = 0        # a new producer has 0 products produced
            self.producers_locks[producer_id] = Lock()  # a producer - a lock for his products
            self.producer_id += 1   # increment producer_id for the next registration
            return producer_id

    def publish(self, producer_id, product):
        """
        Adds the product provided by the producer to the marketplace

        :type producer_id: String
        :param producer_id: producer id

        :type product: Product
        :param product: the Product that will be published in the Marketplace

        :returns True or False. If the caller receives False, it should wait and then try again.
        """

        # the lock is acquired in producer in order to wait the production time
        # if the producer did not yet reach the production limit he may produce
        if self.producer_queue[producer_id] < self.queue_size_per_producer:
            if product in self.items[producer_id]:
                self.items[producer_id][product] += 1   # increase the quantity
            else:
                self.items[producer_id][product] = 1    # create new product entry

            self.producer_queue[producer_id] += 1       # a new product has been produced
            return True
        else:
            return False

    def new_cart(self):
        """
        Creates a new cart for the consumer

        :returns an int representing the cart_id
        """
        with self.register_consumer_lock:
            consumer_id = self.cart_id
            self.carts[consumer_id] = []    # a new consumer needs a new list of products
            self.cart_id += 1   # increment cart_id for the next registration
            return consumer_id

    def add_to_cart(self, cart_id, product):
        """
        Adds a product to the given cart. The method returns

        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to add to cart

        :returns True or False. If the caller receives False, it should wait and then try again
        """

        # get the current number of producers
        with self.register_producer_lock:
            num_of_producers = self.producer_id

        # check each products list for the wanted product
        for producer_id in range(num_of_producers):
            with self.producers_locks[producer_id]:
                # check if the product is in the products list
                if product in self.items[producer_id]:
                    # check if that product has quantity at least 1
                    if self.items[producer_id][product] > 0:
                        # update the quantity and the producer_queue
                        self.items[producer_id][product] -= 1
                        self.producer_queue[producer_id] -= 1

                    # add the found product to the cart
                    self.carts[cart_id].append((product, producer_id))
                    return True
        # the product was not found
        return False

    def remove_from_cart(self, cart_id, product):
        """
        Removes a product from cart.

        :type cart_id: Int
        :param cart_id: id cart

        :type product: Product
        :param product: the product to remove from cart
        """

        cart_item = None
        producer_id = None
        for (cart_product, producer_id) in self.carts[cart_id]:
            if cart_product == product:
                # remove the product from the cart
                self.carts[cart_id].remove((cart_product, producer_id))
                cart_item = (cart_product, producer_id)
                break

        # didn't find the product in the cart
        if cart_item is None or producer_id is None:
            return

        # update the contents of the market and producer_queue
        with self.producers_locks[producer_id]:
            self.items[producer_id][product] += 1
            self.producer_queue[producer_id] += 1

    def place_order(self, cart_id):
        """
        Return a list with all the products in the cart.

        :type cart_id: Int
        :param cart_id: id cart
        """

        # the cart is a list of tuples
        # we need to return only the product part of each tuple
        order = [product for (product, _) in self.carts[cart_id]]

        # the cart becomes empty after the consumer placed the order
        self.carts[cart_id].clear()
        return order
