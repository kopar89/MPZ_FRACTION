#include <iostream>
#include <gmp.h>
#include <string>

using namespace std;

class Fraction
{
private:
    mpz_t numer, denom;

    void normalize(mpz_t numer, mpz_t denom)
    {
        mpz_t gcd;
        mpz_init(gcd);

        mpz_gcd(gcd, numer, denom);
        mpz_divexact(numer, numer, gcd);
        mpz_divexact(denom, denom, gcd);

        if (mpz_sgn(denom) < 0)
        {
            mpz_neg(numer, numer);
            mpz_neg(denom, denom);
        }

        mpz_clear(gcd);
    }

public:

    Fraction()
    {
        mpz_init(numer);
        mpz_init(denom);
        mpz_set_ui(numer, 0);
        mpz_set_ui(denom, 1);
    }

    Fraction(const mpz_t n, const mpz_t d)
    {
        mpz_init(numer);
        mpz_init(denom);

        if (mpz_cmp_ui(d, 0) == 0)
        {
            cout << "Знаменатель не может быть равен нулю!\n";
            mpz_set_ui(numer, 0);
            mpz_set_ui(denom, 1);
        }
        else
        {
            mpz_set(numer, n);
            mpz_set(denom, d);
            normalize(numer, denom);
        }
    }

    Fraction& operator=(const Fraction& other) 
    {
        if (this != &other)
        {
            mpz_set(numer, other.numer);
            mpz_set(denom, other.denom);
        }
        return *this;
    }

    ~Fraction()
    {
        mpz_clear(numer);
        mpz_clear(denom);
    }

    
    Fraction operator+(Fraction& other) 
    {
        mpz_t n, d, t1, t2;
        mpz_inits(n, d, t1, t2, NULL);

        mpz_mul(t1, numer, other.denom);
        mpz_mul(t2, other.numer, denom);
        mpz_add(n, t1, t2);
        mpz_mul(d, denom, other.denom);

        Fraction result(n, d);
        return result;

        mpz_clears(n, d, t1, t2);
    }

    Fraction operator-(Fraction& other) 
    {
        mpz_t n, d, t1, t2;
        mpz_inits(n, d, t1, t2, NULL);

        mpz_mul(t1, numer, other.denom);
        mpz_mul(t2, other.numer, denom);
        mpz_sub(n, t1, t2);
        mpz_mul(d, denom, other.denom);

        Fraction result(n, d);
        return result;
        mpz_clears(n, d, t1, t2);
    }

    Fraction operator*(Fraction& other) 
    {
        mpz_t n, d;
        mpz_inits(n, d, NULL);

        mpz_mul(n, numer, other.numer);
        mpz_mul(d, denom, other.denom);

        Fraction result(n, d);
        return result;
        mpz_clears(n, d);
    }

    Fraction operator/(const Fraction& other) const
    {
        if (mpz_cmp_ui(other.numer, 0) == 0)
        {
            cout << "Ошибка: деление на ноль!\n";
            return Fraction();
        }

        mpz_t n, d;
        mpz_inits(n, d, NULL);

        mpz_mul(n, numer, other.denom);
        mpz_mul(d, denom, other.numer);

        Fraction result(n, d);
        return result;

        mpz_clears(n, d, NULL);
    }

    void print() const
    {
        gmp_printf("%Zd / %Zd\n", numer, denom);
    }
};

int main()
{
    setlocale(LC_ALL, "RUS");

    const int count = 3;
    Fraction arr[count];

    cout << "Введите значения для трех дробей:\n";

    for (int i = 0; i < count; i++)
    {
        mpz_t n, d;
        mpz_inits(n, d, NULL);

        string input;

        cout << "Введите числитель для " << i + 1 << " дроби: ";
        cin >> input;
        mpz_set_str(n, input.c_str(), 10);

        cout << "Введите знаменатель для " << i + 1 << " дроби: ";
        cin >> input;
        mpz_set_str(d, input.c_str(), 10);

        arr[i] = Fraction(n, d);

        mpz_clears(n, d, NULL);
    }

    Fraction result1 = arr[0] + arr[1];
    Fraction result2 = arr[0] - arr[2];
    Fraction total = result1 * result2;

    cout << "\nРезультат:\n";
    total.print();

    return 0;
}