/*
 * (c) 2004-2011 Denora Team 
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 *
 * $Id: sqlqueue.c 386 2011-01-04 17:16:43Z hal9000 $                      
 *
*/

#include "denora.h"

#ifdef USE_THREADS
#ifndef _WIN32
deno_mutex_t queuemut = PTHREAD_MUTEX_INITIALIZER;
deno_cond_t queuecond = PTHREAD_COND_INITIALIZER;
#else
deno_mutex_t queuemut;
deno_cond_t queuecond;
#endif
#endif

QueueEntry *qp;

/*************************************************************************/

int QueueEntryInit(void)
{
#ifdef USE_THREADS
    int i;
    deno_thread_t th;

    for (i = 1; i <= ThreadCount; i++) {
        if (deno_thread_create(th, queue_thread_main, NULL)) {
            return 0;
        }
        if (deno_thread_detach(th)) {
            return 0;
        }
        alog(LOG_DEBUG, "debug: Creating Queue thread %ld (%d of %d)",
             (long) th, i, ThreadCount);
    }

    alog(LOG_DEBUG, "Queue Thread initialized");
#endif
    return 1;
}

/*************************************************************************/

void queue_unlock(void *arg)
{
#ifdef USE_THREADS
    alog(LOG_DEBUG, "debug: Thread %ld: Unlocking queue mutex",
         (long int) deno_thread_self());
    deno_mutex_unlock(queuemut);
#endif
}

/*************************************************************************/

void queue_lock(void)
{
#ifdef USE_THREADS
    alog(LOG_DEBUG, "debug: Thread %ld: Locking proxy queue mutex",
         (long int) deno_thread_self());
    deno_mutex_lock(queuemut);
#endif
}

/*************************************************************************/

void queue_wait(void)
{
#ifdef USE_THREADS
    alog(LOG_DEBUG, "debug: Thread %ld: waiting proxy queue condition",
         (long int) deno_thread_self());
    deno_cond_wait(queuecond, queuemut);
#endif
}

/*************************************************************************/

void queue_signal(void)
{
#ifdef USE_THREADS
    alog(LOG_DEBUG, "debug: Thread %ld: Signaling proxy queue condition",
         (long int) deno_thread_self());
    deno_cond_signal(queuecond);
#endif
}

/*************************************************************************/

void *queue_thread_main(void *arg)
{
#ifdef USE_THREADS
    while (1) {
        deno_cleanup_push(queue_unlock, NULL);
        queue_lock();
        queue_wait();
        deno_cleanup_pop(1);

        while (1) {
            deno_cleanup_push(queue_unlock, NULL);
            queue_lock();
            deno_cleanup_pop(1);

            while (qp != NULL) {
                qp = ExecuteQueue(qp);
            }
            break;
        }
    }
#endif
    return NULL;
}

/***************************************************************************************/

QueueEntry *AddQueueEntry(QueueEntry * qep, char *sqlmsg)
{
    QueueEntry *lp = qep;

    alog(LOG_DEBUG, "Adding Query %s", sqlmsg);

    if (qep != NULL) {
        while (qep && qep->link != NULL) {
            qep = qep->link;
        }
        queue_lock();
        qep->link = (QueueEntry *) malloc(sizeof(QueueEntry));
        qep = qep->link;
        qep->link = NULL;
        qep->msg = sstrdup(sqlmsg);
        queue_signal();
        queue_unlock(NULL);
        return lp;
    } else {
        queue_lock();
        qep = (QueueEntry *) malloc(sizeof(QueueEntry));
        qep->link = NULL;
        qep->msg = sstrdup(sqlmsg);
        queue_signal();
        queue_unlock(NULL);
        return qep;
    }
}

/***************************************************************************************/

QueueEntry *RemoveQueueEntry(QueueEntry * qep)
{
    QueueEntry *tempp;
    alog(LOG_DEBUG, "Removing Queue entry data");
    alog(LOG_DEBUG, "SQL %s", qep->msg);
    free(qep->msg);
    tempp = qep->link;
    free(qep);
    return tempp;
}

/***************************************************************************************/

void PrintQueueEntry(QueueEntry * qep)
{
    int i = 0;

    if (qep == NULL) {
        alog(LOG_DEBUG, "Queue is currently Empty");
    } else {
        while (qep != NULL) {
            alog(LOG_DEBUG, "List Entry %d", i);
            alog(LOG_DEBUG, "Message %s", qep->msg);
            i++;
            qep = qep->link;
        }
    }
}

/***************************************************************************************/

QueueEntry *ExecuteQueue(QueueEntry * qep)
{
    if (qep != NULL) {
        rdb_direct_query(qep->msg);
        return RemoveQueueEntry(qep);
    }
    return NULL;
}

/***************************************************************************************/

void ClearQueueEntry(QueueEntry * qep)
{
    while (qep != NULL) {
        qep = RemoveQueueEntry(qep);
    }
}

/***************************************************************************************/
