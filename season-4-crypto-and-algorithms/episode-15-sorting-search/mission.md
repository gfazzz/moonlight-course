# Mission 15: Sorting & Search Algorithms

## 🎯 Objective

Sort 10 MILLION server log records and find critical timestamp containing coordinates for agent meeting. Time limit: 3 hours until dawn.

## 📋 Mission Briefing

**Date:** December 17, 03:15  
**Location:** MOONLIGHT safe house, Moscow  
**Status:** CRITICAL - DATA OVERLOAD

V. has obtained complete server logs from the organization. 10 MILLION records. Coordinates are encrypted in timestamps. Linear search would take HOURS. You have 3 hours.

**Your mission:**
1. Implement optimal sorting algorithms (Quicksort, Merge Sort, Heap Sort)
2. Implement binary search for timestamp lookup
3. Analyze Big O complexity
4. Compare performance on 10^7 records
5. Extract coordinates from critical timestamp

**Time limit:** Until 06:15 (3 hours)

## 📂 Files Provided

- `artifacts/server_logs_10M.dat` — 10 million records (binary format)
- `artifacts/target_timestamp.txt` — Timestamp to find
- `starter.c` — Template with sorting/search functions

## ✅ Success Criteria

- Implement Quicksort, Merge Sort, Heap Sort
- Implement Binary Search
- Sort 10M records in < 5 minutes
- Find target timestamp in < 1 second
- Extract coordinates successfully

## 💡 Hints

- Quicksort average: O(n log n), worst: O(n²) — use random pivot!
- Merge Sort: O(n log n) guaranteed, but O(n) extra space
- Heap Sort: O(n log n), in-place, but slower constants
- Binary Search: O(log n) — only works on sorted data!

## 🏆 Achievement

**"Algorithm Master"** — Conquer 10M records with optimal algorithms

---

**Next:** [Episode 16: Data Structures](../episode-16-data-structures/)

