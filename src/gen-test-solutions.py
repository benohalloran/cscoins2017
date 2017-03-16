#!/usr/bin/env pypy3

import sys
import hashlib
import random
import json
import heapq

def _int64(x):
    # Get the 64 least significant bits.
    return int(0xFFFFFFFFFFFFFFFF & x)

def seed_from_hash(hash_string):
    hash_prefix = hash_string[:16]
    int_hex = ""
    for i in range(7, -1, -1):
        int_hex += hash_prefix[i*2:(i*2)+2]
    return int(int_hex, 16)

class MT64:
    w = 64
    n = 312
    m = 156
    r = 31
    a = 0xB5026F5AA96619E9
    u = 29
    d = 0x5555555555555555
    s = 17
    b = 0x71D67FFFEDA60000
    t = 37
    c = 0xFFF7EEE000000000
    l = 43
    f = 6364136223846793005

    lower_mask = 0x7FFFFFFF
    upper_mask = 0xffffffff80000000

    def __init__(self, seed):
        self.MT = [0] * MT64.n
        self.seed_mt(seed)

    def seed_mt(self, seed):
        self.index = MT64.n
        self.MT[0] = seed

        for i in range(1, MT64.n):
            self.MT[i] = _int64(MT64.f * (self.MT[i - 1] ^ (self.MT[i - 1] >> (MT64.w - 2))) + i)

    def extract_number(self):
        if self.index >= MT64.n:
            if (self.index > MT64.n):
                raise ValueError("Generator was never seeded")
            self.twist()

        y = self.MT[self.index]
        y = y ^ ((y >> MT64.u) & MT64.d)
        y = y ^ ((y << MT64.s) & MT64.b)
        y = y ^ ((y << MT64.t) & MT64.c)
        y = y ^ (y >> MT64.l)

        self.index += 1

        return y

    def twist(self):
        for i in range(MT64.n):
            x = (self.MT[i] & MT64.upper_mask) + (self.MT[(i + 1) % MT64.n] & MT64.lower_mask)
            xA = x >> 1

            if x % 2 != 0:
                xA = xA ^ MT64.a

            self.MT[i] = self.MT[(i + MT64.m) % MT64.n] ^ xA

        self.index = 0

def generate_solution_sorted(previous_hash, nonce, n):
    seed_hash = hashlib.sha256((previous_hash + str(nonce)).encode()).hexdigest()
    prng = MT64(seed_from_hash(seed_hash))

    element_list = [prng.extract_number() for _ in range(n)]
    element_list.sort()
    return ''.join(map(str, element_list))

def generate_solution_reverse_sorted(previous_hash, nonce, n):
    seed_hash = hashlib.sha256((previous_hash + str(nonce)).encode()).hexdigest()
    prng = MT64(seed_from_hash(seed_hash))

    element_list = [prng.extract_number() for _ in range(n)]
    element_list.sort(reverse=True)
    return ''.join(map(str, element_list))

class PriorityQueue:
    def __init__(self):
        self.elements = []

    def empty(self):
        return len(self.elements) == 0

    def put(self, item, priority):
        heapq.heappush(self.elements, (priority, item))

    def get(self):
        return heapq.heappop(self.elements)[1]

class Grid:
    def __init__(self, grid_size):
        self.grid_size = grid_size
        self.walls = []

    def in_range(self, pos):
        (row, col) = pos
        return 0 < row < self.grid_size - 1 and 0 < col < self.grid_size - 1

    def neighbors(self, pos):
        (row, col) = pos
        results = []
        for coord in ((row+1, col), (row-1, col), (row, col+1), (row, col-1)):
            if self.in_range(coord) and coord not in self.walls:
                results.append(coord)
        return results

def dijkstra_search(grid, start_pos, end_pos):
    frontier = PriorityQueue()
    frontier.put(start_pos, 0)

    came_from = {start_pos: None}
    cost_so_far = {start_pos: 0}

    while not frontier.empty():
        current = frontier.get()

        if current == end_pos:
            break

        for next in grid.neighbors(current):
            new_cost = cost_so_far[current] + 1
            if next not in cost_so_far or new_cost < cost_so_far[next]:
                cost_so_far[next] = new_cost
                frontier.put(next, new_cost)
                came_from[next] = current

    return came_from

