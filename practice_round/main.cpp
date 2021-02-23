#include <bits/stdc++.h>
using namespace std;

struct Pizza {
  // Index into original list.
  int index;
  // Contains the indices of the ingredients in the pizza.
  vector<int> ingredients;
};

int64_t pizzasScore(const vector<Pizza>& pizzas) {
  unordered_set<int> unique_pizzas;
  for (const auto& pizza : pizzas) {
    for (int index : pizza.ingredients) {
      unique_pizzas.insert(index);
    }
  }
  return unique_pizzas.size() * unique_pizzas.size();
}

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
      score += pizzasScore(pizzas);
    }
    return score;
  }
};

int64_t randomInRange(int l, int r) {
  return l + (rand() % (r - l + 1));
}

void improveBySwapping(PizzaAssignment& pizza_assignment) {
  // Randomly select 2 pizzas, and consider swapping them.
  const int A = pizza_assignment.assignment.size();
  for (int iter = 0; iter < 100000; ++iter) {
    const int pizzas1 = randomInRange(0, A - 1);
    const int pizzas2 = randomInRange(0, A - 1);
    if (pizzas1 != pizzas2) {
      const int64_t score_before = pizzasScore(pizza_assignment.assignment[pizzas1]) + pizzasScore(pizza_assignment.assignment[pizzas2]);
      const int a = randomInRange(0, pizza_assignment.assignment[pizzas1].size() - 1);  // index into pizzas1
      const int b = randomInRange(0, pizza_assignment.assignment[pizzas2].size() - 1);  // index into pizzas2
      // Try swapping them.
      swap(pizza_assignment.assignment[pizzas1][a], pizza_assignment.assignment[pizzas2][b]);
      const int64_t score_after = pizzasScore(pizza_assignment.assignment[pizzas1]) + pizzasScore(pizza_assignment.assignment[pizzas2]);
      if (score_before > score_after) {
        // Swap them back.
        swap(pizza_assignment.assignment[pizzas1][a], pizza_assignment.assignment[pizzas2][b]);
      }
    }    
  }
}

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
  improveBySwapping(assignment);
  // assignment.print();
  cout << assignment.calculateScore(num_teams) << endl;
}
