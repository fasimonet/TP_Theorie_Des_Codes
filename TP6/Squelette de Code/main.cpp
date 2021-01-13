//
//  TP6_RSA
//

#include <stdio.h>
#include <iostream>
#include <gmp.h>
#include <cmath>
#include <cstdarg>

#define BITSTRENGTH 14              /* size of modulus (n) in bits */
#define PRIMESIZE (BITSTRENGTH / 2) /* size of the primes p and q  */

/* Declare global variables */
mpz_t d, e, n, M, c;


//////////////////////////////////////////////
//             Add mpz functions            //
//////////////////////////////////////////////

void inits(int count...)
{
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i)
        mpz_init( *(va_arg(args, mpz_t*)) );

    va_end(args);
}

void clears(int count...)
{
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i)
        mpz_clear( *(va_arg(args, mpz_t*)) );

    va_end(args);
}

std::string mpz_str(const mpz_t other)
{
    char str[1000];

    mpz_get_str(str, 10, other);

    return str;
}


//////////////////////////////////////////////
//          Redefine mpz functions          //
//////////////////////////////////////////////

void exponentiation_by_squaring(mpz_t &m, const mpz_t &g, const mpz_t &k, const mpz_t &p)
{
    mpz_t g_tmp, k_tmp, p_tmp;
    inits(3, &g_tmp, &k_tmp, &p_tmp);
    mpz_set(g_tmp, g);
    mpz_set(k_tmp, k);
    mpz_set(p_tmp, p);

    if (mpz_cmp_si(k_tmp, 0) < 0) // si k < 0
    {
        mpz_t one;
        inits(1, &one);
        mpz_set_ui(one, 1);

        mpz_fdiv_q(g_tmp, one, g_tmp); // g = 1 / g
        mpz_mul_si(k_tmp, k_tmp, -1);  // k = k * (-1)
        mpz_clear(one);
    }

    if (mpz_cmp_si(k_tmp, 0) == 0)
    {
        mpz_set_ui(m, 1);
        return;
    }

    mpz_t y;
    inits(1, &y);
    mpz_set_ui(y, 1);

    while (mpz_cmp_si(k_tmp, 1) > 0) // while k > 1
    {
        if (mpz_even_p(k_tmp) != 0) // if k is even
        {
            mpz_mul(g_tmp, g_tmp, g_tmp);   // g = g * g
            mpz_mod(g_tmp, g_tmp, p_tmp);   // g = g mod p
            mpz_fdiv_q_ui(k_tmp, k_tmp, 2); // k = k / 2
        }
        else
        {
            mpz_mul(y, g_tmp, y);         // y = g * y
            mpz_mul(g_tmp, g_tmp, g_tmp); // g = g * g

            // k = (k-1) / 2
            mpz_sub_ui(k_tmp, k_tmp, 1);
            mpz_fdiv_q_ui(k_tmp, k_tmp, 2);
        }
    }

    mpz_mul(m, g_tmp, y); // m = g * y
    mpz_mod(m, m, p_tmp); // m = m [p]
}

void rabin_miller_step_1_dif(mpz_t &s, mpz_t &t, const mpz_t &n)
{
    mpz_t local_n;

    inits(1, &local_n);

    mpz_set(t, n);
    mpz_sub_ui(t, t, 1);
    mpz_set_ui(s, 0);

    while (mpz_even_p(t))
    {
        mpz_fdiv_q_ui(t, t, 2);
        mpz_add_ui(s, s, 1);
    }
}

bool rabin_miller(int k, const mpz_t &n)
{
    // Declare variables
    gmp_randstate_t state;
    mpz_t t, s, a, x, r, tmp_oprd, seed;

    // Init variables
    inits(6, &t, &s, &a, &x, &r, &tmp_oprd);

    rabin_miller_step_1_dif(s, t, n);
    if (mpz_get_si(n) <= 2 && mpz_odd_p(n))
        return false;

    // Init state
    mpz_init_set_str(seed, std::to_string(1000 + rand() % 100000).c_str(), 0);
    gmp_randinit_default(state);
    gmp_randseed(state, seed);

    for (int i = 0; i < k; ++i)
    {
        mpz_sub_ui(tmp_oprd, n, 2);
        mpz_urandomm(tmp_oprd, state, tmp_oprd);
        mpz_add_ui(a, tmp_oprd, 2);
        mpz_powm(x, a, t, n);
        mpz_sub_ui(tmp_oprd, n, 1);

        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, tmp_oprd) == 0)
            goto loop;

        for (mpz_set_ui(r, 1); mpz_cmp(r, s) < 0; mpz_add_ui(r, r, 1))
        {
            // x = (x * x) % n;
            mpz_mul(x, x, x);
            mpz_mod(x, x, n);

            if (mpz_cmp_ui(x, 1) == 0)
                return false;

            mpz_sub_ui(tmp_oprd, n, 1);
            
            if (mpz_cmp(x, tmp_oprd) == 0)
                goto loop;
        }
        return false;

    loop:
        continue;
    }

    return true;
}

void nextprime(mpz_t &rop)
{
    mpz_t tmp;
    mpz_set(tmp, rop);

    if (mpz_even_p(rop) != 0) // If op is even
        mpz_add_ui(tmp, rop, 1);
    
    while (rabin_miller(100000, tmp) != true)
        mpz_add_ui(tmp, tmp, 2);

    mpz_set(rop, tmp);
}


