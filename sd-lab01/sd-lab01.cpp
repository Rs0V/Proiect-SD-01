#include <iostream>
#include <vector>
#include <chrono>
#include <time.h>
#include <random>

#define FOREX(i, _pair) for(auto i = _pair.first; i < _pair.second; ++i)
#define FOR(i, _pair) for(auto i = _pair.first; i <= _pair.second; ++i)

#define NPAIR(_pair, f, s, _pair_name) auto _pair_name = std::pair<int, int>(_pair.first + f, _pair.second + s)

std::random_device rd;
std::mt19937 rng(rd());

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

int* rand_uint_array(int seed, int array_size)
{
	srand(seed);
	int* _array = new int[array_size];
	int i = 0;
	while (i < array_size)
	{
		_array[i] = rand();
		++i;
	}
	return _array;
}

void insertion_sort(int* _array, std::pair<int, int> _interval)
{
	NPAIR(_interval, 1, 0, interv);
	FOREX(key, interv)
	{
		int i = key, j = key - 1;
		while (_array[i] < _array[j] && j > -1)
		{
			_array[i] ^= _array[j], _array[j] ^= _array[i], _array[i] ^= _array[j];
			--i, --j;
		}
	}
}

void count_sort(int* _array, std::pair<int, int> _interval)
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

void radix_sort_aux(int* _array, std::pair<int, int> _interval, int max, int digit = 1)
{
	if (digit < 1)
		return;

	std::vector<std::vector<int>> freq;
	freq.resize(10);
	FOREX(i, _interval)
		freq[_array[i] / digit % 10].push_back(_array[i]);

	int array_index = 0;
	for (int i = 0; i < 10; ++i)
	{
		for (auto number = freq[i].begin(); number != freq[i].end(); ++number)
		{
			_array[array_index] = *number;
			++array_index;
		}
	}
	radix_sort_aux(_array, _interval, max - 1, digit * 10);
}

void radix_sort(int* _array, std::pair<int, int> _interval)
{
	radix_sort_aux(_array, _interval, digit_count(array_max(_array, _interval)));
}

// ne mai gandim =))
void merge_sort_aux(int* _array, std::pair<int, int> _interval)
{
	if (_interval.second - _interval.first > 1)
	{
		int mid = (_interval.first + _interval.second) / 2;
		merge_sort_aux(_array, { _interval.first, mid });
		merge_sort_aux(_array, { mid + 1,_interval.second });
		
		/*
		int i = _interval.first, j = mid + 1, iadd = 1, jadd = 0;
		while (i < j && j <= _interval.second)
		{
			if (_array[i] > _array[j])
			{
				_array[i] ^= _array[j], _array[j] ^= _array[i], _array[i] ^= _array[j];
				i += iadd, j += jadd;
				iadd ^= jadd, jadd ^= iadd, iadd ^= jadd;
			}
			else
				++i;
		}
		*/

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
	{
		_array[_interval.first] ^= _array[_interval.second];
		_array[_interval.second] ^= _array[_interval.first];
		_array[_interval.first] ^= _array[_interval.second];
	}
}

void merge_sort(int* _array, std::pair<int, int> _interval)
{
	merge_sort_aux(_array, { _interval.first, _interval.second - 1 });
}

void quick_sort(int* _array, std::pair<int, int> _interval)
{
	if (_interval.second - _interval.first == 1)
		return;

	std::uniform_int_distribution<std::mt19937::result_type> dist(_interval.first, _interval.second);

	int p[3] = { dist(rng), dist(rng), dist(rng) };
	int pivot = (p[0] + p[1] + p[2]) / 3;
	pivot = _array[pivot];

	int* l = new int[_interval.second - _interval.first];
	int* g = new int[_interval.second - _interval.first];
	int li = 0, gi = 0;
	FOREX(i, _interval)
	{
		if (_array[i] < pivot)
			l[li] = _array[i], ++li;
		else if (_array[i] > pivot)
			g[gi] = _array[i], ++gi;
	}

	int k = 0;
	if (li > 0)
	{
		quick_sort(l, { 0, li });
		for (int i = 0; i < li; ++i)
			_array[k] = l[i], ++k;
	}
	_array[k] = pivot, ++k;
	if (gi > 0)
	{
		quick_sort(g, { 0, gi });
		for (int i = 0; i < gi; ++i)
			_array[k] = g[i], ++k;
	}
	delete[] l;
	delete[] g;
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

int main()
{
	int a_len = 8;
	int* a = rand_uint_array(1, a_len);
	
	auto t0 = std::chrono::system_clock::now();
	shell_sort(a, { 0, a_len });
	auto t1 = std::chrono::system_clock::now();

	for (int i = 0; i < a_len; ++i)
		std::printf("%d, ", a[i]);
	delete[] a;

	std::chrono::duration<double> time = t1 - t0;
	std::cout << "\n\n" << time.count() << '\n';

	return 0;
}

// infoarena - pb. AlgSort