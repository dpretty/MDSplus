// turn_crate_on_off_line.c
//-------------------------------------------------------------------------
//	Stuart Sherman
//	MIT / PSFC
//	Cambridge, MA 02139  USA
//
//	This is a port of the MDSplus system software from VMS to Linux, 
//	specifically:
//			CAMAC subsystem, ie libCamShr.so and verbs.c for CTS.
//-------------------------------------------------------------------------
//	$Id: turn_crate_on_off_line.c,v 1.1 2002/09/10 16:06:59 twf Exp $
//-------------------------------------------------------------------------

//-----------------------------------------------------------
// include files
//-----------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "crate.h"
#include "prototypes.h"

//-----------------------------------------------------------
// turn_crate_on_off_line()
//-----------------------------------------------------------
// Fri Jul 27 11:56:57 EDT 2001 -- changed calling sequence for CamPiow()
// Tue Jul 31 11:55:21 EDT 2001	-- added 'offline' support
//-----------------------------------------------------------
// Tue Apr 10 11:11:48 EDT 2001
// eg. *crate_name == "GKB509"
//-----------------------------------------------------------
int turn_crate_on_off_line( char *crate_name, int state )
{
	char					controller[12], *pController;
	short 					SCCdata;
	int						i, status = SUCCESS;		// optimistic ...
	TranslatedIosb 			iosb;

	if( MSGLVL(FUNCTION_NAME) )
		printf( "turn_crate_on_off_line('%s'=>%s)\n", crate_name, (state == ON) ? "ON" : "off" );

	// convert to all UPPER CASE
	for( i = 0; i < strlen(crate_name); ++i )
		toupper(*crate_name);

	// create full crate controller designation
	// NB! all crate controllers reside in slot 30
	sprintf( &controller[0], "%.6s:N30", crate_name );

	// lookup name -- make sure a valid device
	if( lookup_entry( CRATE_DB, crate_name ) < 0 ) {		// lookup actual device num
		if( MSGLVL(IMPORTANT) )
			fprintf( stderr, "no such crate in 'crate.db'\n" );

		status = NO_DEVICE;									// doesn't exist
		goto TurnCrateOnOffLine_Exit;
	}
	if( MSGLVL(DETAILS) )
		printf( "lookup OK -- found '%s'\n", crate_name );

	pController = &controller[0];

	SCCdata = 1;					// initiates Dataway Z
	status = CamPiow(
					pController,		// serial crate controller name
					0,				// A	--\__ write status register
					17,				// F	--/
					&SCCdata,		// data value
					16,				// mem == 16-bit data
					&iosb			// *iosb
					);

	SCCdata = (state == ON) ? 0 : 0x1000;	// clear status register
	status = CamPiow(
					pController,		// serial crate controller name
					0,				// A	--\__ write status register
					17,				// F	--/
					&SCCdata, 		// data value
					16,				// mem == 16-bit data
					&iosb			// *iosb
					);

//-----------------------------------------------------------
TurnCrateOnOffLine_Exit:
	if( MSGLVL(DETAILS) )
		printf( "tcool(): status %d\n", status );

	return status;
}
