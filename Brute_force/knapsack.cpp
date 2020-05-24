#include <iostream>
#include <algorithm>
#include <vector>

struct Data{
	int v;
	int p;
	double q;

	Data(int v_, int p_):
		v(v_),
		p(p_),
		q(double(double(p_) / double(v_)))
	{}

	bool operator<(const Data& other)
	{
		return this->q < other.q;	
	}
};

bool Compare_Rows_v(const Data& a, const Data& b)
{
	return a.v < b.v;
}

bool Compare_Rows_p(const Data& a, const Data& b)
{
	return a.p < b.p;
}

void store_and_sort(int n_items, std::vector<Data> & gr_arrays)
{
	int v(0), p(0);
	for(int i(0); i < n_items; ++i)
	{
		std::cin >> v >> p;
		Data data(v, p);
		gr_arrays.push_back(data);
	}
	std::sort(&gr_arrays[0], &gr_arrays[n_items]);
}

void PrintData(int n_items, std::vector<Data> & gr_arrays)
{
	for(int i(0); i < n_items; ++i)
		std::cout << gr_arrays[i].q << " " << gr_arrays[i].v << " " << gr_arrays[i].p << "\n";
}

double lp(int node, int now_v, int n_items, std::vector<Data> & gr_arrays)
{
	double lp_solution(0), curr_v(0);
	while((node < n_items) && (curr_v + gr_arrays[node % n_items].v <= now_v))
	{
		curr_v += gr_arrays[node].q;
		lp_solution = lp_solution + gr_arrays[node].p;
		node++;
	}
	if(node < n_items)
		lp_solution += (now_v - curr_v) * gr_arrays[node].q;
	return lp_solution;
}

int maximum(int x, int y, int z) {
	int max = x; 

	if (y > max) { 
		max = y;
	} 

	if (z > max) { 
		max = z;
	} 

	return max;
}

int greedy_max(int node, double total_v, int n_items, std::vector<Data> & gr_arrays)
{
	int gr_1(0), gr_2(0), gr_3(0);
	std::vector<Data> gr_arrays_temp;
	for(int i(node); i < n_items; ++i)
	{
		gr_arrays_temp.push_back(gr_arrays[i]);
	}

	std::sort(&gr_arrays_temp[0], &gr_arrays_temp[n_items - node], Compare_Rows_v);
	double cur_v1(0);
	for(int i(node); i < n_items; ++i)
	{
		if(cur_v1 + gr_arrays[i].v <= total_v)
		{
			cur_v1 += gr_arrays[i].v;
			gr_1 += gr_arrays[i].p;
		}
	}

	double cur_v2(0);
	for(int i(0); i < n_items - node; ++i)
	{
		if(cur_v2 + gr_arrays_temp[i].v <= total_v)
		{
			cur_v2 += gr_arrays_temp[i].v;
			gr_2 += gr_arrays_temp[i].p;
		}
	}

	std::sort(&gr_arrays_temp[0], &gr_arrays_temp[n_items - node], Compare_Rows_p);
	double cur_v3(0);
	for(int i(n_items - node - 1); i > -1; --i) /// i >= -1
	{
		if(cur_v3 + gr_arrays_temp[i].v <= total_v)
		{
			cur_v3 += gr_arrays_temp[i].v;
			gr_3 += gr_arrays_temp[i].p;
		}
	}

	int gr_solution = maximum(gr_1, gr_2, gr_3);
	return gr_solution;
}

int pruning(int all_p, int all_v, int node, int n_items, std::vector<Data> & gr_arrays)
{
	int solution(0);
	if(node == n_items - 1)
		return all_p;
	if(all_v < gr_arrays[node].v)
		return pruning(all_p, all_v, node + 1, n_items, gr_arrays);

	int down_0 = all_p + greedy_max(node + 1, all_v, n_items, gr_arrays);
	int ost1 = all_v - gr_arrays[node].v;

	int down_1 = all_p + gr_arrays[node].p + greedy_max(node + 1, ost1, n_items, gr_arrays);
	int up_0 = all_p + lp(node + 1, all_v, n_items, gr_arrays);

	int ost2 = all_v - gr_arrays[node].v;
	int up_1 = all_p + gr_arrays[node].p + lp(node + 1, ost2, n_items, gr_arrays);

	if(down_0 > solution)
		solution = down_0;
	if(down_1 > solution)
		solution = down_1;
	if((up_0 <= solution) && (solution <= up_1))
		return pruning(all_p + gr_arrays[node].p, all_v - gr_arrays[node].v, node + 1, n_items, gr_arrays);
	else if((up_1 <= solution) && (solution <= up_0))
		return pruning(all_p, all_v, node + 1, n_items, gr_arrays);
	else if((up_1 <= solution) && (up_0 <= solution))
		return all_p;

	int way1 = pruning(all_p + gr_arrays[node].p, all_v - gr_arrays[node].v, node + 1, n_items, gr_arrays);
	int way2 = pruning(all_p, all_v, node + 1, n_items, gr_arrays);
	return std::max(way1, way2);
}

int main()
{
	int V(0), n_items(0);
	std::cin >> V >> n_items;
	std::vector<Data> gr_arrays;
	store_and_sort(n_items, gr_arrays);

	int answer = pruning(0, V, 0, n_items, gr_arrays);
	std::cout << answer << std::endl;

	return 0;
}