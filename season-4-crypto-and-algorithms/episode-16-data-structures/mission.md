# Mission 16: Data Structures — SEASON 4 FINALE

## 🎯 Objective

Build MOONLIGHT Database Engine to manage 250,000 operation documents using Hash Tables, BST, Linked Lists, and Stack/Queue. Final mission before Red Square meeting.

## 📋 Mission Briefing

**Date:** December 17, 22:47  
**Location:** MOONLIGHT safe house, Moscow  
**Status:** CRITICAL - FINAL MISSION

Tomorrow is the final meeting. Red Square, 18:00, December 18.

V. has 250,000 operation documents. Each with unique ID, metadata, cross-references. Arrays are too slow (O(n) search). Need ARCHITECTURE.

**Your mission:**
1. Hash Table for O(1) document lookup by ID
2. Binary Search Tree for sorted access
3. Linked List for change history (append-only log)
4. Stack/Queue for processing pipeline
5. Integrate all structures into unified database engine

**Time limit:** Until tomorrow's meeting (19 hours 13 minutes)

## 📂 Files Provided

- `artifacts/documents_250k.dat` — 250K operation documents
- `artifacts/queries.txt` — Test queries (INSERT, SEARCH, UPDATE, DELETE)
- `starter.c` — Template with data structure skeletons

## ✅ Success Criteria

- Hash Table with collision handling (chaining or open addressing)
- BST with insert, search, delete operations
- Linked List for audit log
- Stack for undo operations
- Queue for async task processing
- All structures work together seamlessly

## 💡 Hints

- Hash function: use modulo with prime number for better distribution
- BST: balance is optional, but consider AVL/Red-Black for O(log n)
- Linked List: doubly-linked makes deletion O(1)
- Stack: LIFO — perfect for undo/redo
- Queue: FIFO — perfect for task scheduling

## 🏆 Achievement

**"Data Structure Engineer"** — Build production-grade database engine

**SEASON 4 FINALE** — All 4 episodes complete!

---

**Result:** [Season 4 Complete](../README.md) → [Season 5: Financial Markets](../../season-5-financial-markets/)

