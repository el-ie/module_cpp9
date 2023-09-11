#include <iostream>
#include <string>
#include <stack>

bool	is_operator(int c)
{
	if (c == '+' || c == '-')
		return true;
	if (c == '*' || c == '/')
		return true;
	return false;
}

int	error_message(void)
{
	std::cerr << "Error" << std::endl;
	return 1;
}

bool	check_input(std::string input)
{
	if (input.empty())
		return false;

	if (is_operator(input[0]) || !is_operator(input[input.size() - 1]))
		return false;
		
	int nb_count = 0;
	int operators_count = 0;

	for (int i = 0; input[i]; i++)
	{
		if (input[i] == ' ')
			continue;

		if ( (input[i] < '0' && input[i] > '9')
				&& (!is_operator(input[i])) )
			return false;
		else if (input[i] >= '0' && input[i] <= '9')
			nb_count++;
		else if (is_operator(input[i]))
			operators_count++;
		else
			return false;
	}
	
	if (nb_count < 2 || operators_count < 1)
		return false;

	if (nb_count != (operators_count + 1))
		return false;

	return true;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: bad number of arguments" << std::endl;
		return 1;
	}

	if (!check_input(argv[1]))
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	std::stack<double>	numbers;

	std::string		line = argv[1];

	double	nb1 = 0;

	for (int i = 0; line[i]; i++)
	{
		if (line[i] == ' ')
			continue;

		if (line[i] >= '0' && line[i] <= '9')
		{
			numbers.push(static_cast<double>(argv[1][i] - 48));
		}

		else if (is_operator(line[i]))
		{
			if (numbers.size() < 2)
				return error_message();

			nb1 = numbers.top();
			numbers.pop();

			if (line[i] == '+')
				nb1 += numbers.top();
			else if (line[i] == '-')
				nb1 = numbers.top() - nb1;
			else if (line[i] == '*')
				nb1 *= numbers.top();
			else if (line[i] == '/')
			{
				if (nb1 == 0)
				{
					std::cerr << "Error: Can't divide by 0." << std::endl;
					return 1;
				}
				nb1 = numbers.top() / nb1;
			}

			numbers.pop();
			if (line[i + 1])
				numbers.push(nb1);
		}
		else
			return error_message();
	}

	if (!numbers.empty())
		error_message();

	std::cout << nb1 << std::endl;

	return 0;
}
