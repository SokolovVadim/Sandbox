import sys
sys.setrecursionlimit(2 ** 31 - 1)


V = int(input())
n_items = int(input())
v = 0
p = 0
gr_arrays = []
for i in range(n_items):
    v, p = map(int, input().split())
    gr_arrays.append([(p * 1.0) / (v * 1.0), v, p])
gr_arrays.sort(reverse=True)
# print(gr_arrays)


#def greedy_one(node, all_v, k):
#    curr_volume, gr_opt = 0, 0
#    if k == 2:
#        for j in range(node, n_items):
#            if curr_volume + gr_arrays[j][k] <= all_v:
#                curr_volume += gr_arrays[j][k]
#                gr_opt += gr_arrays[j][1]
#    if k == 1 or k == 0:
#        for j in range(0, n_items - node):
#            if curr_volume + gr_arrays[j][k] <= all_v:
#                curr_volume += gr_arrays[j][k]
#               gr_opt += gr_arrays[j][1]
#    return gr_opt

def greedy_max(node, total_v):
    gr_1, gr_2, gr_3 = 0, 0, 0
    curr_v1 = 0
    gr_arrays_tmp = gr_arrays[node:]
    gr_arrays_tmp.sort(key=lambda item: item[2], reverse=True)
    for s in range(node, n_items):
        if curr_v1 + gr_arrays[s][1] <= total_v:
            curr_v1 += gr_arrays[s][1]
            gr_1 += gr_arrays[s][2]
    curr_v2 = 0
    for t in range(0, n_items - node):
        if curr_v2 + gr_arrays_tmp[t][1] <= total_v:
            curr_v2 += gr_arrays_tmp[t][1]
            gr_2 += gr_arrays_tmp[t][2]
    curr_v3 = 0
    gr_arrays_tmp.sort(key=lambda item: item[1], reverse=True)
    for m in range(n_items - 1 - node, -1, -1):
        if curr_v3 + gr_arrays_tmp[m][1] <= total_v:
            curr_v3 += gr_arrays_tmp[m][1]
            gr_3 += gr_arrays_tmp[m][2]
    gr_solution = max(gr_1, gr_2, gr_3)
    return gr_solution


def lp(node, now_v):
    lp_solution, curr_v = 0, 0
    
    while (node < n_items) & (curr_v + gr_arrays[node % n_items][1] <= now_v):
        curr_v = curr_v + gr_arrays[node][1]
        lp_solution = lp_solution + gr_arrays[node][2]
        node = node + 1
    
    if node < n_items:
        lp_solution = lp_solution + (now_v - curr_v) * gr_arrays[node][0]
    return lp_solution

def pruning(all_p, all_v, node):
    solution = 0
    if node == n_items - 1:
        return all_p
    if all_v < gr_arrays[node][1]:
        return pruning(all_p, all_v, node + 1)
    
    down_0 = all_p + greedy_max(node + 1, all_v)
    ost1 = all_v - gr_arrays[node][1]
    down_1 = all_p + gr_arrays[node][2] + greedy_max(node + 1, ost1)
    up_0 = all_p + lp(node + 1, all_v)
    ost2 = all_v - gr_arrays[node][1]
    up_1 = all_p + gr_arrays[node][2] + lp(node + 1, ost2)
    
    if down_0 > solution:
        solution = down_0
    if down_1 > solution:
        solution = down_1
    if up_0 <= solution <= up_1:
        return pruning(all_p + gr_arrays[node][2], all_v - gr_arrays[node][1], node + 1)
    elif up_1 <= solution <= up_0:
        return pruning(all_p, all_v, node + 1)
    elif up_1 <= solution and up_0 <= solution:
        return all_p
    
    way1 = pruning(all_p + gr_arrays[node][2], all_v - gr_arrays[node][1], node + 1)
    way2 = pruning(all_p, all_v, node + 1)
    return max(way1, way2)


answer = pruning(0, V, 0)
print(answer)

