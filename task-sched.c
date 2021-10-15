// *********************************************************************
//
//  - Name: task-sched.c
//  - Created on: Sep 23, 2020
//  - Author: Marcus Pieskä
//
// *********************************************************************

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "task-gen.h"
#include "task-proc.h"

//#define PRINT_CORRECT

#define RM 0
#define EDF 1

// *********************************************************************

typedef struct que *pque;
typedef struct que
{
	sch_task *task;
	pque next;

} que;
pque waitlist = NULL;

pque create_q(sch_task *v)
{
	pque tmp = (pque)calloc(1, sizeof(que));
	tmp->next = NULL;
	tmp->task = v;
	return tmp;
}

pque link_in(pque h, pque t)
{
	h->next = t;
	return h;
}

//add the que node in order of shortest duration first
pque add_to_waitlist(sch_task *v, pque list, int algo)
{
	if (list == NULL)
		return create_q(v);

	else if (v->duration < list->task->duration && !algo)
		return link_in(create_q(v), list);
	else if (v->abs_deadline < list->task->abs_deadline && algo)
		return link_in(create_q(v), list);
	else
		return link_in(list, add_to_waitlist(v, list->next, algo));
}

sch_task *get_first_waitlist(pque list)
{

	if (list == NULL)
		return NULL;
	return list->task;
}

pque remove_first_waitlist(pque list)
{

	if (!list)
		return NULL;
	pque temp = list;

	list = list->next;

	free(temp);
	return list;
}

void print_l(pque list)
{
	if (!list)
	{
		//prin("\n");
		return;
	}
	//prin("(duration: %ld, arival: %ld, deadline: %ld )->", list->task->duration,list->task->arrival,list->task->abs_deadline);
	print_l(list->next);
}

int isEmpty(pque head)
{
	return waitlist == NULL ? 1 : 0;
}

void init_task_buf_state(void)
{
	// Initialize your local queue / task storage
	// and any other variables your scheduler will use
	waitlist = NULL;
}

