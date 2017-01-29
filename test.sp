_mverb "./plugin.so" fl

1.3 metro 0.01 0.01 0.01 tenvx 0.8 noise *

#dup 0.97 10000 revsc 
dup _mverb fe 
#dup 20 20 1000 zrev

_mverb fc
