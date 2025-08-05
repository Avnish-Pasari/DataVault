import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter, AutoLocator
import pandas as pd
import subprocess
import os

def run_part3():
    os.chdir("../../")
    subprocess.run(["./part3_experiments"], stderr=subprocess.STDOUT)
    os.chdir("experiments/part3")
    with open('part3-put.txt', 'r') as file:
        part3_put_results = file.read()
    with open('part3-get.txt', 'r') as file:
        part3_get_results = file.read()
    with open('part3-scan.txt', 'r') as file:
        part3_scan_results = file.read()
        
    os.remove("part3-put.txt")
    os.remove("part3-get.txt")
    os.remove("part3-scan.txt")

    return part3_put_results, part3_get_results, part3_scan_results

def parse_put_data_part3(text_data):
    lines = text_data.strip().split('\n')
    data_tuples = []

    for line in lines:
        parts = line.strip().split(',')
        data_size = int(parts[0].split('=')[1])
        throughput = float(parts[1].split('=')[1])

        data_tuples.append((data_size, throughput))
    
    return data_tuples

def parse_get_scan_data_part3(text_data):
    lines = text_data.strip().split('\n')
    data_tuples = []

    for line in lines:
        parts = line.strip().split(',')
        data_size = int(parts[0].split('=')[1])
        throughput_binarysearch = float(parts[2].split('=')[1])
        throughput_btree = float(parts[1].split('=')[1])
        
        data_tuples.append((data_size, throughput_binarysearch, throughput_btree))
    
    return data_tuples

def generate_put_plot_part3(results):
    data_tuples = parse_put_data_part3(results)

    data_volumes = [data_tuple[0] for data_tuple in data_tuples]
    throughput = [data_tuple[1] for data_tuple in data_tuples]

    plt.figure(figsize=(12, 8))

    plt.plot(data_volumes, throughput, marker='o', label='Throughput', color='r', alpha=0.6)
    
    plt.xscale('log', base=2)
    plt.yscale('linear')
    plt.xticks(data_volumes, labels=[str(volume) for volume in data_volumes])

    plt.xlabel('Volume of Data (MB)')
    plt.title('[Part 3] Data Volume vs PUT Throughput')
    plt.ylabel('PUT Throughput (Operations/Second)')

    ax = plt.gca()
    ax.yaxis.set_major_formatter(ScalarFormatter())
    ax.yaxis.get_major_formatter().set_useOffset(False)
    ax.yaxis.get_major_formatter().set_scientific(False)

    plt.grid(True)
    plt.legend()

    plt.savefig('part3-put.png')

def generate_get_scan_plot_part3(results, operation):
    data_tuples = parse_get_scan_data_part3(results)

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

    if (operation == 'GET'):
        plt.ylabel('GET Throughput (Operations/Second)')
        plt.title('[Part 3] B-Tree Search vs Binary Search GET Performance')
    elif (operation == 'SCAN'):
        plt.ylabel('SCAN Throughput (Operations/Second)')
        plt.title('[Part 3] B-Tree Search vs Binary Search SCAN Performance')
    
    ax = plt.gca()
    ax.yaxis.set_major_formatter(ScalarFormatter())
    ax.yaxis.get_major_formatter().set_useOffset(False)
    ax.yaxis.get_major_formatter().set_scientific(False)

    plt.grid(True)
    plt.legend()
    
    plt.savefig(f'part3-{operation}.png')

def generate_csv_part3(data, operation):
    if (operation == 'PUT'):
        data_tuples = parse_put_data_part3(data)
        data_volumes = [data_tuple[0] for data_tuple in data_tuples]
        throughput = [data_tuple[1] for data_tuple in data_tuples]

        df = pd.DataFrame({'Data Volume (MB)': data_volumes, 'PUT Throughput': throughput})
        df.to_csv('part3-put-results.csv', index=False)
    else:
        data_tuples = parse_get_scan_data_part3(data)
        data_volumes = [data_tuple[0] for data_tuple in data_tuples]
        throughput_binarysearch = [data_tuple[1] for data_tuple in data_tuples]
        throughput_btree = [data_tuple[2] for data_tuple in data_tuples]

        df = pd.DataFrame({
            'Data Volume (MB)': data_volumes,
            f'Binary Search {operation} Throughput': throughput_binarysearch,
            f'B-Tree {operation} Throughput': throughput_btree
        })
        df.to_csv(f'part3-{operation}-results.csv', index=False)

def main():
    part3_put_results, part3_get_results, part3_scan_results = run_part3()

    generate_put_plot_part3(part3_put_results)
    generate_csv_part3(part3_put_results, 'PUT')
    plt.cla()
    plt.clf()

    generate_get_scan_plot_part3(part3_get_results, 'GET')
    generate_csv_part3(part3_get_results, 'GET')
    plt.cla()
    plt.clf()

    generate_get_scan_plot_part3(part3_scan_results, 'SCAN')
    generate_csv_part3(part3_scan_results, 'SCAN')
    
    print("Successfully generated plots and CSV files for Part 3")

main()