//klar
i64 sched_with_rm(void)
{

	//as long there are remaining tasks, the waitlist not empty, there is a task working, NOT DONE
	if (tasks_remain() || !isEmpty(waitlist) || peek_task())
	{

		//Get the incoming task
		sch_task *task = get_current_sch_task();

		//if there are more arraiving
		while (task != NULL)
		{
			// add all to the waitlist
			waitlist = add_to_waitlist(task, waitlist, RM);
			task = get_current_sch_task();
		}

		//retrive the first task out of the waitlist
		sch_task *shortesttask = get_first_waitlist(waitlist);

		if (shortesttask != NULL)
		{
			//if the proseseor is not working on a task, remove the shortest duration task from the list, and send it to the proccesor
			if (!peek_task())
			{
				waitlist = remove_first_waitlist(waitlist);

				process_task(shortesttask);

				//if the task inside the prossesor have a longer duration then the task in the waitlist. Remove the shortest job from the list
				//preempt task and send it to the waitlist, and start the shortesttask in the prossesor
			}
			else if (peek_task()->duration > shortesttask->duration)
			{
				waitlist = remove_first_waitlist(waitlist);

				sch_task *endtask = preempt_task();

				waitlist = add_to_waitlist(endtask, waitlist, RM);

				process_task(shortesttask);
			}
		}
		//free(shortesttask);
		//free(task);

		return 1;
	}
	return 0;
}
//klar
i64 sched_with_rm_hard(void)
{
	//det är samma kod som förra men, om get_time() har paserat task, deadtime ska man discarad dem
	//prin("\n\n\ntime: %ld\n", get_time());
	//print_l(waitlist);
	//as long there are remaining tasks, the waitlist not empty, there is a task working, NOT DONE
	if (tasks_remain() || !isEmpty(waitlist) || peek_task())
	{

		if (peek_task())
		{
			//prin("i början för prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);
		}

		//Get the incoming task
		sch_task *task = get_current_sch_task();

		//if there are more arraiving
		while (task != NULL)
		{
			// add all to the waitlist
			waitlist = add_to_waitlist(task, waitlist, RM);
			task = get_current_sch_task();
		}

		print_l(waitlist);

		//retrive the first task out of the waitlist
		sch_task *shortesttask = get_first_waitlist(waitlist);

		if (shortesttask != NULL)
		{
			//if the proseseor is not working on a task, remove the shortest duration task from the list, and send it to the proccesor

			if (!peek_task())
			{

				waitlist = remove_first_waitlist(waitlist);

				print_l(waitlist);

				process_task(shortesttask);

				//prin("nya in prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);

				//if the task inside the prossesor have a longer duration then the task in the waitlist. Remove the shortest job from the list
				//preempt task and send it to the waitlist, and start the shortesttask in the prossesor
			}
			else if (peek_task()->duration > shortesttask->duration)
			{
				waitlist = remove_first_waitlist(waitlist);

				sch_task *endtask = preempt_task();

				waitlist = add_to_waitlist(endtask, waitlist, RM);

				process_task(shortesttask);

				//prin("utbyt\n");
			}
		}
		if (peek_task())
		{
			//prin("här\n");
			//prin("nästan slutet för prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);

			if ((peek_task()->abs_deadline <= get_time()))
			{

				//prin("tid för task deadline: %ld , %ld time: id %ld\n", peek_task()->abs_deadline, get_time(), shortesttask->index);
				abort_task();

				print_l(waitlist);

				shortesttask = get_first_waitlist(waitlist);

				if (shortesttask)
				{

					waitlist = remove_first_waitlist(waitlist);

					process_task(shortesttask);

					//prin("nya in prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);
				}

				//prin("time of abort: %ld\n", get_time());
			}
		}

		//print_l(waitlist);

		return 1;
	}
	return 0;
}
//klar
i64 sched_with_edf(void)
{

	if (tasks_remain() || !isEmpty(waitlist) || peek_task())
	{

		//Get the incoming task
		sch_task *task = get_current_sch_task();

		//if there are more arraiving
		while (task != NULL)
		{
			// add all to the waitlist
			waitlist = add_to_waitlist(task, waitlist, EDF);
			task = get_current_sch_task();
		}

		//retrive the first task out of the waitlist
		sch_task *earliestdeadline = get_first_waitlist(waitlist);

		if (earliestdeadline != NULL)
		{
			//if the proseseor is not working on a task, remove the shortest duration task from the list, and send it to the proccesor
			if (!peek_task())
			{
				waitlist = remove_first_waitlist(waitlist);

				process_task(earliestdeadline);

				//if the task inside the prossesor have a longer duration then the task in the waitlist. Remove the shortest job from the list
				//preempt task and send it to the waitlist, and start the shortesttask in the prossesor
			}
			else if (peek_task()->abs_deadline > earliestdeadline->abs_deadline)
			{
				waitlist = remove_first_waitlist(waitlist);

				sch_task *endtask = preempt_task();

				waitlist = add_to_waitlist(endtask, waitlist, EDF);

				process_task(earliestdeadline);
			}
		}
		//free(shortesttask);
		//free(task);

		return 1;
	}
	return 0;
}
//klar
i64 sched_with_edf_hard(void)
{
	//det är samma kod som förra men, om get_time() har paserat task, deadtime ska man discarad dem
	//prin("\n\n\ntime: %ld\n", get_time());
	//print_l(waitlist);
	//as long there are remaining tasks, the waitlist not empty, there is a task working, NOT DONE
	if (tasks_remain() || !isEmpty(waitlist) || peek_task())
	{

		if (peek_task())
		{
			//prin("i början för prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);
		}

		//Get the incoming task
		sch_task *task = get_current_sch_task();

		//if there are more arraiving
		while (task != NULL)
		{
			// add all to the waitlist
			waitlist = add_to_waitlist(task, waitlist, EDF);
			task = get_current_sch_task();
		}

		print_l(waitlist);

		//retrive the first task out of the waitlist
		sch_task *earliestdeadline = get_first_waitlist(waitlist);

		if (earliestdeadline != NULL)
		{
			//if the proseseor is not working on a task, remove the shortest duration task from the list, and send it to the proccesor

			if (!peek_task())
			{

				waitlist = remove_first_waitlist(waitlist);

				print_l(waitlist);

				process_task(earliestdeadline);

				//prin("nya in prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);

				//if the task inside the prossesor have a longer duration then the task in the waitlist. Remove the shortest job from the list
				//preempt task and send it to the waitlist, and start the shortesttask in the prossesor
			}
			else if (peek_task()->abs_deadline > earliestdeadline->abs_deadline)
			{
				waitlist = remove_first_waitlist(waitlist);

				sch_task *endtask = preempt_task();

				waitlist = add_to_waitlist(endtask, waitlist, EDF);

				process_task(earliestdeadline);

				//prin("utbyt\n");
			}
		}
		if (peek_task())
		{
			//prin("här\n");

			//prin("nästan slutet för prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);

			if ((peek_task()->abs_deadline <= get_time()))
			{

				//prin("tid för task deadline: %ld , %ld time: id %ld\n", peek_task()->abs_deadline, get_time(), earliestdeadline->index);

				abort_task();

				print_l(waitlist);

				earliestdeadline = get_first_waitlist(waitlist);

				if (earliestdeadline)
				{

					waitlist = remove_first_waitlist(waitlist);

					process_task(earliestdeadline);

					//prin("nya in prosesopr (duration: %ld, arival: %ld, deadline: %ld )\n", peek_task()->duration, peek_task()->arrival, peek_task()->abs_deadline);
				}
				else
				{
					//prin("tomt\n");
					print_l(waitlist);
				}

				//prin("time of abort: %ld\n", get_time());
			}
		}

		//print_l(waitlist);

		return 1;
	}
	return 0;
}

