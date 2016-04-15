#!/usr/bin/env python

import sys
import os

import readline

class Command(object):
    """CLI Command"""
    def __init__(self):
        super(Command, self).__init__()

    def help(self, args=None):
        print('help ...')
		'''
		import inspect
		member = getattr(self, method, None)
		if inspect.ismethod(member):
		    print('{} : {}'.format(member.__name__, member.__doc__))
		'''

    def clear(self, args=None):
        """clear screem"""
        os.system('clear')

class Dispatcher:
    """CLI Dispatcher"""
    def __init__(self, executor):
        self.executor = executor

    @staticmethod
    def _get_line():
        try:
            return raw_input('> ')
        except EOFError:
            return EOFError
        except KeyboardInterrupt:
            return EOFError

    def _dispatcher(self, cmd):
        method = cmd[0]
        args = cmd[1:] if len(cmd) > 1 else []
        func = getattr(self.executor, method, None)
        if func:
            try:
                func(args)
            except:
                pass
        else:
            print('no this command!')

    def loop(self):
        for line in iter(lambda: self._get_line(), EOFError):
            if len(line) <= 0:
                continue
            self._dispatcher(line.split(' '))

if __name__ == "__main__":
    Dispatcher(Command()).loop()
