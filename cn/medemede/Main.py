# -*- coding: utf-8 -*-

from copy import copy


def get_link(o, s, i):
    if i > 7:
        return
    for t in link_list[s]:
        if (o == t) & (i > 2):
            res.append(copy(link))
        if (t > o) & (t not in link):
            link.append(t)
            get_link(o, t, i + 1)
            link.pop()


if __name__ == '__main__':

    with open("../Data/test_data.txt", "r") as f:
        data = f.read()
    data = data.split('\n')[:-1]
    data = [i.split(',') for i in data]
    source = [int(i[0]) for i in data]
    target = [int(i[1]) for i in data]
    del data

    link_root = list(set(source))
    link_list = [[] for i in range(max(link_root) + 1)]

    for i in range(len(source)):
        link_list[source[i]].append(target[i])
    del target, source

    res = []
    for i in link_root:
        link = [i]
        get_link(i, i, 1)

    print(res)
    print(len(res))
    res.sort()
    res = sorted(res, key=lambda i: len(i), reverse=False)
    file = open('result.txt', 'w')
    file.write(str(len(res)) + '\n')
    for i in res:
        file.write(str(i).replace(" ", "")[1:-1] + '\n')
