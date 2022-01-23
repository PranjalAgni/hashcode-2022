#include <bits/stdc++.h>
using namespace std;

#define sz(x) ((int)(x).size())

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



// Local simulator function, which tells the score of generated output
int simulator(unordered_map<string, bool>& answerMap, vector<Customer>& customerList) {
	int score = 0;
	for (Customer& customer : customerList) {
		bool isHappy = true;
		for (string& ing : customer.likedIngridients) {
			if (answerMap.find(ing) == answerMap.end()) {
				isHappy = false;
				break;
			}
		}

		if (!isHappy) continue;

		for (string& ing : customer.dislikedIngridients) {
			if (answerMap.find(ing) != answerMap.end()) {
				isHappy = false;
				break;
			}
		}

		if (isHappy) score += 1;
	}

	// debug(score);
	return score;
}

// prints the output to the output.txt file
void printOutput(vector<string>& answer) {
	int N = answer.size();
	cout << N << " ";
	for (string& ingridient : answer) {
		cout << ingridient << " ";
	}
};


bool comparator(Customer& c1, Customer& c2) {
	return (sz(c1.dislikedIngridients) + sz(c1.likedIngridients)) < (sz(c2.dislikedIngridients) + sz(c2.likedIngridients));
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


	int attempts = 10000;
	int bestScore = INT_MIN;
	while (attempts-- > 0) {
		unordered_map<string, int> likedIngridientsMap;
		unordered_map<string, int> dislikedIngridientsMap;
		unordered_set<string> ingridientsSet;
		vector<string> optimalIngridients;
		unordered_map<string, bool> optimalIngridientsMap;


		int times = 1;
		while (times-- > 0) {
			random_shuffle(customerList.begin(), customerList.end());
		}

		for (Customer& customer : customerList) {
			bool isHappy = true;

			for (string& ingridient : customer.dislikedIngridients) {
				if (optimalIngridientsMap.find(ingridient) != optimalIngridientsMap.end()) {
					isHappy = false;
					break;
				}
			}

			if (isHappy) {
				for (string& ingridient : customer.likedIngridients) {
					if (optimalIngridientsMap.find(ingridient) == optimalIngridientsMap.end()) {
						optimalIngridients.push_back(ingridient);
						optimalIngridientsMap[ingridient] = true;
					}
				}
			}
		}

		int score = simulator(optimalIngridientsMap, customerList);
		bestScore = max(bestScore, score);
		// printOutput(optimalIngridients);
	}

	debug(bestScore);


	return 0;
}




