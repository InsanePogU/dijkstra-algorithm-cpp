## Problem Description
The goal of the project is to find the shortest path through a grid-based terrain with varying heights and special one-way transport links (lifts).

### Movement Rules:
* **Standard movement:** Between four adjacent fields (up, down, left, right). 
  * If Target Height ($A$) > Current Height ($B$): Cost = $A - B + 1$ minutes.
  * If $A \le B$: Cost = 1 minute.
* **Lifts:** One-way transport between specific coordinates.
  * Lifts have a **travel time** and a **frequency** (interval).
  * Waiting time depends on the arrival time: `wait = (interval - current_time % interval) % interval`.

## Key Features
* **Custom Binary Heap:** Manual implementation of a priority queue (`sift_up`, `sift_down`) without using STL containers.
* **Dynamic Edge Weights:** Handles time-dependent costs for lift travel.
* **Manual Memory Management:** Efficient use of dynamic arrays and pointers for grid representation.

## Tech Stack
* **Language:** C++
* **Data Structures:** Custom Binary Heap, Adjacency List (for lifts), 2D Grid.

## Input Format
The program reads from standard input:
1. `W H` (Map dimensions)
2. `sx sy` (Start coordinates)
3. `dx dy` (Destination coordinates)
4. `num_lifts` (Number of lifts)
5. Lift details: `x1 y1 x2 y2 travel_time interval`
6. `W * H` integers representing terrain heights.
