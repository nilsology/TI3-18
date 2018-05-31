#include <stdlib.h>
#include "scheduler.h"

struct Process* rr(struct Process* head, struct Process* current)
{
  if (current && current->next != head)
    return current->next;
  if (head->next != head)
    return head->next;
  return NULL;
}

struct Process* fcfs(struct Process* head, struct Process* current)
{
  if(current && current->cycles_todo)
    return current;
  if(current && current->next != head)
    return current->next;
  if(head->next != head)
    return head->next;
  return NULL;
}

struct Process* spn(struct Process* head, struct Process* current)
{
  if(current && current->cycles_todo)
    return current;
  if(head->next != head)
  {
    struct Process* result = head->next;
    struct Process* tmp = head->next->next;
    while(tmp != head)
    {
      if(tmp->cycles_todo < result->cycles_todo)
        result = tmp;
      tmp = tmp->next;
    }
    return result;
  }
  return NULL;
}

struct Process* srt(struct Process* head, struct Process* unused)
{
  (void)unused;
  if(head->next != head)
  {
    struct Process* result = head->next;
    struct Process* tmp = head->next->next;
    while(tmp != head)
    {
      if(tmp->cycles_todo < result->cycles_todo)
        result = tmp;
      tmp = tmp->next;
    }
    return result;
  }
  return NULL;
}

struct Process* hrrnNonPreemptive(struct Process* head, struct Process* current)
{
  if(current && current->cycles_todo)
    return current;
  if(head->next != head)
  {
    struct Process* result = head->next;
    uint64_t result_rr = (result->cycles_done) ? ((result->cycles_waited/result->cycles_done)+1) : (result->cycles_waited+1);
    struct Process* tmp = head->next->next;
    while(tmp != head)
    {
      uint64_t tmp_rr =  (tmp->cycles_done) ? ((tmp->cycles_waited/tmp->cycles_done)+1) : (tmp->cycles_waited+1);
      if(tmp_rr > result_rr)
      {
        result = tmp;
        result_rr = tmp_rr;
      }
      tmp = tmp->next;
    }
    return result;
  }
  return NULL;
}

struct Process* hrrnPreemptive(struct Process* head, struct Process* current) {
  return NULL;
}
