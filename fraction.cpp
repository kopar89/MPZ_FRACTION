#include <iostream>
#include <gmp.h>
#include <string>
using namespace std;

class Fraction
{
private:
    mpz_t numer, denom;
public:
    Fraction()
    {
        mpz_init(numer);
        mpz_init(denom);
        mpz_set_ui(numer, 0);
        mpz_set_ui(denom, 1);
    }

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

    Fraction(const Fraction& other)
    {
        mpz_init(numer);
        mpz_init(denom);
        mpz_set(numer, other.numer);
        mpz_set(denom, other.denom);
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
        mpz_clears(n, d, t1, t2, NULL);
        return result;
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
        mpz_clears(n, d, t1, t2, NULL);
        return result;
    }

    Fraction operator*(Fraction& other)
    {
        mpz_t n, d;
        mpz_inits(n, d, NULL);
        mpz_mul(n, numer, other.numer);
        mpz_mul(d, denom, other.denom);
        Fraction result(n, d);
        mpz_clears(n, d, NULL);
        return result;
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
        mpz_clears(n, d, NULL);
        return result;
    }

    void print() const
    {
        gmp_printf("%Zd / %Zd\n", numer, denom);
    }

    void print_decimal() const
    {
        mpz_t q, r, abs_r, abs_d, ten, digit;
        mpz_inits(q, r, abs_r, abs_d, ten, digit, NULL);

        mpz_tdiv_qr(q, r, numer, denom);
        mpz_abs(abs_r, r);
        mpz_abs(abs_d, denom);
        mpz_set_ui(ten, 10);

        bool negative = (mpz_sgn(numer) < 0) != (mpz_sgn(denom) < 0);
        if (negative && mpz_cmp_ui(q, 0) == 0 && mpz_cmp_ui(abs_r, 0) != 0)
            cout << "-";

        gmp_printf("%Zd", q);

        if (mpz_cmp_ui(abs_r, 0) != 0)
        {
            cout << ".";
            for (int i = 0; i < 20; i++)
            {
                mpz_mul_ui(abs_r, abs_r, 10);          
                mpz_tdiv_q(digit, abs_r, abs_d);       
                gmp_printf("%Zd", digit);
                mpz_submul(abs_r, digit, abs_d);        
                if (mpz_cmp_ui(abs_r, 0) == 0) break;   
            }
        }
        cout << "\n";

        mpz_clears(q, r, abs_r, abs_d, ten, digit, NULL);
    }

    void swap_fraction()
    {
        if (mpz_cmp_ui(this->numer, 0) == 0)
        {
            cout << "\nЗнаменатель не может быть равен нулю!\n";
        }
        else
        {
            mpz_t temp;
            mpz_init(temp);
            mpz_set(temp, this->numer);
            mpz_set(this->numer, denom);
            mpz_set(this->denom, temp);
            mpz_clear(temp);
            if (mpz_cmp_ui(this->numer, 0) != 0)
                cout << "Дробь успешно обращена!\n";
            else
                cout << "Знаменатель не может быть равен нулю!\n";
        }
    }

};

int main()
{
    setlocale(LC_ALL, "RUS");
    const int count = 3;
    Fraction arr[count];

    cout << "Введите три дроби\n";
    for (int i = 0; i < count; i++)
    {
        mpz_t n, d;
        mpz_inits(n, d, NULL);
        string input;

        cout << "\nДробь " << i + 1 << ":\n";
        cout << "  Числитель:   ";
        cin >> input;
        mpz_set_str(n, input.c_str(), 10);

        cout << "  Знаменатель: ";
        cin >> input;
        mpz_set_str(d, input.c_str(), 10);

        arr[i] = Fraction(n, d);
        mpz_clears(n, d, NULL);

        cout << "  Десятичный вид: ";
        arr[i].print_decimal();
    }

    Fraction result1 = arr[0] + arr[1];
    Fraction result2 = arr[0] - arr[2];
    Fraction total = result1 * result2;

    cout << "\nРезультат (обыкновенная дробь): ";
    total.print();
    cout << "Результат (десятичная): ";
    total.print_decimal();

    total.swap_fraction();

    cout << "После обращения (обыкновенная): ";
    total.print();
    cout << "После обращения (десятичная): ";
    total.print_decimal();

    return 0;
}
