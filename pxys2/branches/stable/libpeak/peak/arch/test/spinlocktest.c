/* $Id: spinlocktest.c,v 1.1 2004/01/02 18:59:36 mbuna Exp $
 */
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "spinlock.h"

#define MAX_ITER 10000000

static peak_spinlock_t lock = PEAK_SPINLOCK_INITIALIZER;
static int target = 0;

int _peak_is_threaded;

static void *
runthread_inc(void *arg)
  {
  int i, j;
  for (i = MAX_ITER; i-- > 0; )
    {
    _peak_spinlock_lock(&lock);
    target++;
    for (j = 10; j-- > 0; )
      ;
    _peak_spinlock_unlock(&lock);
    }
  return NULL;
  }

static void *
runthread_dec(void *arg)
  {
  int i, j;
  for (i = MAX_ITER; i-- > 0; )
    {
    _peak_spinlock_lock(&lock);
    target--;
    for (j = 10; j-- > 0; )
      ;
    _peak_spinlock_unlock(&lock);
    }
  return NULL;
  }

int
main()
  {
  pthread_t t1, t2, t3;

  _peak_is_threaded = 1;

  pthread_create(&t1, NULL, runthread_inc, NULL);
  pthread_create(&t2, NULL, runthread_dec, NULL);
  pthread_create(&t3, NULL, runthread_inc, NULL);
  
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  
  printf("target: %d\n", target);
  assert(target == MAX_ITER);
  printf("OK\n");
  return 0;
  }
