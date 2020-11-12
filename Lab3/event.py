from threading import enumerate, Event, Thread, Condition

class Master(Thread):
    def __init__(self, max_work, work_available, result_available, condition):
        Thread.__init__(self, name = "Master")
        self.max_work = max_work
        self.work_available = work_available
        self.result_available = result_available
        self.condition = condition
    
    def set_worker(self, worker):
        self.worker = worker
    
    def run(self):
        for i in range(self.max_work):
            # generate work
            self.work = i
            # notify worker
            #self.work_available.set()
            # get result
            #self.result_available.wait()
            #self.result_available.clear()

            with self.condition:

                self.condition.notify()

                self.condition.wait()

                if self.get_work() + 1 != self.worker.get_result():
                    print ("oops")
                print ("%d -> %d" % (self.work, self.worker.get_result()))
        print("Master done")

    def get_work(self):
        return self.work

class Worker(Thread):
    def __init__(self, terminate, work_available, result_available, condition):
        Thread.__init__(self, name = "Worker")
        self.terminate = terminate
        self.work_available = work_available
        self.result_available = result_available
        self.condition = condition

    def set_master(self, master):
        self.master = master
    
    def run(self):
        while(True):
            # wait work
            #self.work_available.wait()
            #self.work_available.clear()

            if (terminate.is_set()):
                break

            with self.condition:
                if (terminate.is_set()):
                    break
                self.condition.wait()

                if (terminate.is_set()):
                    break

                # generate result
                self.result = self.master.get_work() + 1
                # notify master
                #self.result_available.set()
                self.condition.notify()

    def get_result(self):
        return self.result

if __name__ ==  "__main__":
    # create shared objects
    terminate = Event()
    work_available = Event()
    result_available = Event()
    condition = Condition()

    # start worker and master
    w = Worker(terminate, work_available, result_available, condition)
    m = Master(10, work_available, result_available, condition)
    w.set_master(m)
    m.set_worker(w)
    w.start()
    m.start()

    # wait for master
    m.join()
    with condition:
        condition.notify()
    # wait for worker
    terminate.set()
    print("Got here")

    #work_available.set()
    w.join()


    # print running threads for verification
    print(enumerate())

