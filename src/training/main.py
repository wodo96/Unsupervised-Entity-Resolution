import argparse
from datetime import datetime
import os
import sys
sys.path.append("..")
from util.calculate_precision import cal_prec
from util.cal_score import cal_sco
from util.cal_result import cal_res

parser = argparse.ArgumentParser()
parser.add_argument('-dataset', required=True)
parser.add_argument('-block', type=int)
opt = parser.parse_args()

dataset = opt.dataset
blocks = opt.block
start = datetime.now()


for block in range(1, blocks+1):
    os.system("g++ -fopenmp Matrix_block.cpp -o Matrix_block")
    os.system("./Matrix_block %s %d %d" % (dataset, block, blocks))


end = datetime.now()
total_seconds = (end - start).seconds

hours = total_seconds // 3600
total_seconds = total_seconds % 3600
minutes = total_seconds // 60
seconds = total_seconds % 60

print("Time lost: %d : %d : %d" % (hours, minutes, seconds))

cal_prec(dataset, blocks)
print("DATASET: %s" %dataset)
cal_sco(dataset, blocks)