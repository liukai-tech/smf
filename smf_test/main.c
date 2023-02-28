#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "smf.h"

/*  reference doc
 *  https://docs.zephyrproject.org/latest/services/smf/index.html
 */

/* Forward declaration of state table */
static const struct smf_state demo_states[];

/* List of demo states */
enum demo_state { PARENT, S0, S1, S2 };

/* User defined object */
struct s_object {
    /* This must be first */
    struct smf_ctx ctx;

    /* Other state specific data add here */
} s_obj;

/* Parent State */
static void parent_entry(void *o)
{
    /* Do something */
    printf("parent_entry\n");
}

static void parent_run(void *o)
{
    /* Do something */
    printf("parent_run\n");
}

static void parent_exit(void *o)
{
    /* Do something */
    printf("parent_exit\n");
}

/* State S0 */
static void s0_entry(void *o)
{
    /* Do something */
    printf("s0_entry\n");
}
static void s0_run(void *o)
{
    printf("s0_run\n");
    smf_set_state(SMF_CTX(&s_obj), &demo_states[S1]);
}
static void s0_exit(void *o)
{
    /* Do something */
    printf("s0_exit\n");
}

/* State S1 */
static void s1_run(void *o)
{
    printf("s1_run\n");
    smf_set_state(SMF_CTX(&s_obj), &demo_states[S2]);
}
static void s1_exit(void *o)
{
    /* Do something */
    printf("s1_exit\n");
}

/* State S2 */
static void s2_entry(void *o)
{
    /* Do something */
    printf("s2_entry\n");
}
static void s2_run(void *o)
{
    printf("s2_run\n");
    smf_set_state(SMF_CTX(&s_obj), &demo_states[S0]);
}

/* Populate state table */
static const struct smf_state demo_states[] = {
    /* Parent state does not have a run action */
    [PARENT]    = SMF_CREATE_STATE(parent_entry, parent_run, parent_exit, NULL),
    [S0]        = SMF_CREATE_STATE(s0_entry, s0_run, s0_exit, &demo_states[PARENT]),
    /* State S1 does not have an entry action */
    [S1]        = SMF_CREATE_STATE(NULL, s1_run, s1_exit, &demo_states[PARENT]),
    /* State S2 does not have an exit action */
    [S2]        = SMF_CREATE_STATE(s2_entry, s2_run, NULL, NULL),
};

int main(void)
{
    int32_t ret;

    printf("smf test!\n");

    /* Set initial state */
    smf_set_initial(SMF_CTX(&s_obj), &demo_states[S0]);

    /* Run the state machine */
    while(1) {
        /* State machine terminates if a non-zero value is returned */
        ret = smf_run_state(SMF_CTX(&s_obj));
        if (ret) {
            /* handle return code and terminate state machine */
            break;
        }
        Sleep(1000);
    }

    return 0;
}

