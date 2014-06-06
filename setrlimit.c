/*
 * © 2014, Tadeu Bastos
 * 
 * Usage of the works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument.
 * 
 * DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
 */

#include <sys/resource.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

static int convert_resource(char *);
static void apply_rlim_cur(int, rlim_t);

int
main(int argc, char *argv[])
{
	int resource;
	rlim_t rlim_cur;
	char *file;

	if (argc < 4)
		errx(1, "Usage: setrlimit "
			"<resource> <limit> <file> [args]");
        resource = convert_resource(argv[1]);
	rlim_cur = (rlim_t)strtoul(argv[2], NULL, 0);
	apply_rlim_cur(resource, rlim_cur);
	file = argv[3];	
	if (execvp(file, &argv[3]) == -1)
		err(1, NULL);
	exit(0);
}

#define RETURN_IF_MATCH(i, r) \
	if (strcmp((i), (#r)) == 0) return (RLIMIT_##r)

int
convert_resource(char *input)
{

	RETURN_IF_MATCH(input, AS);
	RETURN_IF_MATCH(input, CORE);
	RETURN_IF_MATCH(input, CPU);
	RETURN_IF_MATCH(input, DATA);
	RETURN_IF_MATCH(input, FSIZE);
	RETURN_IF_MATCH(input, LOCKS);
	RETURN_IF_MATCH(input, MEMLOCK);
	RETURN_IF_MATCH(input, MSGQUEUE);
	RETURN_IF_MATCH(input, NICE);
	RETURN_IF_MATCH(input, NOFILE);
	RETURN_IF_MATCH(input, NPROC);
	RETURN_IF_MATCH(input, RSS);
	RETURN_IF_MATCH(input, RTPRIO);
	RETURN_IF_MATCH(input, RTTIME);
	RETURN_IF_MATCH(input, SIGPENDING);
	/* FIXME: the returned value may collide with the defined resources */
	return -1;
}

#undef RETURN_IF_MATCH

void
apply_rlim_cur(int resource, rlim_t rlim_cur)
{
	struct rlimit rlim;

	if (getrlimit(resource, &rlim) == -1)
		err(1, NULL);
	rlim.rlim_cur = rlim_cur;
	if (setrlimit(resource, &rlim) == -1)
		err(1, NULL);
}
