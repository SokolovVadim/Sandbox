#include <iostream>
#include <vector>

struct Meal
{
	int protein;
	int fat;
	int carbs;
	int calories;
};

int read_data(std::vector<Meal>& data)
{
	Meal meal = {};
	int i(0), meal_num(0);
	std::cin >> meal_num;
	while(i < meal_num)
	{
		meal = {};
		
		std::cin >> meal.protein;
		std::cin >> meal.fat;
		std::cin >> meal.carbs;
		std::cin >> meal.calories;
		
		data.push_back(meal);
		i++;
	}
	return meal_num;
}

void calculate_sum(std::vector<Meal>& data, Meal& sum, int meal_num)
{
	sum = {};
	for(int i(0); i < meal_num; ++i)
	{
		sum.protein += data[i].protein;
		sum.fat += data[i].fat;
		sum.carbs += data[i].carbs;
		sum.calories += data[i].calories;
	}
}

void print_sum(Meal& sum)
{
	std::cout << "Protein: " << sum.protein << std::endl;
	std::cout << "Fat: " << sum.fat << std::endl;
	std::cout << "Carbs: " << sum.carbs << std::endl;
	std::cout << "Calories: " << sum.calories << std::endl;
}

int main()
{
	std::vector<Meal> data;
	int meal_num = read_data(data);
	Meal sum = {};
	calculate_sum(data, sum, meal_num);
	print_sum(sum);
	return 0;
}