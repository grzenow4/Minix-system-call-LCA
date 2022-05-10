#include <lib.h>
#include <minix/rs.h>

int get_pm_endpt(endpoint_t *pt)
{
	return minix_rs_lookup("pm", pt);
}

pid_t getlcapid(pid_t pid_1, pid_t pid_2)
{
	endpoint_t pm_pt;
	message m;
	m.m_m1.m1i1 = pid_1;
	m.m_m1.m1i2 = pid_2;
	if (get_pm_endpt(&pm_pt) != 0)
	{
		errno = ENOSYS;
		return -1;
	}

	int res = _syscall(pm_pt, PM_GETLCAPID, &m);

	if (res < 0)
	{
		errno = EINVAL;
		return -1;
	}
	else if (res == 0)
	{
		errno = ESRCH;
		return -1;
	}
	else
	{
		return res;
	}
}