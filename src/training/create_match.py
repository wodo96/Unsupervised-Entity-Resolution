import os
import csv
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-dataset', required= True)
parser.add_argument('-block', required= False, default=20)
opt = parser.parse_args()
dataset = opt.dataset
block = opt.block

data_path = "../../data/%s" %(dataset)

valid_path = os.path.join(data_path, "valid.csv")
train_path = os.path.join(data_path, "train.csv")
test_path = os.path.join(data_path, "test.csv")

def readcsv(filename):
    with open(filename,newline= "\n", encoding="ISO-8859-1") as f:
        reader = csv.reader(f)
        return list(reader)

def makefile(valid, train, test):
    match=[]
    
    for i in range(len(valid[1:])):
        if(valid[i][2]=="1"):
            match.insert(i,[valid[i][0],valid[i][1]])
    
    lenv = len(valid[1:])
    for j in range(len(train[1:])):
        if (train[j][2]=="1"):
            if not ([train[j][0],train[j][1]]) in match:
                match.insert((j+(lenv)),[train[j][0],train[j][1]])
    
    lent = len (match)
    for k in range(len(test[1:])):
        if (test[k][2]=="1"):
            if not ([test[k][0],test[k][1]]) in match:
                match.insert((k+(lent)),[test[k][0],test[k][1]])

    match_def=""
    for touple in match:
        match_def += ((str(touple[0])) + "\t" + (str(touple[1])) + "\n")
    return match_def

new_match = makefile(readcsv(valid_path), readcsv(train_path), readcsv(test_path))

with open ((data_path + "/match.txt"), "w+", encoding="utf-8") as f:
    f.write(new_match)