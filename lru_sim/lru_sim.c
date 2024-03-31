#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Node struct
typedef struct Node {
    char *data;
    struct Node *next;
} Node;

// Define LinkedList struct
typedef struct {
    Node *head;
    Node *tail;
    int length;
} LinkedList;

// Define CacheSimulator struct
typedef struct {
    LinkedList *cache;
    int cache_slots;
    int cache_hit;
    int tot_cnt;
} CacheSimulator;

// Function to create a new Node
Node* createNode(char *data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->data = strdup(data); // strdup 함수를 사용하여 문자열 복사
    new_node->next = NULL;
    return new_node;
}

// Function to create a new LinkedList
LinkedList* createLinkedList() {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

// Function to append data to the LinkedList
void append(LinkedList *list, char *data) {
    Node *new_node = createNode(data);
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->length++;
}

// Function to remove the head of the LinkedList
char* remove_head(LinkedList *list) {
    if (list->head == NULL) {
        return NULL;
    }
    Node *temp = list->head;
    char *data = temp->data;
    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    free(temp->data); // strdup으로 할당된 메모리 해제
    free(temp);
    list->length--;
    return data;
}

// Function to create a new CacheSimulator
CacheSimulator* createCacheSimulator(int cache_slots) {
    CacheSimulator *cache_sim = (CacheSimulator*)malloc(sizeof(CacheSimulator));
    cache_sim->cache_slots = cache_slots;
    cache_sim->cache = createLinkedList();
    cache_sim->cache_hit = 0;
    cache_sim->tot_cnt = 0;
    return cache_sim;
}

// Function to simulate cache
void do_sim(CacheSimulator *cache_sim, char *page) {
    cache_sim->tot_cnt++;

    Node *current = cache_sim->cache->head;
    while (current != NULL) {
        if (strcmp(current->data, page) == 0) {
            cache_sim->cache_hit++;
            remove_head(cache_sim->cache);
            append(cache_sim->cache, page);
            return;
        }
        current = current->next;
    }

    if (cache_sim->cache->length < cache_sim->cache_slots) {
        append(cache_sim->cache, page);
    } else {
        remove_head(cache_sim->cache);
        append(cache_sim->cache, page);
    }
}

// Function to print statistics
void print_stats(CacheSimulator *cache_sim) {
    printf("cache_slot = %d cache_hit = %d hit ratio = %f\n", cache_sim->cache_slots, cache_sim->cache_hit, (float)cache_sim->cache_hit / cache_sim->tot_cnt);
}

int main() {
    FILE *data_file;
    char line[100]; // Assuming maximum line length is 100 characters

    data_file = fopen("./linkbench.trc", "r");
    if (data_file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    for (int cache_slots = 100; cache_slots <= 1000; cache_slots += 100) {
        CacheSimulator *cache_sim = createCacheSimulator(cache_slots);
        while (fgets(line, sizeof(line), data_file) != NULL) {
            char *page = strtok(line, " "); // Split line by space
            do_sim(cache_sim, page);
        }
        rewind(data_file); // Reset file pointer to the beginning
        print_stats(cache_sim);
        free(cache_sim->cache);
        free(cache_sim);
    }

    fclose(data_file);
    return 0;
}
