# CAD_VLSI_Design-EDA-



[PA1. **Benchmark_Translator**](./PA1_Benchmark_Translator) - Implemented a program that reads ISCAS’85 netlist descriptions and converts them into the corresponding gate-level Verilog format.

[PA2. **Scheduling**](./PA2_Scheduling-main) - Implemented three key High-Level Synthesis (HLS) scheduling algorithms: ASAP (As-Soon-As-Possible), ALAP (As-Late-As-Possible), and Force-Directed Scheduling (FDS). This program reads a Data-Flow Graph (DFG) and determines the optimal start time for each operation to balance and optimize hardware resources under a given latency constraint.

[PA3. **Partition**](./PA3_partition) - Implemented the Fiduccia-Mattheyses (FM) algorithm for two-way min-cut partitioning. This solution uses a SortedBucketList data structure to efficiently manage and select nodes based on the highest gain. The algorithm iteratively moves nodes, updates neighbor gains, and runs multiple trials to find the optimal minCutSize.

[PA4. **Hardware Trojan Detection**](./PA4_Hardware_Trojan_Detection) - Detected and removed hardware trojans from 4 Verilog circuits. By comparing waveforms (Infected vs. Golden) and analyzing the code, we located and fixed the malicious logic to restore normal functionality.

[Final Project. **Learning Arithmetic Operations from Gate-level Circuit**](https://youtu.be/Fec11u-RrQ0) - Implemented a program to read a gate-level netlist and convert it into a functionally equivalent RTL (Verilog) module with optimized operator cost. This project uses the CUDD library to convert the netlist into an ROBDD (Reduced Ordered Binary Decision Diagram). By creating a library of combinational circuits, it scans and matches circuit blocks to iteratively extract high-level arithmetic operations, ultimately completing the circuit mapping and calculating the cost.
