import math
import hashlib
import os
import shutil
import sys
import traceback

import logging
logger = logging.getLogger('calculus')

import coloredlogs
coloredlogs.install(level='DEBUG')

class Numb:
    cache_in = None
    cache_out = None
    precision = 1
    strategy = 'ignore-cache' # or use-cache, load-cache

    @staticmethod
    def setup(cache_out=None, cache_in=None, precision=1, strategy='ignore-cache'):
        Numb.cache_in = cache_in
        Numb.cache_out = cache_out
        Numb.precision = precision
        Numb.strategy = strategy

    @staticmethod
    def cache(signature, content):
        if Numb.cache_out:
            try:
                if not os.path.exists("{0}".format(Numb.cache_out)):
                    os.makedirs("{0}".format(Numb.cache_out))
            except:
                logger.error("Unable to create the cache directory.")
            try:
                with open("{0}/{1}.cache".format(Numb.cache_out, signature), "w") as cache_file:
                    cache_file.write(content)
                    logger.debug("{0} operation was successfuly cached.".format(content.split('|')[0]))
            except:
                logger.error("Unable to open the output cache entry {0} file.".format(signature))

    @staticmethod
    def query(signature=None):
        content = None
        if Numb.cache_in:
            try:
                with open("{0}/{1}.cache".format(Numb.cache_in, signature), "r") as cache_file:
                    content = cache_file.read()
            except:
                traceback.print_exc(file=sys.stdout)
                logger.error("Unable to open the input cache entry {0} file.".format(signature))
                return content
        return content

    @staticmethod
    def check(cache=None, current=None):
        logger.debug("current = {0}".format(current))
        logger.debug("cache = {0}".format(cache.split('|')[1]))
        if cache.split('|')[1] != format(current, '.{0}f'.format(Numb.precision)):
            logger.warning("previously cached operation evaluation changed from [{0}] to [{1}].".format(cache.split('|')[1], current))
            logger.warning("cached: {0}".format(current))
            return False
        else:
            logger.debug("operator gave an identical evaluation that match the cache.")
            return True


    def __init__(self, value):
        # cache leaf if needed.
        if isinstance(value, list):
            self.signature = value[0]
            self.value = value[1]
            content = 'asign|{0}'.format(format(self.value, '.{0}f'.format(Numb.precision)))
            Numb.cache(self.signature, content)
            print "hash = asign{0}".format(format(self.value, '.{0}f'.format(Numb.precision)))
            print "signature = {0}".format(self.signature)
        else:
            self.signature = hashlib.sha256("asign{0}".format(format(value, '.{0}f'.format(Numb.precision)))).hexdigest()
            queried = Numb.query(self.signature)
            if queried:
                self.value = value
            else:
                self.value = value
            content = 'asign|{0}'.format(format(value, '.{0}f'.format(Numb.precision)))
            Numb.cache(self.signature, content)
            print "hash = asign{0}".format(format(value, '.{0}f'.format(Numb.precision)))
            print "signature = {0}".format(self.signature)

    def __str__(self):
        return str("{0} -> {1}".format(self.signature, format(self.value, '.{0}f'.format(Numb.precision))))

    @staticmethod
    def doublon(operator, numb1, numb2, result):
        print "{0}{1}{2}".format(operator, numb1, numb2)
        signature = hashlib.sha256("{0}{1}{2}".format(operator, numb1.signature, numb2.signature)).hexdigest()
        print "hash = {0}{1}{2}".format(operator, numb1.signature, numb2.signature)
        print "signature = {0}".format(signature)
        content = '{0}|{1}|{2}|{3}'.format(operator, format(result, '.{0}f'.format(Numb.precision)), format(numb1.value, '.{0}f'.format(Numb.precision)), format(numb2.value, '.{0}f'.format(Numb.precision)))
        Numb.cache(signature, content)
        queried = Numb.query(signature)
        if queried:
            if Numb.strategy == 'load-cache':
                logger.info("loading [{0}] from cache entry [{1}].".format(queried.split('|')[1], signature))
                return [signature, float(queried.split('|')[1])]
            else:
                if Numb.check(queried, result):
                    return [signature, result]
                else:
                    if Numb.strategy == 'ignore-cache':
                        return [signature, result]
                    elif Numb.strategy == 'use-cache':
                        logger.info("using [{0}] from cache entry [{1}].".format(queried.split('|')[1], signature))
                        return [signature, float(queried.split('|')[1])]
                    else:
                        logger.error("unknown cache strategy provided. Only accepts ['ignore-cache', 'use-cache'].")
                        return [signature, result]
        else:
            return [signature, result]

    def __radd__(self, numb):
        return Numb(Numb.doublon('radd', self, numb, self.value + numb))

    def __add__(self, numb):
        return Numb(Numb.doublon('add', self, numb, self.value + numb.value))

    def __sub__(self, numb):
        return Numb(Numb.doublon('sub', self, numb, self.value - numb.value))

    def __mul__(self, numb):
        return Numb(Numb.doublon('mul', self, numb, self.value * numb.value))

    def __mod__(self, numb):
        return Numb(Numb.doublon('mod', self, numb, self.value % numb.value))

    def __div__(self, numb):
        return Numb(Numb.doublon('div', self, numb, self.value / numb.value))

    def __lt__(self, numb):
        return self.value < numb.value

    def __le__(self, numb):
        return self.value <= numb.value

    def __eq__(self, numb):
        return self.value == numb.value

    def __ne__(self, numb):
        return self.value != numb.value

    def __gt__(self, numb):
        return self.value > numb.value

    def __ge__(self, numb):
        return self.value >= numb.value

    def __getitem__(self, index):
        return Numb(self.value[index])

    def __contains__(self, numb):
        return numb.value in self.value

    def __len__(self):
        return len(self.value)

    @staticmethod
    def singleton(operator, numb, result):
        signature = hashlib.sha256("{0}{1}".format(operator, numb.signature)).hexdigest()
        print "hash = {0}{1}".format(operator, numb.signature)
        print "signature = {0}".format(signature)
        content = '{0}|{1}|{2}'.format(operator, format(result, '.{0}f'.format(Numb.precision)), format(numb.value, '.{0}f'.format(Numb.precision)))
        Numb.cache(signature, content)
        queried = Numb.query(signature)
        if queried:
            if Numb.strategy == 'load-cache':
                logger.info("loading [{0}] from cache entry [{1}].".format(queried.split('|')[1], signature))
                return [signature, float(queried.split('|')[1])]
            else:
                if Numb.check(queried, result):
                    return [signature, result]
                else:
                    if Numb.strategy == 'ignore-cache':
                        return [signature, result]
                    elif Numb.strategy == 'use-cache':
                        logger.info("using [{0}] from cache entry [{1}].".format(queried.split('|')[1], signature))
                        return [signature, float(queried.split('|')[1])]
                    else:
                        logger.error("unknown cache strategy provided. Only accepts ['ignore-cache', 'use-cache'].")
                        return [signature, result]
        else:
            return [signature, result]

