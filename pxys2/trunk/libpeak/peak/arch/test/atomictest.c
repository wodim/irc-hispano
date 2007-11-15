/* $Id: atomictest.c,v 1.4 2007/05/24 12:54:10 mbuna Exp $
 */
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "atomic.h"

#define MAX_ITER 1000000

#ifdef NOATOMIC
static int target;
#define ATOMIC_INC(t)  t++
#define ATOMIC_DEC(t)  t--
#define ATOMIC_READ(t)  (t)
#else
static peak_atomic_t target;
#define ATOMIC_INC(t)  peak_atomic_inc(&t)
#define ATOMIC_DEC(t)  peak_atomic_dec(&t)
#define ATOMIC_READ(t) peak_atomic_read(&t)
#endif

static void *
runthread_inc(void *arg)
  {
  int i;
  for (i = MAX_ITER; i-- > 0; )
    ATOMIC_INC(target);
  return NULL;
  }

static void *
runthread_dec(void *arg)
  {
  int i;
  for (i = MAX_ITER; i-- > 0; )
    ATOMIC_DEC(target);
  return NULL;
  }

int
main()
  {
  pthread_t t1, t2, t3;
  
  pthread_create(&t1, NULL, runthread_inc, NULL);
  pthread_create(&t2, NULL, runthread_dec, NULL);
  pthread_create(&t3, NULL, runthread_inc, NULL);
  
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  
  printf("target: %d\n", ATOMIC_READ(target));
  assert(ATOMIC_READ(target) == MAX_ITER);
  printf("OK\n");
  return 0;
  }
