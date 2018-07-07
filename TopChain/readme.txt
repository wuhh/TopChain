

### compile
The topchain labeling is implemented using c++. We only implement an experimental version. 
Use the following command to compile the program:
make topchain

### run
examples:
./topchain 5 ${file}_scc.txt ${file}_query.txt ${file}_info.txt
arguments:
the first argument is a number K,
the second argument is an input graph, we assume it is a scc,
the third argument is an input query file,
the fourth argument is an info files about the input graph
The specification of these files are in data/readme.txt

### example
we run the script run.sh and get the following output:
1
out.ca-cit-HepPh.uniq
load done.
chain Num : 28093
label done.
query number: 1000
reach number: 529

countQuery= 16903
QUERY_TIME = 2.528 ms
CONSTR_TIME= 1987.97 ms
INDEX_SIZE= 20.6179 MB

The infomation contains the chain number, total query number, the number of query that return true.
It also reports the total query time, label construction time and total index size. 
Warning: Since the result is query-dependent, different query set may have different query time, the query time may not be the same as it is in the original paper.

