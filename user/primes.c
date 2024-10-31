#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUMBER 280
#define FIRST_PRIME 2
// primes(int) __attribute__((noreturn));

int write_num_to_pipe() {
	int p[2];
	pipe(p);

	if (fork() == 0) {
		for (int i = FIRST_PRIME; i <= MAX_NUMBER; i++) {
			write(p[1], &i, sizeof(i));
		}
		close(p[1]);
		exit(0);
	}

	close(p[1]);
	return p[0];
}

// p = get a number from left neighbor
// print p
// loop:
//     n = get a number from left neighbor
//     if (p does not divide n)
//         send n to right neighbor

int sieve(int fd, int prime) 
{
	int num;
	int p[2];

	pipe(p);

	if (fork() == 0) {
		while (read(fd, &num, sizeof(int)) != 0) {
			if (num % prime != 0) {
				write(p[1], &num, sizeof(int));
			}
		}
		close(fd);
		close(p[1]);
		
		exit(0);
	}

	close(fd);
	close(p[1]);

	return p[0];
}

int main(int argc, char *argv[])
{
	int prime; 
	int num = write_num_to_pipe();
	while (read(num, &prime, sizeof(int)) != 0) {
		printf("prime %d\n", prime); 
		num = sieve(num, prime);
	}

	exit(0);
}