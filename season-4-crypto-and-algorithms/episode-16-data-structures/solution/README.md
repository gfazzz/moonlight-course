# Solution for Episode 16: Data Structures

## Complete Implementation - SEASON 4 FINALE

This solution implements all fundamental data structures needed for building a database engine.

## Files

- `data_structures.c` — Complete implementation with:
  - Linked List (singly-linked)
  - Stack (array-based, LIFO)
  - Queue (circular array, FIFO)
  - Binary Search Tree (BST)
  - Hash Table (with chaining)
  - AVL Tree (self-balancing, BONUS)

## Data Structures Implemented

### 1. Linked List

**Operations**: O(1) push_front, O(n) push_back, O(n) find, O(n) delete

```c
Node: data + next pointer
Functions: push_front, push_back, find, delete_node, print_list
```

### 2. Stack (LIFO - Last In First Out)

**Operations**: O(1) push, O(1) pop, O(1) peek

```c
Array-based with top pointer
Functions: push, pop, peek, is_empty
Use case: Undo operations, expression evaluation
```

### 3. Queue (FIFO - First In First Out)

**Operations**: O(1) enqueue, O(1) dequeue

```c
Circular array with front/rear pointers
Functions: enqueue, dequeue, is_empty
Use case: Task scheduling, BFS
```

### 4. Binary Search Tree (BST)

**Operations**: O(log n) average, O(n) worst for insert/search/delete

```c
TreeNode: data + left + right pointers
Functions: insert, search, delete, inorder_traversal
Property: left < parent < right
```

### 5. Hash Table

**Operations**: O(1) average insert/search/delete

```c
Array of buckets with chaining (linked list)
Hash function: djb2 (5381 * 33 + c)
Functions: insert, search, delete, load_factor
Collision resolution: Chaining
```

### 6. AVL Tree (BONUS - Self-Balancing BST)

**Operations**: O(log n) guaranteed for all operations

```c
AVLNode: data + height + left + right
Balance factor = height(left) - height(right)
Rotations: left, right, left-right, right-left
Functions: insert with auto-balancing
```

## Complexity Summary

| Data Structure | Insert | Search | Delete | Space |
|----------------|--------|--------|--------|-------|
| Linked List | O(1) front | O(n) | O(n) | O(n) |
| Stack | O(1) | - | O(1) | O(n) |
| Queue | O(1) | - | O(1) | O(n) |
| BST | O(log n) avg | O(log n) avg | O(log n) avg | O(n) |
| Hash Table | O(1) avg | O(1) avg | O(1) avg | O(n) |
| AVL Tree | O(log n) | O(log n) | O(log n) | O(n) |

## Features

✅ Complete implementations of 6 data structures  
✅ All CRUD operations (Create, Read, Update, Delete)  
✅ Memory management (malloc/free)  
✅ Demonstration code for each structure  
✅ Comments explaining complexity  

## Compilation

```bash
gcc -o data_structures data_structures.c -Wall
```

## Usage

```bash
./data_structures
```

The program demonstrates each data structure with example operations.

## Key Concepts

### Hash Table

- **Hash Function**: Maps keys to array indices
- **Collision Resolution**: Chaining (linked list in each bucket)
- **Load Factor**: count / table_size (rehash when > 0.7)

### BST vs AVL

- **BST**: Simple but can degenerate to O(n) if unbalanced
- **AVL**: Self-balancing with rotations, guarantees O(log n)

### When to Use What

- **Array**: Random access O(1), fixed size
- **Linked List**: Dynamic size, frequent insertions/deletions
- **Stack**: LIFO operations (undo, recursion)
- **Queue**: FIFO operations (task queue, BFS)
- **BST**: Sorted data with range queries
- **Hash Table**: Fast lookup by key
- **AVL**: Guaranteed O(log n) with frequent operations

## 🎉 Season 4 Complete!

You've mastered:
- Cryptography (Episode 13)
- Blockchain & Hashes (Episode 14)
- Sorting & Search (Episode 15)
- Data Structures (Episode 16)

**Next**: Season 5 - Financial Markets & Trading Algorithms

