/*
 * This file is part of tcpflow by Jeremy Elson <jelson@circlemud.org>
 * Initial Release: 7 April 1999.
 *
 * This source code is under the GNU Public License (GPL).  See
 * LICENSE for details.
 *
 * $Id: sysdep.h,v 1.5 2001/08/08 19:39:40 jelson Exp $
 *
 * $Log: sysdep.h,v $
 * Revision 1.5  2001/08/08 19:39:40  jelson
 * ARGH!  These are changes that made up tcpflow 0.20, which for some reason I
 * did not check into the repository until now.  (Which of couse means
 * I never tagged v0.20.... argh.)
 *
 * Changes include:
 *
 *   -- portable signal handlers now used to do proper termination
 *
 *   -- patch to allow tcpflow to read from tcpdump stored captures
 *
 * Revision 1.4  2000/12/08 07:32:39  jelson
 * Took out the (broken) support for fgetpos/fsetpos.  Now we always simply
 * use fseek and ftell.
 *
 * Revision 1.3  1999/04/21 01:40:14  jelson
 * DLT_NULL fixes, u_char fixes, additions to configure.in, man page update
 *
 * Revision 1.2  1999/04/13 23:17:56  jelson
 * More portability fixes.  All system header files now conditionally
 * included from sysdep.h.
 *
 * Integrated patch from Johnny Tevessen <j.tevessen@gmx.net> for Linux
 * systems still using libc5.
 *
 * Revision 1.1  1999/04/13 01:38:13  jelson
 * Added portability features with 'automake' and 'autoconf'.  Added AUTHORS,
 * NEWS, README, etc files (currently empty) to conform to GNU standards.
 *
 * Various portability fixes, including the FGETPOS/FSETPOS macros; detection
 * of header files using autoconf; restructuring of debugging code to not
 * need vsnprintf.
 *
 */

/*
 * Set up various machine-specific things based on the values determined
 * from configure and conf.h.
 */


/* Standard C headers  *************************************************/

#ifndef __SYSDEP_H__
#define __SYSDEP_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif

# include <sys/types.h>
# include <unistd.h>
# include<sys/bitypes.h>
# include <sys/time.h>
# include <time.h>

#ifndef __FAVOR_BSD
#define __FAVOR_BSD
#endif

#ifndef __USE_BSD
#define __USE_BSD
#endif

# include <sys/socket.h>
# include <net/if.h>
# include <netinet/tcp.h>
# include <netinet/udp.h>
# include <netinet/in_systm.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/if_ether.h>

/* Linux libc5 systems have different names for certain structures.
 * Patch sent by Johnny Tevessen <j.tevessen@gmx.net> */
#if !defined(HAVE_NETINET_IF_ETHER_H) && defined(HAVE_LINUX_IF_ETHER_H)
# include <linux/if_ether.h>
# define ether_header ethhdr
# define ether_type h_proto
# define ETHERTYPE_IP  ETH_P_IP
# define ETHERTYPE_ARP ETH_P_ARP
#endif

# include <signal.h>
#include <pcap.h>



/****************** Ugly System Dependencies ******************************/

/* We always want to refer to RLIMIT_NOFILE, even if what you actually
 * have is RLIMIT_OFILE */
#ifdef RLIMIT_OFILE
# ifndef RLIMIT_NOFILE
#  define RLIMIT_NOFILE RLIMIT_OFILE
# endif
#endif

/* We always want to refer to OPEN_MAX, even if what you actually have
 * is FOPEN_MAX. */
#ifdef FOPEN_MAX
# ifndef OPEN_MAX
#  define OPEN_MAX FOPEN_MAX
# endif
#endif

/* some systems don't define SEEK_SET... sigh */
# define SEEK_SET 0

#define FGETPOS(file, position) *(position) = ftell(file)
#define FSETPOS(file, position) fseek((file), (*position), SEEK_SET)


#endif /* __SYSDEP_H__ */

