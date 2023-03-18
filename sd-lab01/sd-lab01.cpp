#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <string>

#define FOREX(i, _pair) for(auto i = _pair.first; i < _pair.second; ++i)
#define FOR(i, _pair) for(auto i = _pair.first; i <= _pair.second; ++i)

#define RFOREX(i, _pair) for(auto i = _pair.second - 1; i >= _pair.first; --i)
#define RFOR(i, _pair) for(auto i = _pair.second; i >= _pair.first; --i)

#define NPAIR(_pair, f, s, _new_pair) auto _new_pair = std::pair<int, int>(_pair.first + f, _pair.second + s)

#define SORT_PTR(sort_ptr) void (*sort_ptr)(int*, std::pair<int, int>)



std::random_device rd;
std::mt19937 rng(rd());

std::ifstream input("input.in");
std::ofstream output("output.out");



int array_max(int* _array, std::pair<int, int> _interval)
{
	int max = 0;
	FOREX(i, _interval)
		if (_array[i] > max)
			max = _array[i];
	return max;
}

int digit_count(int number)
{
	int digits = 0;
	while (number)
	{
		++digits;
		number /= 10;
	}
	return digits;
}

int* rand_uint_array(int array_size, int max_nr)
{
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, max_nr);

	int* _array = new int[array_size];
	int i = 0;
	while (i < array_size)
	{
		_array[i] = dist(rng);
		++i;
	}
	return _array;
}

int* create_array_copy(int* _array, int array_size)
{
	int* new_array = new int[array_size];
	for(int i = 0; i < array_size; ++i)
		new_array[i] = _array[i];
	return new_array;
}

bool is_sorted(int* _array, std::pair<int, int> _interval)
{
	NPAIR(_interval, 1, 0, interv);
	FOREX(i, interv)
		if (_array[i - 1] > _array[i])
			return false;
	return true;
}



void count_sort(int* _array, std::pair<int, int>_interval)
{
	int max = array_max(_array, _interval);
	++max;
	int* count_array = new int[max] {0};

	FOREX(i, _interval)
		++count_array[_array[i]];

	int array_index = 0;
	for (int i = 0; i < max; ++i)
	{
		while (count_array[i] > 0)
		{
			_array[array_index] = i;
			--count_array[i];
			++array_index;
		}
	}
	delete[] count_array;
}



void radix_sort_aux(int* _array, std::pair<int, int>_interval, int exp)
{
	int* output = new int[_interval.second - _interval.first];
	int count[10]{ 0 };

	FOREX(i, _interval)
		++count[_array[i] / exp % 10];

	for (int i = 1; i < 10; ++i)
		count[i] += count[i - 1];

	RFOREX(i, _interval)
	{
		--count[_array[i] / exp % 10];
		output[count[_array[i] / exp % 10]] = _array[i];
	}

	FOREX(i, _interval)
		_array[i] = output[i];

	delete[] output;
}

void radix_sort(int* _array, std::pair<int, int>_interval)
{
	int max = array_max(_array, _interval);
	for (int exp = 1; max / exp > 0; exp *= 10)
		radix_sort_aux(_array, _interval, exp);
}



void merge_sort_aux(int* _array, std::pair<int, int> _interval)
{
	if (_interval.second - _interval.first > 1)
	{
		int mid = (_interval.first + _interval.second) / 2;
		merge_sort_aux(_array, { _interval.first, mid });
		merge_sort_aux(_array, { mid + 1,_interval.second });

		int* aux = new int[_interval.second - _interval.first + 1];
		{
			int i = _interval.first, j = mid + 1, k = 0;
			while (i <= mid && j <= _interval.second)
			{
				if (_array[i] < _array[j])
					aux[k] = _array[i], ++i;
				else
					aux[k] = _array[j], ++j;
				++k;
			}
			while (i <= mid)
			{
				aux[k] = _array[i];
				++i, ++k;
			}
			while (j <= _interval.second)
			{
				aux[k] = _array[j];
				++j, ++k;
			}
		}
		FOR(i, _interval)
			_array[i] = aux[i - _interval.first];
		delete[] aux;
	}
	else if (_array[_interval.first] > _array[_interval.second])
		std::swap(_array[_interval.first], _array[_interval.second]);
}

void merge_sort(int* _array, std::pair<int, int> _interval)
{
	merge_sort_aux(_array, { _interval.first, _interval.second - 1 });
}



void shell_sort(int* _array, std::pair<int, int> _interval)
{
	int n = _interval.second - _interval.first;
	for (int gap = n / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < n; ++i)
		{
			int temp = _array[i];
			int j;
			for (j = i; j >= gap && _array[j - gap] > temp; j -= gap)
				_array[j] = _array[j - gap];
			_array[j] = temp;
		}
	}
}



int partition(int* _array, int low, int high)
{
	int pivot = _array[high];
	int i = low - 1;

	for (int j = low; j <= high - 1; ++j)
		if (_array[j] < pivot)
		{
			++i;
			i = std::min(i, high);
			std::swap(_array[i], _array[j]);
		}
	std::swap(_array[std::min(i + 1, high)], _array[high]);
	
	return i + 1;
}

void quick_sort_aux(int* _array, std::pair<int, int> _interval)
{
	if (_interval.first < _interval.second) {
		int pi = partition(_array, _interval.first, _interval.second);

		quick_sort_aux(_array, { _interval.first, pi - 1 });
		quick_sort_aux(_array, { pi + 1, _interval.second });
	}
}

void quick_sort(int* _array, std::pair<int, int> _interval)
{
	quick_sort_aux(_array, { _interval.first, _interval.second - 1 });
}



int main()
{
	int tests, array_size, max_nr;
	input >> tests;

	for (int i = 0; i < tests; ++i)
	{
		input >> array_size >> max_nr;
		int* a = rand_uint_array(array_size, max_nr);

		SORT_PTR(sorts[]) = { radix_sort, merge_sort, shell_sort, count_sort, quick_sort };
		std::string sorts_names[] = {"radix_sort", "merge_sort", "shell_sort", "count_sort", "quick_sort" };
		
		output << 'T' << i + 1 << ":\n";
		int nr_sorts = sizeof(sorts) / sizeof(SORT_PTR());
		for (int j = 0; j < nr_sorts; ++j)
		{
			int* a_copy = create_array_copy(a, array_size);
			
			auto t0 = std::chrono::system_clock::now();
			sorts[j](a_copy, { 0, array_size });
			auto t1 = std::chrono::system_clock::now();

			output << '\t' << sorts_names[j] << ": ";
			if (is_sorted(a_copy, { 0, array_size }))
			{
				std::chrono::duration<double> time = t1 - t0;
				output << time.count() << "s\n";
			}
			else
				output << "Sort Failed\n";

			std::cout << sorts_names[j] << ": finished!\n\n";

			delete[] a_copy;
		}
		output << '\n';
		std::cout << 'T' << i + 1 << ": finished!\n\n\n\n";

		delete[] a;
	}
	return 0;
}