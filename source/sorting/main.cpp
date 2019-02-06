// Data Structures and Algorithms 1
// Fraser Barker(1600196) Assessment Submission
// Based on the Lab 4 - Sorting by Adam Sampson <a.sampson@abertay.ac.uk>
// Comparison of QuickSort and MergeSort operating on large collections of a vector<int>
// QuickSort pseudocode - https://en.wikipedia.org/wiki/Quicksort (Utilising Hoare partition scheme)
// MergeSort pseudocode - https://en.wikipedia.org/wiki/Merge_sort (Utilising Top-down implementation)

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>

// Import things we need from the standard library
using std::cin;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::cout;
using std::endl;
using std::list;
using std::ofstream;
using std::sort;
using std::vector;
using std::iterator;

// Define the alias "the_clock" for the clock type we're going to use.
// (You can change this to make the code below use a different clock.)
typedef std::chrono::steady_clock the_clock;

// Put "count" random integers in the range (0, 99) into "collection".
// (This is templated so it'll work with any collection type that has push_back.)
template <typename T>
void MakeRandomValues(T& collection, int count) {
	for (int i = 0; i < count; i++) {
		collection.push_back(rand() % 100);
	}
}

// Print out all the items in "collection".
template <typename T>
void ShowValues(const T& collection) {
	for (auto item : collection) {
		cout << " " << item;
	}
	cout << "\n";
}

// Check that "collection" is in sorted order.
template <typename T>
void AssertSorted(const T& collection) {
	auto it = collection.begin();
	if (it == collection.end()) {
		// The collection is empty. So it's sorted!
		return;
	}

	auto prev = it;
	it++;
	while (it != collection.end()) {
		// Check that each pair of items is in order.
		assert(*it >= *prev);

		prev = it;
		it++;
	}

	// We've got to the end of the list. So it's sorted!
}

// Utilise MergeSort algorithm to sort the specified data set.
template <typename T>
auto MergeSort(T& collection) 
{
	vector<int>::iterator it = collection.begin();
	if (collection.size() <= 1)
	{
		//Collection is already sorted as only 0/1 elements in collection
		return collection;
	}
	else
	{
		vector<int> left;
		vector<int> right;
		for (int i = 0; i < collection.size(); i++)
		{
			if (i < (collection.size())/2)
			{
				left.push_back(*it);
				it++;
			}
			else
			{
				right.push_back(*it);
				it++;
			}
		}
		int l = left.front();
		int r = right.front();
		if (left.size() == 1 && right.size() == 1 && l < r)
		{
			return collection;
		}
		left = MergeSort(left);
		right = MergeSort(right);
		return Merge(left, right);
	}
}

// Component of MergeSort to merge the two split vectors into a complete ordered vector.
template <typename T>
auto Merge(T& collection, T& collection2)
{
	vector<int> result;
	vector<int> left = collection;
	vector<int> right = collection2;
	
	do
	{
		int t = left.front();
		int t2 = right.front();
		if (t <= t2)
		{
			result.push_back(t);
			left.erase(left.begin());
		}
		else
		{
			result.push_back(t2);
			right.erase(right.begin());
		}		
	} while (!left.empty() && !right.empty());

	if (!left.empty())
	{
		do
		{
			int t = left.front();
			result.push_back(t);
			left.erase(left.begin());
		} while (!left.empty());
	}

	if (!right.empty())
	{
		do
		{
			int t2 = right.front();
			result.push_back(t2);
			right.erase(right.begin());
		} while (!right.empty());
	}
	return result;
}

// Utilise QuickSort to sort the specified data set.
template <typename T>
void QuickSort(T& collection, int lo, int hi) 
{
	if (collection.size() <= 1)
	{
		//Collection is already sorted as only 0/1 elements in collection
		return;
	}
	//--==Hoare partition scheme==--//
	if (lo < hi)
	{
		int p = Partition(collection, lo, hi);	// Calculate partitioning of the collection.
		QuickSort(collection, lo, p);	// Quicksort lower half of collection (items less than the pivot point).
		QuickSort(collection, p + 1, hi);	// Quicksort upper half of collection (items greater than the pivot point).
	}
}

// Component of QuickSort to split the vector at a specified pivot point.
template <typename T>
auto Partition(T& collection, int lo, int hi)
{
	int pivot = collection[lo];
	int i = lo - 1, j = hi + 1;
	while (true)
	{
		do
		{
			i = i + 1;
		} while (collection[i] < pivot);

		do
		{
			j = j - 1;
		} while (collection[j] > pivot);

		if (i >= j)
		{
			return j;
		}
		swap(collection[i], collection[j]);
	}
}

// Component of QuickSort to swap values in the vector around the pivot point.
void swap(int &i, int &j)
{
	int temp = 0;
	temp = i;
	i = j;
	j = temp;
}

int main(int argc, char *argv[]) {
	int item = 10000, item2 = 10000;
	ofstream my_file("quickSortRelease.csv");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			srand((unsigned int)time(NULL));

			cout << "Sorting " << item << " integers using Quick Sort.\n";

			vector<int> input;
			MakeRandomValues(input, item);

			// Sort the list.
			vector<int> output = input;

			//Start timing
			the_clock::time_point start = the_clock::now();

			QuickSort(output, 0, output.size() - 1);

			// Stop timing
			the_clock::time_point end = the_clock::now();

			// Compute the difference between the two times in milliseconds
			auto time_taken = duration_cast<milliseconds>(end - start).count();

			// Check the vector is sorted
			AssertSorted(output);

			// Write time taken to order vector number of items in vector to a file
			my_file << item << "," << time_taken << endl;
		}
		item += 10000;
	}

	ofstream my_file2("mergeSortRelease.csv");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			srand((unsigned int)time(NULL));

			cout << "Sorting " << item2 << " integers using Merge Sort.\n";

			vector<int> input;
			MakeRandomValues(input, item2);

			// Sort the list.
			vector<int> output = input;

			//Start timing
			the_clock::time_point start = the_clock::now();

			output = MergeSort(output);

			// Stop timing
			the_clock::time_point end = the_clock::now();

			// Compute the difference between the two times in milliseconds
			auto time_taken = duration_cast<milliseconds>(end - start).count();

			// Check the vector is sorted
			AssertSorted(output);

			// Write time taken to order vector number of items in vector to a file
			my_file2 << item2 << "," << time_taken << endl;
		}
		item2 += 10000;
	}
		cout << "All OK! Press return to exit...\n";
		cin.get();

		return 0;
}