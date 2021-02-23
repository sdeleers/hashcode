#include <bits/stdc++.h>
using namespace std;

struct Pizza {
  // Index into original list.
  int index;
  // Contains the indices of the ingredients in the pizza.
  vector<int> ingredients;
};


struct PizzaAssignment {
  vector<vector<Pizza>> assignment;

  void print() {
    cout << "assignment: " << endl;
    for (const auto& pizzas : assignment) {
      cout << "   ";
      for (const auto& pizza : pizzas) {
        cout << pizza.index << " ";
      }
      cout << endl;
    }
  }

  int64_t calculateScore(vector<int> num_teams) {
    int64_t score = 0;
    for (const auto& pizzas : assignment) {
      const int team_size = pizzas.size();
      if (team_size < 2 || team_size > 4 || num_teams[team_size] == 0) {
        // Invalid assignment.
        return static_cast<int64_t>(-1);
      }

      --num_teams[team_size];
      unordered_set<int> unique_pizzas;
      for (const auto& pizza : pizzas) {
        for (int index : pizza.ingredients) {
          unique_pizzas.insert(index);
        }
      }
      score += unique_pizzas.size() * unique_pizzas.size();
    }
    return score;
  }
};

PizzaAssignment randomStrategy(const vector<Pizza>& pizzas,
                               vector<int> num_teams) {
  // Randomly shuffle the pizzas.                                 
  const int P = pizzas.size();                                 
  vector<int> pizza_indices(P);
  iota(pizza_indices.begin(), pizza_indices.end(), 0);
  random_shuffle(pizza_indices.begin(), pizza_indices.end());

  PizzaAssignment assignment;
  // Do largest team first.
  int pizza_index = 0;
  while (pizza_index < P) {
    int num_remaining_pizzas = P - pizza_index;
    int team_size = 0;
    if (num_teams[4] > 0 && num_remaining_pizzas >= 4) {
      // Take team of 4.
      team_size = 4;
    } else if (num_teams[3] > 0 && num_remaining_pizzas >= 3) {
      // Take team of 3.
      team_size = 3;
    } else if (num_teams[2] > 0 && num_remaining_pizzas >= 2) {
      // Take team of 2.
      team_size = 2;
    } else {
      break;
    }

    vector<Pizza> team_pizzas;
    for (int i = 0; i < team_size; ++i) {
      const auto& pizza = pizzas[pizza_indices[pizza_index]];
      team_pizzas.push_back(pizza);
      ++pizza_index;
    }
    --num_teams[team_size];
    assignment.assignment.push_back(team_pizzas);
  }
  return assignment;
}

int main() {
  int num_pizzas;
  cin >> num_pizzas;
  vector<int> num_teams(5, 0);
  for (int i = 2; i <= 4; ++i) {
    cin >> num_teams[i];
  }

  unordered_map<string, int> pizza_to_index;

  vector<Pizza> pizzas(num_pizzas);
  int num_unique_ingredients = 0;
  for (int i = 0; i < num_pizzas; ++i) {
    int num_ingredients;
    cin >> num_ingredients;
    for (int j = 0; j < num_ingredients; ++j) {
      string ingredient;
      cin >> ingredient;
      if (pizza_to_index.count(ingredient) == 0) {
        pizza_to_index[ingredient] = num_unique_ingredients;
        ++num_unique_ingredients;
      }
      pizzas[i].ingredients.push_back(pizza_to_index[ingredient]);
      pizzas[i].index = i;
    }
  }

  PizzaAssignment assignment = randomStrategy(pizzas, num_teams);
  // assignment.print();
  cout << assignment.calculateScore(num_teams) << endl;
}