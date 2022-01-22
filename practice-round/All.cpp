#include <bits/stdc++.h>
using namespace std;

const int DISLIKE_THRESHOLD = 5;


struct Customer {
	int customerId;
	vector<string> likedIngridients;
	vector <string> dislikedIngridients;
};

// Local simulator function, which tells the score of generated output
void simulator(unordered_map<string, bool>& answerMap, vector<Customer>& customerList) {
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

	cout << "Score = " << score << endl;
}

// prints the output to the output.txt file
void printOutput(vector<string>& answer) {
	int N = answer.size();
	cout << N << " ";
	for (string& ingridient : answer) {
		cout << ingridient << " ";
	}
};

int main() {
#ifndef ONLINE_JUDGE
	// for getting input from input.txt
	freopen("input.txt", "r", stdin);
	// for writing output to output.txt
	freopen("output.txt", "w", stdout);
#endif

	int customers;
	cin >> customers;
	vector<Customer> customerList(customers);

	for (int customerIdx = 0; customerIdx < customers; customerIdx++) {
		int numLike, numDisLike;
		cin >> numLike;
		vector<string> likeIngridients(numLike);



		for (int idx = 0; idx < numLike; idx++) {
			cin >> likeIngridients[idx];
		}

		cin >> numDisLike;
		vector<string> dislikeIngridients(numDisLike);

		for (int idx = 0; idx < numDisLike; idx++) {
			cin >> dislikeIngridients[idx];
		}

		struct Customer customer = {customerIdx, likeIngridients, dislikeIngridients};
		customerList[customerIdx] = customer;
	}

	unordered_map<string, int> likedIngridientsMap;
	unordered_map<string, int> dislikedIngridientsMap;
	unordered_set<string> ingridientsSet;
	vector<string> optimalIngridients;
	unordered_map<string, bool> optimalIngridientsMap;

	for (Customer& customer : customerList) {
		for (string& ingridient : customer.likedIngridients) {
			ingridientsSet.insert(ingridient);
			likedIngridientsMap[ingridient] += 1;
		}

		for (string& ingridient : customer.dislikedIngridients) {
			ingridientsSet.insert(ingridient);
			dislikedIngridientsMap[ingridient] += 1;
		}
	}


	// hashmap solution
	for (auto& ingridient : ingridientsSet) {
		int likes = likedIngridientsMap[ingridient];
		int disLikes = dislikedIngridientsMap[ingridient];
		if (likes >= disLikes) {
			optimalIngridients.push_back(ingridient);
			optimalIngridientsMap[ingridient] = true;
		} else if (disLikes <= DISLIKE_THRESHOLD) {
			optimalIngridients.push_back(ingridient);
			optimalIngridientsMap[ingridient] = true;
		}
	}

	simulator(optimalIngridientsMap, customerList);
	printOutput(optimalIngridients);

	return 0;
}




