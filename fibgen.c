// Generates data for a Fibonacci heap

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <getopt.h>

#define MAX_LEN (2000000)
#define MIN_LEN (100000)
#define DIF_LEN (100000)
#define BIAS (1000)
#define NAIVE_MAX (25)
#define NAIVE_MIN (1)
#define NAIVE_DIF (1)

/*
 * This is the xoroshiro128+ random generator, designed in 2016 by David Blackman
 * and Sebastiano Vigna, distributed under the CC-0 license. For more details,
 * see http://vigna.di.unimi.it/xorshift/.
 */
static uint64_t rng_state[2];

static uint64_t rng_rotl(const uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

static uint64_t rng_next_u64(void)
{
	uint64_t s0 = rng_state[0], s1 = rng_state[1];
	uint64_t result = s0 + s1;
	s1 ^= s0;
	rng_state[0] = rng_rotl(s0, 55) ^ s1 ^ (s1 << 14);
	rng_state[1] = rng_rotl(s1, 36);
	return result;
}

static uint32_t rng_next_u32(void)
{
	return rng_next_u64() >> 11;
}

static void rng_setup(unsigned int seed)
{
	rng_state[0] = seed * 0xdeadbeef;
	rng_state[1] = seed ^ 0xc0de1234;
	for (int i=0; i<100; i++)
		rng_next_u64();
}

/*** The RNG ends here ***/

static void usage(void)
{
	fprintf(stderr, "Usage: heapgen [-s <student-id>] [-r] [-b] [-x]\n");
	exit(1);
}

/* Consolidate into one tree with high order and restore the heap structure after that */
static void expensive_loop(int loops)
{
	for (int i=0; i<loops; i++) {
		printf("I 1 1\n");
		printf("I 2 2\n");
		printf("M\n");
		printf("M\n");
	}
}

/* Construct a star with n nodes and root index r */
static void star(int n, int r, bool consolidate, int c, int N)
{
    if (n == 1) {
	    // Add a single node
	    printf("I %d %d\n",r,r);

	    // Consolidate everything if this is the second star of size 1 added
	    if (consolidate) {
		    printf("I 1 1\n");
		    printf("M\n");
	    }
    } else {
	    // Build two stars with n-1 vertices and consolidate them
	    star(n-1,r,false,0,N);
	    star(n-1,r+n-1,true,c+1,N);

	    // Delete unnecessary parts
	    for (int l = r+n; l < r+2*n-2; l++) {
		    printf("D %d %d\n", l, 1);
		    printf("M\n");
	    }
    }
}

/*
 * generates a sequence on which non-cascading heaps need lots of time
 * source: "Replacing Mark Bits with Randomness in Fibonacci Heaps" Jerry Li and John Peebles, MIT
 * -> modification so that only a quadratic number of elements are needed in the star construction
 */
static void ncascade_gen_mod(void)
{
	int nr, from;

	for (int N=NAIVE_MIN; N<=NAIVE_MAX; N+=NAIVE_DIF) {
		from = 3; 		// Reserve two vertices with smaller keys than any other
		nr = from + N*(N+1)/2;	// The number of needed elements

		printf("# %i\n",nr);

		// Construct N stars of size n
		for (int n=N; n>=1; n--) {
			star(n,from,false,0,N);
			from+=n;
		}

		expensive_loop(10*(1<<N));
	}
}

/* generates a sequence with a bias towards fewer delete-mins
 * E[nr of decrease keys] = 2 * E[nr of inserts]
 * E[delete]= 0.6 * E[nr of inserts] / (bias+1)
 * bias=0: totally random, according to above expected values
 */
static void random_gen(int bias)
{
	int *a = malloc(sizeof(int) * (MAX_LEN+1));
	int next, op, j, nr_del, nr_dec, num_elts;

	for (int length = MIN_LEN; length <= MAX_LEN; length += DIF_LEN) {
		for (int i=0; i<length; i++) // Initialize some random elements to insert
			a[i] = rng_next_u32() % length;

		printf("# %d\n",length);
		next = 0; // Next element to insert
		nr_del = floor(length/(bias+1.6));
		nr_dec = floor(2*length+5.2*nr_del);

		while (next < 10) { // Insert the first few elements to have something to work with
			printf("I %d %d\n", next, a[next]);
			next++;
		}
		num_elts = next;

		while (next < length) {
			op = rng_next_u32() % (length+nr_dec+nr_del); // Choose an operation at random

			if (op < length) { // Insert the next element
				printf("I %d %d\n", next, a[next]);
				next++;
				num_elts++;
			} else if (op < length+nr_dec) { // Decrease the key of some element
				j = rng_next_u32() % next;
				if (a[j] > 0) { // Key of element can be decreased
					a[j] -= (rng_next_u32() % next) + 1; // Subtract some positive number
					if (a[j] < 0) a[j]=0;
					printf("D %d %d\n", j, a[j]);
				}
			} else { // Delete the minimum
				if (num_elts) {
					printf("M\n");
					num_elts--;
				}
			}
		}
	}
}

void path_gen() {
	for(int size = 8; size < (1<<24); size *= 3) {
		printf("# %d\n", size);

		printf("I %d %d\n", size-1, 2*size-2);
		printf("I 1 %d\n", 2*size);
		printf("I 4 0\nM\n");
		printf("I %d %d\n", size-2, 2*size-4);
		printf("I 0 %d\n", 2*size);
		printf("I 4 0\nM\n");
		printf("I 2 %d\n", 2*size);
		printf("I 3 %d\n", 2*size+1);
		printf("I 4 0\nM\n");
		int inserts = 7;

		for(int i = size-3; i > 5; i-=2) {
			printf("D 1 %d\n", 2*i+3);
			printf("I %d %d\n", i, 2*i);
			printf("I 4 0\nM\n");
			printf("D 2 %d\n", 2*i-1);

			printf("D 0 %d\n", 2*i+1);
			printf("I %d %d\n", i-1, 2*i-2);
			printf("I 4 0\nM\n");
			printf("D 2 %d\n", 2*i-3);
			inserts += 2;
		}

		printf("D %d %d\n", size-1, 0);
		for(int i = 0; i < inserts; i++)
			printf("M\n");

		for(int b = 0; b < 2; b++) {
			printf("I 0 1\n");
			printf("I %d %d\n", size-1, size-1);
			inserts = 2;

			for(int i = size-2; i > 2; i-=2) {
				printf("D 0 0\n");
				printf("I %d %d\n", i, i);
				printf("I 1 %d\n", size);
				printf("M\n");
				printf("D 1 0\n");
				printf("I %d %d\n", i-1, i-1);
				printf("I 0 %d\n", size);
				printf("M\n");
				inserts += 2;
			}

			if(b == 0)
			{
				printf("D %d %d\n", size-1, 0);
				for(int i = 0; i < inserts; i++)
					printf("M\n");
			}
		}
	}
}

int main(int argc, char* argv[])
{
	bool few_del_min = false, cascade = false, random = false, path = false;
	int opt, student_id = -1;

	if (argc > 1 && !strcmp(argv[1], "--help"))
		usage();

	while ((opt = getopt(argc, argv, "prbxs:")) >= 0)
		switch (opt) {
			case 's': student_id = atoi(optarg); break;
			case 'b': few_del_min = true; break;
			case 'x': cascade = true; break;
			case 'r': random = true; break;
			case 'p': path = true; break;
			default: usage();
		}

	if (student_id < 0) {
		fprintf(stderr, "WARNING: Student ID not given, defaulting to 42.\n");
		student_id = 42;
	}

	if ((few_del_min?1:0) + (cascade?1:0) + (random?1:0) + (path?1:0) >= 2) {
		fprintf(stderr, "ERROR: Two or more tests given. Please, choose exactly one test.\n");
		usage();
	}

	rng_setup(student_id);

	if (few_del_min) random_gen(BIAS);
	else if (cascade) ncascade_gen_mod();
	else if (random) random_gen(0);
	else if (path) path_gen();
	else fprintf(stderr, "ERROR: No test given. You have to choose -r for the random test or -b for the biased test or -x for the special test or -p for the path test.\n");

	return 0;
}
