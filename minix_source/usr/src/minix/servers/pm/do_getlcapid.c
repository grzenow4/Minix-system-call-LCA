#include <lib.h>
#include "pm.h"
#include "proto.h"
#include "mproc.h"

static pid_t find_parent(pid_t pid)
{
	for (int i = 0; i < NR_PROCS; i++)
	{
		if (mproc[i].mp_pid == pid)
		{
			int parent = mproc[i].mp_parent;
			if (parent < 0 || mproc[parent].mp_pid == pid)
				return -1;
			return mproc[parent].mp_pid;
		}
	}
	return -1;
}

int do_getlcapid(void)
{
	pid_t pid_1 = m_in.m_m1.m1i1;
	pid_t pid_2 = m_in.m_m1.m1i2;

	if (pid_1 < 0 || pid_2 < 0)
		return -1;

	int idx_1 = -1, idx_2 = -1;
	for (int i = 0; i < NR_PROCS; i++)
	{
		if (mproc[i].mp_pid == pid_1)
			idx_1 = i;
		if (mproc[i].mp_pid == pid_2)
			idx_2 = i;
	}

	if (idx_1 < 0 || idx_2 < 0)
		return -1;

	if (!(mproc[idx_1].mp_flags & IN_USE) || !(mproc[idx_2].mp_flags & IN_USE))
		return -1;

	pid_t pid_1_parents[NR_PROCS]; // wszyscy przodkowie pid_1

	int len = 0;
	while (pid_1 > 0)
	{
		pid_t parent = find_parent(pid_1);
		pid_1_parents[len] = parent;
		pid_1 = parent;
		len++;
	}

	while (pid_2 > 0)
	{
		pid_t parent = find_parent(pid_2);
		if (parent < 0)
			return 0;
		for (int i = 0; i < len; i++)
		{
			if (pid_1_parents[i] == parent)
				return parent;
		}
		pid_2 = parent;
	}

	return 0;
}