def reconstruct_path(came_from, start_pos, end_pos):
    current = end_pos
    path = [current]

    while current != start_pos:
        current = came_from[current]
        path.append(current)

    path.append(start_pos)
    path.reverse()

    fixed_path = []

    for coord in path:
        if len(fixed_path) == 0:
            fixed_path.append(coord)
        else:
            if fixed_path[-1] != coord:
                fixed_path.append(coord)

    return fixed_path

def generate_solution_shortest_path(previous_hash, nonce, n, m):
    grid = Grid(n)
    seed_hash = hashlib.sha256((previous_hash + str(nonce)).encode()).hexdigest()
    prng = MT64(seed_from_hash(seed_hash))

    for i in range(n):
        # placing extremity walls
        grid.walls.append((i, 0))
        grid.walls.append((i, n - 1))

        if i > 0 and i < n - 1:
            grid.walls.append((0, i))
            grid.walls.append((n - 1, i))

    start_pos = (prng.extract_number() % n, prng.extract_number() % n)
    while start_pos in grid.walls:
        start_pos = (prng.extract_number() % n, prng.extract_number() % n)

    end_pos = (prng.extract_number() % n, prng.extract_number() % n)
    while end_pos in grid.walls or start_pos == end_pos:
        end_pos = (prng.extract_number() % n, prng.extract_number() % n)

    # placing walls
    for i in range(m):
        # wall pos (row, col)
        block_pos = (prng.extract_number() % n, prng.extract_number() % n)
        if block_pos != start_pos and block_pos != end_pos and block_pos not in grid.walls:
            grid.walls.append(block_pos)

    path = []
    came_from = dijkstra_search(grid, start_pos, end_pos)
    if end_pos not in came_from:
        return ""
    path = reconstruct_path(came_from, start_pos, end_pos)

    return ''.join(map(lambda x: str(x[0]) + str(x[1]), path))

def loop_sorted(previous_hash, prefix, n):
    prefix_len = len(prefix)

    for nonce in range(1000000):
        if hashlib.sha256(generate_solution_sorted(previous_hash, nonce,
            n).encode()).hexdigest()[:prefix_len] == prefix:
            print(nonce)

def loop_reverse_sorted(previous_hash, prefix, n):
    prefix_len = len(prefix)

    for nonce in range(1000000):
        if hashlib.sha256(generate_solution_reverse_sorted(previous_hash,
            nonce, n).encode()).hexdigest()[:prefix_len] == prefix:
            print(nonce)

def loop_shortest_path(previous_hash, prefix, n, m):
    prefix_len = len(prefix)

    for nonce in range(1000000):
        if nonce % 1000 == 0:
            print(nonce)
        solution = generate_solution_shortest_path(previous_hash, nonce, n,
                m).encode()
        if solution == "":
            print("Failure!", nonce)
        elif hashlib.sha256(solution).hexdigest()[:prefix_len] == prefix:
            print("Success!", nonce)

sorted_list_input = """
{
    "challenge_id": 0,
    "challenge_name": "sorted_list",
    "last_solution_hash": "0000000000000000000000000000000000000000000000000000000000000000",
    "hash_prefix": "94f9",
    "parameters": {
        "nb_elements": 20
    }
}
"""

reverse_sorted_list_input = """
{
    "challenge_id": 1,
    "challenge_name": "reverse_sorted_list",
    "last_solution_hash": "0000000000000000000000000000000000000000000000000000000000000000",
    "hash_prefix": "94f9",
    "parameters": {
        "nb_elements": 20
    }
}
"""

shortest_path_input = """
{
    "challenge_id": 2,
    "challenge_name": "shortest_path",
    "last_solution_hash": "0000000000000000000000000000000000000000000000000000000000000000",
    "hash_prefix": "94f8",
    "parameters": {
        "grid_size": 25,
        "nb_blockers": 80
    }
}
"""

def main():
    js = json.loads(sorted_list_input)
    previous_hash = js["last_solution_hash"]
    n = js["parameters"]["nb_elements"]
    prefix = js["hash_prefix"]

    loop_sorted(previous_hash, prefix, n)

    js = json.loads(reverse_sorted_list_input)
    previous_hash = js["last_solution_hash"]
    n = js["parameters"]["nb_elements"]
    prefix = js["hash_prefix"]

    loop_reverse_sorted(previous_hash, prefix, n)

    js = json.loads(shortest_path_input)
    previous_hash = js["last_solution_hash"]
    n = js["parameters"]["grid_size"]
    m = js["parameters"]["nb_blockers"]
    prefix = js["hash_prefix"]

    loop_shortest_path(previous_hash, prefix, n, m)

if __name__ == "__main__":
    main()
