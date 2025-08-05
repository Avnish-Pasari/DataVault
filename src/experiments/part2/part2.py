import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd
import subprocess
import os

def run_part2():
    os.chdir("../../")
    subprocess.run(["./part2_experiments"], stderr=subprocess.STDOUT)
    os.chdir("experiments/part2")
    with open('part2-bufferpool.txt', 'r') as file:
        part2_bufferpool_results = file.read()
        
    os.remove("part2-bufferpool.txt")
        
    return part2_bufferpool_results

"""
Structure of Text File:
data_size=X, throughput_binarysearch=Y, throughput_btree=Z
"""
def parse_data_part2(text_data):
    lines = text_data.strip().split('\n')
    data_tuples = []

    for line in lines:
        parts = line.strip().split(',')
        data_size = int(parts[0].split('=')[1])
        throughput_binarysearch = float(parts[1].split('=')[1])
        throughput_btree = float(parts[2].split('=')[1])

        data_tuples.append((data_size, throughput_binarysearch, throughput_btree))
    
    return data_tuples

def generate_plot_part2(results, bufferPool):
    data_tuples = parse_data_part2(results)

    data_volumes = [data_tuple[0] for data_tuple in data_tuples]
    throughput_binarysearch = [data_tuple[1] for data_tuple in data_tuples]
    throughput_btree = [data_tuple[2] for data_tuple in data_tuples]
    
    plt.figure(figsize=(12, 8))

    plt.plot(data_volumes, throughput_binarysearch, marker='o', label='Binary Search Throughput', color='r', alpha=0.6)
    plt.plot(data_volumes, throughput_btree, marker='o', label='B-Tree Throughput', color='g', alpha=0.6)

    plt.xscale('log', base=2) 
    plt.yscale('log', base=10)  
    plt.xticks(data_volumes, labels=[str(volume) for volume in data_volumes])

    plt.xlabel('Volume of Data (MB)')
    plt.ylabel('Throughput (Operations/Second)')

    if (bufferPool):
        plt.title('[Part 2] B-Tree Search vs Binary Search Point Query Performance with Buffer Pool')
    else:
        plt.title('[Part 2] B-Tree Search vs Binary Search Point Query Performance without Buffer Pool')
    
    plt.grid(True)
    plt.legend()
    
    if (bufferPool):
        plt.savefig('part2-bufferpool.png')
    else:
        plt.savefig('part2-nobufferpool.png')

def generate_csv_part2(data, bufferPool=True):
    data_tuples = parse_data_part2(data)
    data_volumes = [data_tuple[0] for data_tuple in data_tuples]
    throughput_binarysearch = [data_tuple[1] for data_tuple in data_tuples]
    throughput_btree = [data_tuple[2] for data_tuple in data_tuples]

    df = pd.DataFrame({'Data Volume': data_volumes, 'Binary Search Throughput': throughput_binarysearch, 'B-Tree Throughput': throughput_btree})
    if (bufferPool):
        df.to_csv('part2-bufferpool-results.csv', index=False)
    else:
        df.to_csv('part2-nobufferpool-results.csv', index=False)

def main():
    part2_bufferpool_results = run_part2()
    generate_plot_part2(part2_bufferpool_results, True)
    generate_csv_part2(part2_bufferpool_results, True)
    plt.cla()
    plt.clf()

    print("Successfully generated plots and CSV files for Part 2 with Buffer Pool")

main()