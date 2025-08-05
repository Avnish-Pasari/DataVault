# Key-Value Store Database 📦

A high-performance **Key-Value (KV) Store** database system implemented in **C++17**. 
This project demonstrates modern database design concepts such as **Memtables, SSTables, Buffer Pools, B-Trees, and LSM Trees with Bloom Filters**.  

---

## 📖 About the Project

The system implements a disk-backed KV store capable of supporting:

- `open` → Open/create a database  
- `close` → Flush memtable and close DB  
- `put` → Insert KV pairs  
- `get` → Retrieve values by key  
- `scan` → Range queries  
- `delete` → Remove KV pairs  

The implementation is divided into three stages:

1. **Memtable & SSTables (Part 1)**  
   - AVL tree-based memtable  
   - Flush to disk as Sorted String Tables (SSTs)  
   - Optimized write I/O using page-level buffering  

2. **Buffer Pools & B-Tree SSTs (Part 2)**  
   - Extendible buffer pool with **LRU eviction policy**  
   - B-Tree structured SSTs for efficient indexing  
   - Random page access with MurmurHash3-based hashing  

3. **LSM Tree with Bloom Filters (Part 3)**  
   - Multi-level SST storage  
   - Bloom Filters for fast membership tests  
   - Compaction mechanics for managing updates/deletions  

---

## 🛠️ Features

- **Efficient In-Memory Data Structures**
  - AVL tree-based memtable for balanced insert/search  
  - Custom Buffer Pool for caching pages  

- **On-Disk Storage**
  - Binary file API for 4KB page-aligned storage  
  - Metadata management via text files (`db_names.txt`, `file_names.txt`)  

- **Optimized Search & Writes**
  - Binary search on SST pages  
  - Buffered writes reducing I/O cost from **O(N) → O(N/B)**  

- **Advanced Indexing**
  - Static B-Trees for fast lookups  
  - Bloom Filters for probabilistic existence checks  

- **Testing & Debugging**
  - 100+ unit and integration tests  
  - Support for custom fanout & buffer pool collision tests  
