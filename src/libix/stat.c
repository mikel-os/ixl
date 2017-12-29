/*
	Copyright © 2016-2017 Mikel Cazorla Pérez
	This file is part of Ixl, licensed under the terms of GPLv2.
*/

#include <u.h>
#include <libc.h>
#include <libix.h>
#include <sysp.h>
#include "fns.h"

int
ixlstat(char file[], uchar edir[], int nedir)
{
	Syscall	c;
	Sysreply	r;

	c.i = SYSSTAT;
	c.stat.file = file;
	c.stat.nedir = nedir;

	if(syspC2R(getixlfd(), &c, &r) < 0)
		return -1;

	if(r.i != SYSSTAT){
		werrstr("wrong reply (bug signal), n: %d", r.i);
		freeretR(&r);
		return -1;
	}
	if(!r.stat.edir){
		werrstr("empty buffer");
		freeretR(&r);
		return -1;
	}
	if(r.stat.nedir > nedir){
		werrstr("buffer too long (bug signal; r: %d, e: %d)",
			r.stat.nedir, nedir);
		freeretR(&r);
		return -1;
	}
	nedir = r.stat.nedir;
	memmove(edir, r.stat.edir, nedir);
	freeretR(&r);

	return nedir;
}
