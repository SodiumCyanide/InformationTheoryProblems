#include <iostream>
#include <numeric>
#include <ratio>

#include <cstdint>
namespace coin
{
	struct rational
	{
		std::uint64_t numerator, denominator;
	};
	
	rational add(const rational& arg1, const rational& arg2)
	{
		rational result;
		result.numerator = arg1.numerator * arg2.denominator + arg1.denominator * arg2.numerator;
		result.denominator = arg1.denominator * arg2.denominator;
		std::uint64_t fac = std::gcd(result.numerator, result.denominator);
		result.numerator /= fac;
		result.denominator /= fac;
		return result;
	}

	rational subtract(const rational& arg1, const rational& arg2)
	{
		rational result;
		result.numerator = arg1.numerator * arg2.denominator - arg1.denominator * arg2.numerator;
		result.denominator = arg1.denominator * arg2.denominator;
		std::uint64_t fac = std::gcd(result.numerator, result.denominator);
		result.numerator /= fac;
		result.denominator /= fac;
		return result;
	}

	rational multiply(const rational& arg1, const rational& arg2)
	{
		rational result;
		result.numerator = arg1.numerator * arg2.numerator;
		result.denominator = arg1.denominator * arg2.denominator;
		std::uint64_t fac = std::gcd(result.numerator, result.denominator);
		result.numerator /= fac;
		result.denominator /= fac;
		return result;
	}

	rational divide(const rational& arg1, const rational& arg2)
	{
		rational result;
		result.numerator = arg1.numerator * arg2.denominator;
		result.denominator = arg1.denominator * arg2.numerator;
		std::uint64_t fac = std::gcd(result.numerator, result.denominator);
		result.numerator /= fac;
		result.denominator /= fac;
		return result;
	}

	bool less(const rational& arg1, const rational& arg2)
	{
		return double(arg1.numerator) / arg1.denominator < double(arg2.numerator) / arg2.denominator;
	}

	rational buffer[100000];  // enlarge it as you want
	rational unit;
	bool sign[100000];  // same for this

	rational answer(std::uint32_t N)
	{
		if(sign[N])
			return buffer[N];
		else
		{
			rational optimal, temp, prob;
			optimal.numerator = 0, optimal.denominator = 1;
			prob.numerator = 2, prob.denominator = N;
			optimal = add(optimal, add(multiply(prob, add(answer(1), unit)), multiply(subtract(unit, prob), add(answer(N - 2), unit))));
			for(std::uint32_t i = 2; i <= N / 2; ++i)
			{
				temp.numerator = 0, temp.denominator = 1;
				prob.numerator = 2 * i, prob.denominator = N;
				temp = add(temp, add(multiply(prob, add(answer(i), unit)), multiply(subtract(unit, prob), add(answer(N - 2 * i), unit))));
				if(less(temp, optimal))
					optimal = temp;
			}
			buffer[N] = optimal;
			sign[N] = true;
			return optimal;
		}
	}
}
int main()
{
	coin::unit.numerator = 1, coin::unit.denominator = 1;
	coin::buffer[0].numerator = 0, coin::buffer[0].denominator = 1;
	coin::buffer[1].numerator = 0, coin::buffer[1].denominator = 1;
	coin::buffer[2].numerator = 1, coin::buffer[2].denominator = 1;
	coin::sign[0] = coin::sign[1] = coin::sign[2] = true;
	coin::rational answer = {0,1};
	std::uint32_t quote;
	while(true)
	{
		std::cin >> quote;
		answer = coin::answer(quote);
		if(quote <= 1)
			std::cout << "For " << quote << " coin expected value is " << answer.numerator << '\n';
		else if(answer.denominator != 1)
			std::cout << "For " << quote << " coins expected value is " << answer.numerator << '/' << answer.denominator << '\n';
		else
			std::cout << "For " << quote << " coins expected value is " << answer.numerator << '\n';
	}
}