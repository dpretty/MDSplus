/* $Id: JiDataSource.java,v 1.3 2002/04/26 13:14:29 jgk Exp $ */
import java.io.*;
import java.util.*;

/**
 * Interface for reading scientific data sets
 * Looks a lot like netCDF
 */ 

public interface JiDataSource {
	/**
	 * Get a global attribute named 'name'
	 *
	 * @exception IOException
	 * @return the global attribute named 'name'
	 */
	public JiVar getGlobalAtt(String name) throws IOException ;
	/**
	 * Get all the global attributes for this source
	 *
	 * @return array containing global attributes
	 */
	public JiVar[] getGlobalAtts();
	/**
	 * Get a variable named 'name'
	 *
	 * @exception IOException
	 * @return the variable named 'name'
	 */
	public JiVar getVar(String name) throws IOException ;
	/**
	 * Get all the variables for this source
	 *
	 * @return array containing variables
	 */
	public JiVar[] getVars();
}
