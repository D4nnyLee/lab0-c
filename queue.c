#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Create a new node which stores the content of s.
 * Return NULL if could not allocate space.
 */
static list_ele_t *ele_new(char *s)
{
    list_ele_t *node = malloc(sizeof(list_ele_t));
    if (!node)
        return NULL;

    node->next = node;
    node->value = NULL;

    if (s) {
        size_t len = strlen(s) + 1;

        node->value = malloc(len);
        if (!node->value) {
            free(node);
            return NULL;
        }

        strncpy(node->value, s, len);
    }

    return node;
}

static void ele_free(list_ele_t *node)
{
    if (node)
        free(node->value);
    free(node);
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    list_ele_t *newh = ele_new(NULL);  // dummy node
    /* TODO: What if malloc returned NULL? */
    if (!q || !newh) {
        free(q);
        ele_free(newh);
        return NULL;
    }

    q->head = newh;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;

    list_ele_t *ele = q->head;
    for (int i = q->size; i >= 0; i--) {  // loop for q->size + 1 times
        list_ele_t *tmp = ele;
        ele = ele->next;
        ele_free(tmp);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    newh = ele_new(s);
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!q || !newh) {
        ele_free(newh);
        return false;
    }

    newh->next = q->head->next;
    q->head->next = newh;
    ++q->size;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q_insert_head(q, s))
        return false;

    q->head->value = q->head->next->value;
    q->head->next->value = NULL;
    q->head = q->head->next;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q_size(q))
        return false;

    if (sp && bufsize) {
        strncpy(sp, q->head->next->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *tmp = q->head->next;
    q->head->next = q->head->next->next;
    --q->size;

    ele_free(tmp);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    list_ele_t *node, *next, *prev;
    if (!q)
        return;

    prev = q->head;
    node = prev->next;
    next = node->next;
    for (int i = q->size; i >= 0; i--) {  // loop for q->size + 1 times
        node->next = prev;
        prev = node;
        node = next;
        next = node->next;
    }
}

/*
 * Sort elements in range (beg, end)
 * Note: not in [beg, end)
 */
static void merge_sort(list_ele_t *beg, list_ele_t *end)
{
    if (beg->next == end || beg->next->next == end)  // length <= 1
        return;

    list_ele_t *slow = beg->next, *left, *right, *left_end;
    for (list_ele_t *fast = slow; fast->next != end && fast->next->next != end;
         fast = fast->next->next)
        slow = slow->next;

    merge_sort(slow, end);
    left_end = right = slow->next;

    merge_sort(beg, right);
    left = beg->next;

    for (; left != left_end || right != end; beg = beg->next) {
        if (right == end ||
            (left != left_end && strcasecmp(left->value, right->value) <= 0)) {
            beg->next = left;
            left = left->next;
        } else {
            beg->next = right;
            right = right->next;
        }
    }

    beg->next = end;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q_size(q))
        merge_sort(q->head, q->head);
}
