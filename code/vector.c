#include <stdlib.h>
#include <string.h>
#include "f9cc.h"

#define BLOCK_SIZE 8

static int max(int a, int b)
{
	return (a > b) ? a : b;
}

static int roundup(int n)
{
	if (n == 0) return 0;
	int r = 1;
	while (n > r) r *= 2;
	return r;
}

Vector init_vector(int size = 0)
{
	Vector *r = malloc(sizeof(Vector));
	size = roundup(size);
	if (size > 0) r->body = malloc(sizeof(void *) * size);
	r->len = 0;
	r->nalloc = size;
	return r;
}

static void extend_vector(Vector *vec, int delta)
{
	if (vec->len + delta <= vec->nalloc) return;
	int new_size = max(roundup(vec->len + delta), BLOCK_SIZE);
	void *newbody = malloc(sizeof(void *) * new_size);
	memcpy(newbody, vec->body, sizeof(void *) * vec->len);
	vec->body = newbody;
	vec->nalloc = new_size;
}

void push_vector(Vector *vec, void *item)
{
	extend_vector(vec, 1);
	vec->body[vec->len++] = item;
}

void *pop_vector(Vector *vec)
{
	assert(vec->len > 0);
	return vec->body[--vec->len];
}

void *head_vector(Vector *vec)
{
	assert(vec->len > 0);
	return vec->body[0];
}

void *tail_vector(Vector *vec)
{
	assert(vec->len > 0);
	return vec->body[vec->len - 1];
}

