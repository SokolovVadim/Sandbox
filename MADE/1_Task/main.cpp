#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Dish
{
	std::string name;
	int number;
};

struct Ingredient
{
	std::string name;
	int number;
};

struct Recipe
{
	std::string name;
	int r_number_of_ingredients;
	std::vector<Ingredient> ingredients;
};

struct Fridge
{
	std::string name;
	int number;
};

struct Data_set
{
	int n_dishes_num;
	int k_recipes_num;
	int f_products_num;
	std::vector<Dish> dish;
	std::vector<Recipe> recipe;
	std::vector<Fridge> fridge;
};

void read_dish(std::vector<Dish>& dish, int n_dishes_num, std::ifstream& fin)
{
	std::cout << "DISHES:\n";
	for(int i(0); i < n_dishes_num; ++i)
	{
		Dish cur_dish = {};

		fin >> cur_dish.name >> cur_dish.number;
		dish.push_back(cur_dish);
		std::cout << "name: " << dish[i].name << ",\tnumber: " << dish[i].number << std::endl;
	}
}

void read_recipe(std::vector<Recipe>& recipe, int k_recipes_num, std::ifstream& fin)
{
	std::cout << "Recipes:\n";

	for(int i(0); i < k_recipes_num; ++i)
	{
		Recipe cur_recipe = {};
		fin >> cur_recipe.name >> cur_recipe.r_number_of_ingredients;
		std::cout << "recipe[" << i << "]:\n";
		for(int j(0); j < cur_recipe.r_number_of_ingredients; ++j)
		{
			Ingredient cur_ingredient = {};
			fin >> cur_ingredient.name >> cur_ingredient.number;
			cur_recipe.ingredients.push_back(cur_ingredient);
			std::cout << cur_recipe.ingredients[j].name << " " << cur_recipe.ingredients[j].number << std::endl;
		}
		recipe.push_back(cur_recipe);
	}
}

void read_fridge(std::vector<Fridge>& fridge, int f_products_num, std::ifstream& fin)
{
	std::cout << "Fridge\n";

	for(int i(0); i < f_products_num; ++i)
	{
		Fridge cur_fridge = {};
		fin >> cur_fridge.name >> cur_fridge.number;
		fridge.push_back(cur_fridge);
		std::cout << "name: " << fridge[i].name << ",\tnumber: " << fridge[i].number << std::endl;
	}
}

void read_data_set(Data_set& data_set, std::ifstream& fin)
{
	// int n_dishes_num(0), k_recipes_num(0), f_products_num(0);
	fin >> data_set.n_dishes_num >> data_set.k_recipes_num >> data_set.f_products_num;
	std::cout << "n_dishes_num:\t" << data_set.n_dishes_num <<
	"\nk_recipes_num:\t" << data_set.k_recipes_num << 
	"\nf_products_num:\t" << data_set.f_products_num << std::endl;

	data_set.dish.reserve(data_set.n_dishes_num);
	read_dish(data_set.dish, data_set.n_dishes_num, fin);

	data_set.recipe.reserve(data_set.k_recipes_num);
	read_recipe(data_set.recipe, data_set.k_recipes_num, fin);

	data_set.fridge.reserve(data_set.f_products_num);
	read_fridge(data_set.fridge, data_set.f_products_num, fin);
}

int main()
{
	std::cout << "I'm here!\n";

	std::ifstream fin("input.txt");

	int x_data_set_num(0);
	fin >> x_data_set_num;
	std::cout << "x_data_set_num:\t" << x_data_set_num << std::endl;

	std::vector<Data_set> data_set_vec(x_data_set_num);

	for(int i(0); i < x_data_set_num; ++i)
	{
		read_data_set(data_set_vec[i], fin);
	}

	fin.close();

	return 0;
}