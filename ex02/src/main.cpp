#include <cstring>
#include <iostream>
#include <vector>
#include <deque>
#include <sys/time.h>
#include <climits>
#include <algorithm>

unsigned long long int	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((long long int)(long long int)now.tv_sec * 1000000
		+ (long long int)now.tv_usec);
}

int	get_upper_jacobsthal(int nb)
{
	if (nb < 0)
		return 0;
	if (nb == 0)
		return 1;
	if (nb < 3)
		return 3;

	int jm2 = 1;
	int jm1 = 3;
	int tmp;

	while (nb >= (jm2 * 2) + jm1)
	{
		tmp = jm1;
		jm1 = jm1 + (2 * jm2);
		jm2 = tmp;
	}

	return ((jm2 * 2) + jm1);
}

bool	is_jacobsthal_number(int nb)
{

	if (nb < 5 && (nb == 0 || nb == 1 || nb == 3))
		return true;

	int jm2 = 1;
	int jm1 = 3;
	int tmp;

	while (nb > (jm2 * 2) + jm1)
	{
		tmp = jm1;
		jm1 = jm1 + (2 * jm2);
		jm2 = tmp;
	}

	if (nb == (jm2 * 2) + jm1)
		return true;
	return false;
}

int	get_next_jacobsthal_index(int last_index)
{

	if (!is_jacobsthal_number(last_index))
		return (last_index - 1);

	return (get_upper_jacobsthal(get_upper_jacobsthal(last_index)) - 1);

}

bool	is_even(int nb)
{
	if (nb % 2 == 0)
		return true;
	return false;
}

template <typename container>
void	move_element(container & sequence, int real_index, typename container::iterator to)
{
	typename container::iterator from = sequence.begin() + real_index;

	if (from == sequence.end() || to == sequence.end() || from == to)
		return;

	int tmp = *from;

	sequence.insert(to, tmp);
	sequence.erase(sequence.begin() + real_index + 1);

}

template <typename container>
int	get_index(container & sequence, typename container::iterator it)
{
	int i = 0;
	while (sequence.begin() + i != it)
		i++;
	return i;
}

template <typename container>
void	johnson(container & sequence, int range)
{
	int size = range;
	int middle = (range / 2); 

	if (range == 1)
		return;

	for (int i = 0; i < middle; i++)
	{
		if ((sequence[i] > sequence[i + middle]))
		{
			std::swap(sequence[i], sequence[i + middle]);

			while (size != (int)sequence.size())
			{
				std::swap(sequence[i + size], sequence[i + middle + size]);
				size += range;
			}

		}
	}

	//----------------------------------
	johnson(sequence, range / 2);
	//----------------------------------

	if (range == (int)sequence.size())
		return;

	typename container::iterator it;
	int loop = 0;
	int jacob_index = 0;
	int real_index = range * 2 - 1;
	int shift = 0;
	int upper = 0;
	int offset_right;

	int target = 0;

	while (loop < range)
	{

		if (loop != 0)
		{
			jacob_index = get_next_jacobsthal_index(jacob_index);
			while (jacob_index >= range)
			{
				shift++;
				jacob_index = get_next_jacobsthal_index(jacob_index);
			}
			upper = get_upper_jacobsthal(jacob_index);
			upper -= shift;
			real_index = (range * 2 - 1) - jacob_index - (upper - jacob_index - 1) + loop;
		}
		it = std::lower_bound(sequence.begin(), sequence.begin() + (range - 1) + loop, sequence[real_index]);
		offset_right = (*it < sequence[real_index]) ? 1 : 0;
		target = get_index(sequence, it);
		move_element(sequence, real_index, it + offset_right);
		size = range * 2;
		while (size != (int)sequence.size())
		{
			move_element(sequence, real_index + size, sequence.begin() + target + offset_right + size);
			size += range * 2;
		}
		loop++;
	}
}

int	get_next_power_two(int nb)
{
	if (nb < 2)
		return 0;

	int result = 2;

	while (result < nb)
		result *= 2;

	return result;
}

template <typename container>
void	display_container(container & sequence)
{
	for (typename container::iterator it = sequence.begin(); it != sequence.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

template <typename container>
bool is_sorted(container & sequence) {
	for (std::size_t i = 1; i < sequence.size(); ++i) {
		if (sequence[i-1] > sequence[i]) {
			return false;
		}
	}
	return true;
}

int	err_log(std::string str)
{
	std::cerr << str << std::endl;
	return 1;
}

bool	correct_characters(char *str)
{
	for (int i = 0; str[i]; i++)
		if (str[i] < '0' || str[i] > '9')
			return false;
	return true;
}

template <typename container>
void	timed_johnson(container & sequence, std::string container_type, char **argv)
{
	unsigned long long int start_time = get_time();

	for (int i = 1; argv[i]; i++)
		sequence.push_back(std::atoi(argv[i]));

	int saved_size = sequence.size();
	int next_power_two = get_next_power_two(sequence.size());

	while ((int)sequence.size() < next_power_two)
		sequence.push_back(INT_MAX);

	johnson(sequence, sequence.size());

	sequence.erase(sequence.begin() + saved_size, sequence.end());

	unsigned long long int processing_time = get_time() - start_time;

	std::cout << "Time to process a range of " << sequence.size() << " elements with " << container_type << " : " << processing_time << " us" << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc < 2 || argc > 50000)
		return (err_log("Error"));
	
	std::vector<int> vec;

	for (int i = 1; argv[i]; i++)
	{
		if (argv[i][0] == '\0')
			return (err_log("Error: Empty argument.")); 
		if (!correct_characters(argv[i]))
			return (err_log("Error: Bad input.")); 
		if (std::strtod(argv[i], NULL) > 2147483647)
			return (err_log("Error: Number exceed INT limitations.")); 
		if (std::atoi(argv[i]) < 0)
			return (err_log("Error: negative number.")); 

		vec.push_back(std::atoi(argv[i]));
	}

	if (vec.empty())
		return(err_log("Error."));
	
	if (vec.size() == 1)
		return(err_log("Can't sort only one element."));

	if (is_sorted(vec))
		return (err_log("Container already sorted."));

	std::cout << "Before:   ";
	display_container(vec);

	int saved_size = vec.size();
	int next_power_two = get_next_power_two(vec.size());

	while ((int)vec.size() < next_power_two)
		vec.push_back(INT_MAX);

	johnson(vec, vec.size());

	vec.erase(vec.begin() + saved_size, vec.end());

	std::cout << "After:   ";
	display_container(vec);

			//// Timer part	////

	// Time for std::vector processing
	std::vector<int> vec2;
	timed_johnson(vec2, "std::vector", argv);

	// Time for std::deque processing
	std::deque<int> deq;
	timed_johnson(deq, "std::deque", argv);

	return 0;
}
