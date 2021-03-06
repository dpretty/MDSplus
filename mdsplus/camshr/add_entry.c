// add_entry.c -- new as of 2001.01.11
//-------------------------------------------------------------------------
//	Stuart Sherman
//	MIT / PSFC
//	Cambridge, MA 02139  USA
//
//	This is a port of the MDSplus system software from VMS to Linux, 
//	specifically:
//			CAMAC subsystem, ie libCamShr.so and verbs.c for CTS.
//-------------------------------------------------------------------------
//	$Id: add_entry.c,v 1.2 2009/03/11 18:05:03 twf Exp $
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// include files
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <errno.h>

#include "common.h"
#include "module.h"
#include "crate.h"
#include "prototypes.h"

//-------------------------------------------------------------------------
// add_entry()
// Thu Jan 11 10:41:13 EST 2001
// Tue Apr  3 16:28:20 EDT 2001
//-------------------------------------------------------------------------
// Add a cts or crate entry to the appropriate db. The new entry is inserted 
// 	into the list alphabetically.
//
// input:	db type, 
// 			pointer to a c-string containing a complete entry
// output:	status
//-------------------------------------------------------------------------
int add_entry( int dbType, char *newEntry )
{
	void					*dbptr;						// re-usable pointer for dbs
	int						entrySize, i, numOfEntries;
	int						status = SUCCESS;			// assume the best
	extern struct MODULE	*CTSdb;		// pointer to in-memory copy of data file
	extern struct CRATE		*CRATEdb;	// pointer to in-memory copy of data file

	if( MSGLVL(FUNCTION_NAME) )
		printf( "add_entry()\n" );

//----------------------------
//	adding an entry 
//----------------------------
//-- 'critical section' start
//----------------------------
	// 'lock' with semaphore
	if( lock_file() != SUCCESS ) {
		status = LOCK_ERROR;
		goto AddEntry_Exit;
	}

	// get current number of entries
	if( (numOfEntries = get_file_count(dbType)) < 0 ) {
		status = FILE_ERROR;
		goto AddEntry_Exit;
	}

	// cull db specific info
	switch( dbType ) {
		case CTS_DB:
			dbptr = (void *)CTSdb;
			entrySize = MODULE_ENTRY;
			break;

		case CRATE_DB:
			dbptr = (void *)CRATEdb;
			entrySize = CRATE_ENTRY;
			break;
	}

	// shift current entries by one
	if( numOfEntries )		// ... only if any entries exist
		for( i = numOfEntries - 1; i >= 0; --i )
			memcpy( (char *)dbptr+((i + 1) * entrySize),
					(char *)dbptr+( i      * entrySize),
					entrySize
					);

	// put new entry at head of list
	memcpy( (char *)dbptr, newEntry, entrySize );

	// insertion sort
	if( numOfEntries > 0 ) 		// only insert if more than one entry exists already
		if( issort(dbptr, numOfEntries + 1, entrySize, compare_str) != 0 ) {
			status = ERROR;
			goto AddEntry_Exit;
		}

	// commit change to file
	if( commit_entry(dbType) != SUCCESS ) {
		status = COMMIT_ERROR;
		goto AddEntry_Exit;
	}

	// release semaphore
	if( unlock_file() != SUCCESS )
		status = UNLOCK_ERROR;

//----------------------------
//-- 'critical section' finish
//----------------------------

AddEntry_Exit:
	if( MSGLVL(DETAILS) ) {
		printf( "add_entry(): " ); ShowStatus( status );
	}

	return status;
}
