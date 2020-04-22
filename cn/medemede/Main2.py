# -*- coding: utf-8 -*-

from copy import copy
import multiprocessing

import sys

sys.setrecursionlimit(10000000)

def get_link(s, n):
    global link
    if n > 8:
        return
    if (s in link) & (n > 3):
        res.append(copy(link))
        return

    link.append(s)
    visited[s] = 1
    for i in link_list[s]:
        get_link(i, n + 1)
    link.pop()


def find_cycle(v):
    global visited
    global link
    if v in link:
        j = link.index(v)
        if 2 < len(link) - j < 8:
            tmp = link[j:]
            tmp.sort()
            res.append(tuple(tmp))
        return
    link.append(v)
    for t in link_list[v]:
        visited[t] = 1
        find_cycle(t)
    link.pop()


if __name__ == '__main__':

    with open("test_data.txt", "r") as f:
        data = f.read()
    data = data.split('\n')[:-1]
    data = [i.split(',') for i in data]
    source = [int(i[0]) for i in data]
    target = [int(i[1]) for i in data]
    del data

    link_root = list(set(source))
    visited = [0 for i in range(max(link_root) + 1)]
    link_list = [[] for i in range(max(link_root) + 1)]

    for i in range(len(source)):
        link_list[source[i]].append(target[i])
    del target, source

    res = []
    link = []
    for i in link_root:
        if visited[i] == 0:
            link = []
            find_cycle(i)

    res = list(set(res))
    res.sort()
    res = sorted(res, key=lambda i: len(i), reverse=False)
    file = open('result.txt', 'w')
    file.write(str(len(res)) + '\n')
    for i in res:
        file.write(str(i).replace(" ", "")[1:-1] + '\n')
