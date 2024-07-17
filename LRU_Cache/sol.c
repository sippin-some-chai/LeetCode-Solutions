typedef struct LinkedNode {
    int key;
    int val;
    struct LinkedNode* next;
    struct LinkedNode* prev;
} LinkedNode;

typedef struct HashNode {
    LinkedNode* node;
    struct HashNode* next;
    int key;
} HashNode;

typedef struct LRUCache {
    HashNode** table;
    int capacity;
    int size;
} LRUCache;
// Global vars
static int CAP = 0;
static LinkedNode* head = NULL;
static LinkedNode* tail = NULL;

static void add_link(LinkedNode* in) {
    if (head == NULL && tail == NULL) {
        // set head and tail to new Node
        head = in;
        tail = head;
        head->prev = tail;
        tail->next = head;
    }
    // set new prev and next for head and tail
    tail->next = in;
    head->prev = in;

    // set prev and next for new node
    in->prev = tail;
    in->next = head;

    tail = in;
}
static void extract_link(LinkedNode* node) {
    if (node == head && node == tail) {
        head = NULL;
        tail = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if (node == head) {
            head = node->next;
        }
        if (node == tail) {
            tail = node->prev;
        }
    }
    node->next = NULL;
    node->prev = NULL;
    return;
}
static int hash(int input) {
    if (input < CAP) {
        return input;
    } else {
        return input % CAP;
    }
}

static void add_hash_node(LRUCache* obj, LinkedNode* insert, int key) {
    int hash_id = hash(key);
    HashNode* at = obj->table[hash_id];
    HashNode* new_hash = (HashNode*)malloc(sizeof(HashNode));
    new_hash->node = insert;
    new_hash->key = key;
    new_hash->next = NULL;
    if (at == NULL) {
        obj->table[hash_id] = new_hash;
    } else {
        while (at->next != NULL) {
            at = at->next;
        }
        at->next = new_hash;
    }
}
static void remove_hash_node(LRUCache* obj, int key) {
    int hash_id = hash(key);
    HashNode* at = obj->table[hash_id];
    HashNode* prev = NULL;
    // Traverse the linked list at the hash table entry to find the node
    while (at != NULL && at->key != key) {
        prev = at;
        at = at->next;
    }
    // If the node was not found, return
    if (at == NULL) {
        return;
    }
    // Remove the node from the hash table linked list
    if (prev == NULL) {
        // The node to remove is the first node in the linked list
        obj->table[hash_id] = at->next;
    } else {
        prev->next = at->next;
    }

    LinkedNode* extracted = at->node;
    extract_link(extracted);

    free(at);
    free(extracted);
}

static HashNode* traverse_table(LRUCache* obj, int s_key) {
    int hash_id = hash(s_key);
    HashNode* at = obj->table[hash_id];
    while (at != NULL) {
        if (at->key == s_key) {
            // return position Key Found at
            return at;
        } else {
            at = at->next;
        }
    }
    // key not found
    return NULL;
}

// Int Cache to size of capcacity(i.e init hashtable)
LRUCache* lRUCacheCreate(int capacity) {
    CAP = capacity;
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->table = (HashNode**)malloc(sizeof(HashNode*) * capacity);
    cache->size = 0;
    for (int i = 0; i < capacity; i++) {
        cache->table[i] = NULL;
    }
    return cache;
}

int lRUCacheGet(LRUCache* obj, int key) {
    HashNode* search = traverse_table(obj, key);
    int ret = -1;
    // if Found Pull Link out and place at the end of Linked List
    LinkedNode* extracted = NULL;
    if (search != NULL) {
        extracted = search->node;
        ret = extracted->val;
        extract_link(extracted);
        add_link(extracted);
    }

    return ret;
}
/*
1. Traverse List at Hash to look for key<Done>
2a. If Found update val
    I.  Remove From Linked List <Done>
    II. Place LinkedNode at back of LinkedList <Done>
2b. If not found time to add
    I. If size == capacity pop head
    II. Otherwise just add to tail <Done>
    III. Increment Vars as Needed

*/
void lRUCachePut(LRUCache* obj, int key, int value) {
    HashNode* search = traverse_table(obj, key);
    // if Found Pull Link out and place at the end of Linked List
    LinkedNode* extracted = NULL;
    if (search != NULL) {
        extracted = search->node;
        extracted->val = value;
        extract_link(extracted);
        add_link(extracted);
        return;
    }

    LinkedNode* new_link = (LinkedNode*)malloc(sizeof(LinkedNode));
    new_link->key = key;
    new_link->val = value;
    if (obj->size == obj->capacity) {
        remove_hash_node(obj, head->key);
        obj->size -= 1;
    }
    add_link(new_link);
    add_hash_node(obj, new_link, key);
    obj->size += 1;
    return;
}

void lRUCacheFree(LRUCache* obj) { 
    head = NULL;
    tail = NULL;

 }

/**
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);

 * lRUCachePut(obj, key, value);

 * lRUCacheFree(obj);
*/