def exp(numb):
    return Numb(Numb.singleton('exp', numb, math.exp(numb.value)))

def ceil(numb):
    return Numb(Numb.singleton('ceil', numb, math.ceil(numb.value)))

def fabs(numb):
    return Numb(Numb.singleton('fabs', numb, math.fabs(numb.value)))

def factorial(numb):
    return Numb(Numb.singleton('factorial', numb, math.factorial(numb.value)))

def floor(numb):
    return Numb(Numb.singleton('floor', numb, math.floor(numb.value)))

def fmod(numb):
    return Numb(Numb.singleton('fmod', numb, math.fmod(numb.value)))

def frexp(numb):
    return Numb(Numb.singleton('frexp', numb, math.frexp(numb.value)))

def fsum(numb):
    return Numb(Numb.singleton('fsum', numb, math.fsum(numb.value)))

def ldexp(numb):
    return Numb(Numb.singleton('ldexp', numb, math.ldexp(numb.value)))

def modf(numb):
    return Numb(Numb.singleton('modf', numb, math.modf(numb.value)))

def trunc(numb):
    return Numb(Numb.singleton('trunc', numb, math.trunc(numb.value)))

def expm1(numb):
    return Numb(Numb.singleton('expm1', numb, math.expm1(numb.value)))

def log(numb):
    return Numb(Numb.singleton('log', numb, math.log(numb.value)))

def log1p(numb):
    return Numb(Numb.singleton('log1p', numb, math.log1p(numb.value)))

def log10(numb):
    return Numb(Numb.singleton('log10', numb, math.log10(numb.value)))

def pow(numb):
    return Numb(Numb.singleton('pow', numb, math.pow(numb.value)))

def sqrt(numb):
    return Numb(Numb.singleton('sqrt', numb, math.sqrt(numb.value)))

def acos(numb):
    return Numb(Numb.singleton('acos', numb, math.acos(numb.value)))

def asin(numb):
    return Numb(Numb.singleton('asin', numb, math.asin(numb.value)))

def atan(numb):
    return Numb(Numb.singleton('atan', numb, math.atan(numb.value)))

def atan2(numb):
    return Numb(Numb.singleton('atan2', numb, math.atan2(numb.value)))

def cos(numb):
    return Numb(Numb.singleton('cos', numb, math.cos(numb.value)))

def sin(numb):
    return Numb(Numb.singleton('sin', numb, math.sin(numb.value)))

def tan(numb):
    return Numb(Numb.singleton('tan', numb, math.tan(numb.value)))

def hypot(numb):
    return Numb(Numb.singleton('hypot', numb, math.hypot(numb.value)))

def acosh(numb):
    return Numb(Numb.singleton('acosh', numb, math.acosh(numb.value)))

def asinh(numb):
    return Numb(Numb.singleton('asinh', numb, math.asinh(numb.value)))

def atanh(numb):
    return Numb(Numb.singleton('atanh', numb, math.atanh(numb.value)))

def cosh(numb):
    return Numb(Numb.singleton('cosh', numb, math.cosh(numb.value)))

def sinh(numb):
    return Numb(Numb.singleton('sinh', numb, math.sinh(numb.value)))

def tanh(numb):
    return Numb(Numb.singleton('tanh', numb.value, tanh.hypot(numb.value)))

def erf(numb):
    return Numb(Numb.singleton('erf', numb, math.erf(numb.value)))

def erfc(numb):
    return Numb(Numb.singleton('erfc', numb, math.erfc(numb.value)))

def gamma(numb):
    return Numb(Numb.singleton('gamma', numb, math.gamma(numb.value)))

def lgamma(numb):
    return Numb(Numb.singleton('hypot', numb, math.lgamma(numb.value)))
