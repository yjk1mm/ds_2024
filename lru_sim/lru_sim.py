class Node:
    def __init__(self, data=None):
        self.data = data
        self.next = None


class LinkedList:
    def __init__(self):
        self.head = None
        self.tail = None
        self.length = 0

    def append(self, data):
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            self.tail = new_node
        else:
            self.tail.next = new_node
            self.tail = new_node
        self.length += 1

    def remove_head(self):
        if not self.head:
            return None
        data = self.head.data
        self.head = self.head.next
        if not self.head:
            self.tail = None
        self.length -= 1
        return data

    def __len__(self):
        return self.length


class CacheSimulator:
    def __init__(self, cache_slots):
        self.cache_slots = cache_slots
        self.cache = LinkedList()
        self.cache_hit = 0
        self.tot_cnt = 0
    
    def do_sim(self, page):
        self.tot_cnt += 1
        
        current = self.cache.head
        while current:
            if current.data == page:
                self.cache_hit += 1
                self.cache.remove_head()
                self.cache.append(page)
                return
            current = current.next
        
        if len(self.cache) < self.cache_slots:
            self.cache.append(page)
        else:
            self.cache.remove_head()
            self.cache.append(page)
    
    def print_stats(self):
        print("cache_slot =", self.cache_slots, "cache_hit =", self.cache_hit, "hit ratio =", self.cache_hit / self.tot_cnt)


if __name__ == "__main__":
    data_file = open("./linkbench.trc")
    lines = data_file.readlines()
    for cache_slots in range(100, 1001, 100):
        cache_sim = CacheSimulator(cache_slots)
        for line in lines:
            page = line.split()[0]
            cache_sim.do_sim(page)
        
        cache_sim.print_stats()
