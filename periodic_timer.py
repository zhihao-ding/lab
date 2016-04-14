#!/usr/bin/env python

import threading
import time

class PeriodicTimer:
    def __init__(self, interval):
        self.interval = interval
        self.flag = 0
        self.condition_variable = threading.Condition()

    def start(self):
        t = threading.Thread(target=self.run)
        t.daemon = True
        t.start()

    def run(self):
        '''Run the timer and notify waiting threads after each interval'''
        while True:
            time.sleep(self.interval)
            with self.condition_variable:
                self.flag ^= 1
                self.condition_variable.notify_all()

    def wait_for_tick(self):
        '''Wait for the next tick of the timer'''
        with self.condition_variable:
            last_flag = self.flag
            while last_flag == self.flag:
                self.condition_variable.wait()

ptimer = PeriodicTimer(5)
ptimer.start()

def countdown(nticks):
    while nticks > 0:
        ptimer.wait_for_tick()
        print('T-minus', nticks)
        nticks -= 1

def countup(last):
    n = 0
    while n < last:
        ptimer.wait_for_tick()
        print('Counting', n)
        n += 1

threading.Thread(target=countdown, args=(10,)).start()
threading.Thread(target=countup, args=(5,)).start()
