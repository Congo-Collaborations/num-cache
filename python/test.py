from numb import *

def run():
    num1 = Numb(1.28763)
    num2 = Numb(-23.5829)
    num3 = num1 + num2
    print "{0} + {1} = {2}".format(num1, num2, num3)
    num4 = num1 + num2 + num3
    print "sum [num1, num2, num3] = {0}".format(num4)
    num5 = num4 * num1
    num6 = num5 / num4
    num7 = exp(num5)

if __name__ == '__main__':
    print "+++ Generating cache1."
    Numb.setup(cache_out='cache1', precision=15)
    run()
    print "+++ Generating cache2 with cache1 as witness."
    Numb.setup(cache_out='cache2', cache_in='cache1', precision=15, strategy='ignore-cache')
    run()
    print "+++ Loading cache1."
    Numb.setup(cache_out='cache2', cache_in='cache1', precision=15, strategy='load-cache')
    run()
    print "+++ Using cache1."
    Numb.setup(cache_out='cache2', cache_in='cache1', precision=15, strategy='use-cache')
    run()