//////////////////////////////////////////////
//               RSA functions              //
//////////////////////////////////////////////

/*
 *  Generate two large distinct primes p and q randomly
 */
void generatePrimes(mpz_t &p, mpz_t &q, mpz_t &seed, gmp_randstate_t &state) {
    mpz_init_set_str(seed, std::to_string(1000 + rand() % 100000).c_str(), 0);

    gmp_randinit_default(state);
    gmp_randseed(state, seed);

    mpz_urandomm(p, state, seed);
    mpz_urandomm(q, state, seed);

    nextprime(p);
    nextprime(q);
}

/*
 *  Calculate n = pq and x = (p-1)(q-1)
 */
void calculateNX(mpz_t &n, mpz_t &x, mpz_t &p, mpz_t &q) {
    mpz_t p_minus_1, q_minus_1;
    inits(2, &p_minus_1, &q_minus_1);

    // Calculate n
    mpz_mul(n, p, q);
    std::cout << "    n  = " << mpz_str(n) << std::endl;

    // Calculate x
    mpz_sub_ui(p_minus_1, p, (unsigned long int)1);
    mpz_sub_ui(q_minus_1, q, (unsigned long int)1);

    mpz_mul(x, p_minus_1, q_minus_1);
    std::cout << "phi(n) = " << mpz_str(x) << std::endl;

    clears(2, &p_minus_1, &q_minus_1);
}

/*
 *  Select a random integer e (1<e<x) such that gcd(e,x) = 1
 */
void calculateE(mpz_t &e, mpz_t &x, mpz_t &seed, gmp_randstate_t &state) {
    mpz_t e_tmp, pgcd;
    inits(2, &e_tmp, &pgcd);

    do
    {
        mpz_init_set_str(seed, std::to_string(rand()).c_str(), 0);

        gmp_randinit_default(state);
        gmp_randseed(state, seed);

        mpz_urandomm(e_tmp, state, seed);
        mpz_init_set_str(e, std::to_string(mpz_get_ui(e_tmp) % mpz_get_ui(x)).c_str(), 0);

        mpz_gcd(pgcd, e, x);
    } while (mpz_get_ui(pgcd) != 1);
    
    std::cout << "    e  = " << mpz_str(e) << std::endl;

    clears(2, &e_tmp, &pgcd);
}

/*
 *  Calculate the unique d such that ed = 1(mod x)
 */
void calculateUniqueD(mpz_t &d, mpz_t &e, mpz_t &x) {
    if (mpz_invert(d, e, x) == 0)
        std::cout << "Error while inversing e mod x" << std::endl;

    std::cout << "    d  = " << mpz_str(d) << std::endl;
}

/*
 *  Print Public key pair : (e,n) and Private key pair : (d,n)
 */
void printKeys(mpz_t &e, mpz_t &d, mpz_t &n) {
    std::cout << "Public  Keys (e,n): ( " << mpz_str(e) << " , " << mpz_str(n) << " )" << std::endl;
    std::cout << "Private Keys (d,n): ( " << mpz_str(d) << " , " << mpz_str(n) << " )" << std::endl;
}

void createKeys(mpz_t &e, mpz_t &d, mpz_t &n) {
    gmp_randstate_t state;
    mpz_t p, q, seed, x;

    // Initialize the local GMP integers
    inits(4, &p, &q, &seed, &x);

    // Step 1 : Generate two large distinct primes p and q randomly
    std::cout << "###################" << std::endl;
    generatePrimes(p, q, seed, state);

    // Step 2 : Calculate n = pq and x = (p-1)(q-1)
    calculateNX(n, x, p, q);

    // Step 3 : Select a random integer e (1<e<x) such that gcd(e,x) = 1
    calculateE(e, x, seed, state);

    // Step 4 : Calculate the unique d such that ed = 1(mod x)
    calculateUniqueD(d, e, x);
    std::cout << "###################" << std::endl;

    // Step 5 : Public key pair : (e,n), Private key pair : (d,n)
    std::cout << std::endl << "###############################################" << std::endl;
    printKeys(e, d, n);
    std::cout << "###############################################" << std::endl;

    // Clean up the local GMP integers
    clears(4, &p, &q, &seed, &x);
}

void rsa_cipher(mpz_t &k, mpz_t &n, std::string prefix = "") {
    mpz_t before, after;

    // Initialize the local GMP integers
    inits(2, &before, &after);

    // Scan message
    char in[1000];
    std::cout << std::endl << "################################" << std::endl;
    std::cout << "Message à " << prefix << "chiffrer   : ";
    std::cin >> in;
    mpz_set_str(before, in, 10);

    // Cipher message
    exponentiation_by_squaring(after, before, k, n);

    // Display message
    std::cout << "Message " << prefix << "chiffré      : " << mpz_str(after) << std::endl;
    std::cout << "################################" << std::endl;

    // Clean up the local GMP integers
    clears(2, &before, &after);
}

int main()
{
    // Initialize the gobal GMP integers
    inits(5, &d, &e, &n, &M, &c);

    // Initialize random time
    srand(time(NULL));

    // Create RSA keys
    createKeys(e, d, n);

    // Encrypt
    rsa_cipher(e, n);

    // Decrypt
    rsa_cipher(d, n, "dé");

    // Clean up the gobal GMP integers
    clears(5, &d, &e, &n, &M, &c);
}
