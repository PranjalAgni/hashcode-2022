#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double lld;

#ifndef ONLINE_JUDGE
#define debug(x) cerr << #x <<" "; _print(x); cerr << endl;
#else
#define debug(x)
#endif

void _print(ll t) {cerr << t;}
void _print(int t) {cerr << t;}
void _print(string t) {cerr << t;}
void _print(char t) {cerr << t;}
void _print(lld t) {cerr << t;}
void _print(double t) {cerr << t;}
void _print(ull t) {cerr << t;}


struct Customer {
	int customerId;
	vector<string> likedIngridients;
	vector <string> dislikedIngridients;
};

struct Solution {
	unordered_set<int> goodCustomers;
	unordered_set<int> unusedCustomers;
	unordered_set<string> ingridients;
};


int intersectSize(vector<string>& arr, unordered_set<string>& ingridientsSet) {
	int size = 0;
	for (string& name : arr) {
		if (ingridientsSet.find(name) != ingridientsSet.end()) size += 1;
	}

	return size;
}

bool isHappyCustomer(Customer& customer, unordered_set<string>& ingridientsSet) {
	int likeCount = intersectSize(customer.likedIngridients, ingridientsSet);
	if (likeCount < customer.likedIngridients.size()) {
		return false;
	}

	int dislikeCount = intersectSize(customer.dislikedIngridients, ingridientsSet);
	if (dislikeCount > 0) {
		return false;
	}

	return true;
}

// Local simulator function, which tells the score of generated output
int score(unordered_set<string>& ingridientsSet, vector<Customer>& customerList) {
	int score = 0;
	for (Customer& customer : customerList) {
		if (isHappyCustomer(customer, ingridientsSet)) score += 1;
	}

	return score;
}

// writes the output to the output-score.txt file
void writeOutput(Solution& solution) {
	ofstream fout;
	string fileName = "output-" + to_string(solution.goodCustomers.size());
	fout.open(fileName, ios::out);

	if (fout) {
		fout << solution.ingridients.size() << " ";
		for (auto& ing : solution.ingridients) {
			fout << ing << " ";
		}
	}

	fout.close();
};

vector<Customer> prepareCustomerList() {
	int customers;
	cin >> customers;
	vector<Customer> customerList(customers);

	for (int customerIdx = 0; customerIdx < customers; customerIdx++) {
		int numLike, numDisLike;
		cin >> numLike;
		vector<string> likeIngridients(numLike);

		for (int idx = 0; idx < numLike; ++idx) {
			cin >> likeIngridients[idx];
		}

		cin >> numDisLike;
		vector<string> dislikeIngridients(numDisLike);

		for (int idx = 0; idx < numDisLike; ++idx) {
			cin >> dislikeIngridients[idx];
		}

		struct Customer customer = {customerIdx, likeIngridients, dislikeIngridients};
		customerList[customerIdx] = customer;
	}

	return customerList;
}

unordered_set<string> getIngridients(vector<Customer>& customerList) {
	unordered_set<string> ingridientsSet;
	for (Customer& customer : customerList) {
		ingridientsSet.insert(customer.likedIngridients.begin(), customer.likedIngridients.end());
		ingridientsSet.insert(customer.dislikedIngridients.begin(), customer.dislikedIngridients.end());
	}

	return ingridientsSet;
}

unordered_set<string> getIngridientsFromFile(string fileName) {
	unordered_set<string> ingridients;
	fstream fio;
	string line;

	fio.open(fileName, ios::in);
	if (fio.is_open()) {
		string word;
		getline(fio, line);
		istringstream ss(line);
		bool isFirst = true;
		while (ss >> word) {
			if (isFirst) {
				isFirst = false;
				continue;
			}
			ingridients.insert(word);
		}
	}

	fio.close();
	return ingridients;
}

unordered_map<int, Customer> buildIdVsCustomerMap(vector<Customer>& customerList) {
	unordered_map<int, Customer> idVsCustomerMap;
	for (Customer& customer : customerList) {
		idVsCustomerMap[customer.customerId] = customer;
	}

	return idVsCustomerMap;
}

// initalizes a solution containing good and bad customers
// good customers - who have 0 dislike ingridients
// bad customers - who are not good customers
Solution initalizeSolution(vector<Customer>& customerList, unordered_set<string>& ingridientsSet) {
	unordered_set<int> goodCustomers;
	unordered_set<int> unusedCustomers;
	for (Customer& customer : customerList) {
		if (isHappyCustomer(customer, ingridientsSet)) {
			goodCustomers.insert(customer.customerId);
		} else {
			unusedCustomers.insert(customer.customerId);
		}
	}

	struct Solution solution = {goodCustomers, unusedCustomers, ingridientsSet};
	return solution;
}

Solution hillClimbingOptimization(Solution& solution, vector<Customer>& customerList) {
	int retry = 0;
	unordered_map<int, Customer> idVsCustomerMap = buildIdVsCustomerMap(customerList);

	// current score
	int currentScore = score(solution.ingridients, customerList);

	while (retry < 500) {
		retry += 1;

		// number of unused customers
		int N = solution.unusedCustomers.size();
		// generate random number between 0 to N-1
		int randomIndex = rand() % N;
		// get the customer belonging to randomIndex in unusedCustomers set
		unordered_set<int>::iterator it = solution.unusedCustomers.begin();
		advance(it, randomIndex);
		int customerId = *it;
		// selected random customer
		Customer& customer = idVsCustomerMap[customerId];

		unordered_set<string> currentIngridients = solution.ingridients;

		// remove dislike elements
		for (string& ing : customer.dislikedIngridients) {
			if (currentIngridients.find(ing) != currentIngridients.end()) {
				currentIngridients.erase(ing);
			}
		}

		// add like elements
		for (string& ing : customer.likedIngridients) {
			if (currentIngridients.find(ing) == currentIngridients.end()) {
				currentIngridients.insert(ing);
			}
		}

		unordered_set<int> newHappyCustomers;
		unordered_set<int> newUnusedCustomers;

		for (Customer& customer : customerList) {
			if (isHappyCustomer(customer, currentIngridients)) {
				newHappyCustomers.insert(customer.customerId);
			} else {
				newUnusedCustomers.insert(customer.customerId);
			}
		}

		if (newHappyCustomers.size() >= currentScore) {
			currentScore = newHappyCustomers.size();
			cout << "Found a better score: " << currentScore << endl;
			solution = {newHappyCustomers, newUnusedCustomers, currentIngridients};
		}
	}

	return solution;
}

int main() {
#ifndef ONLINE_JUDGE
	// for getting input from input.txt
	freopen("input.txt", "r", stdin);
	// for writing output to output.txt
	freopen("output.txt", "w", stdout);
	// for writing output to error.txt
	freopen("error.txt", "w", stderr);
#endif

	// stores customers information
	vector<Customer> customerList = prepareCustomerList();

	// ingridients set
	unordered_set<string> ingridientsSet = getIngridientsFromFile("output-1805");

	// initial solution
	Solution solution = initalizeSolution(customerList, ingridientsSet);

	solution = hillClimbingOptimization(solution, customerList);
	writeOutput(solution);
	return 0;
}