// *********************************************************************

int main(int argc, char *argv[])
{

	i64 run_tests = 0;
	i64 run_prog1_prog2 = 1;

	if (run_tests)
	{
		init_task_buf_state();
		init_custom_prog(STD_DURATION, "test-comp-rm");
		add_custom_task(0, 25, 50);
		add_custom_task(0, 30, 75);
		run_custom_prog(sched_with_rm);
	}
	if (run_tests)
	{
		init_task_buf_state();
		init_custom_prog(STD_DURATION, "test-comp-edf");
		add_custom_task(0, 25, 50);
		add_custom_task(0, 30, 75);
		run_custom_prog(sched_with_edf);
	}
	if (run_tests)
	{
		init_task_buf_state();
		init_custom_prog(STD_DURATION, "test-comp-rm-hard");
		add_custom_task(0, 25, 50);
		add_custom_task(0, 30, 75);
		run_custom_prog(sched_with_rm_hard);
	}
	if (run_tests)
	{
		init_task_buf_state();
		init_custom_prog(STD_DURATION, "test-comp-edf-hard");
		add_custom_task(0, 25, 50);
		add_custom_task(0, 30, 75);
		run_custom_prog(sched_with_edf_hard);
	}

	if (run_prog1_prog2)
	{
		init_task_buf_state();
		init_test_prog1(sched_with_rm, "rm");
	}
	if (run_prog1_prog2)
	{
		init_task_buf_state();
		init_test_prog2(sched_with_rm, "rm");
	}
	if (run_prog1_prog2)
	{
		init_task_buf_state();
		init_test_prog1(sched_with_edf, "edf");
	}
	if (run_prog1_prog2)
	{
		init_task_buf_state();
		init_test_prog2(sched_with_edf, "edf");
	}

	//prin("*** exiting normally\n");
	return 0;
}

// *********************************************************************
