/* ------------------------------------------------------------------------ */
/*
 * TWUSignal: Handling the TEC Web-Umbrella Signal (Bulk) File.
 *
 * Copyright � 2002, J.G.Krom, H.R.Koslowski, Forschungszentrum J�lich GmbH.
 * All rights reserved.
 * 
 * This class handles the bulk datafiles as used in the TEC Web-Umbrella.
 *
 * The TWU* classes (with TWU in three capitals) are written so that they 
 * can also be used in other java based applications and not only by jScope.  
 * Please keep the code of these separate of jScope specific elements.
 *
 * Authors: Jon Krom, H.R.Koslowski, 
 *          Forschungszentrum J�lich, Institut f�r Plasmaphysik.
 *
 * $Id: TWUSignal.java,v 1.21 2003/03/14 15:53:49 jgk Exp $
 *
 * ------------------------------------------------------------------------ 
 * Derived from (matlab-) WUSignal.java Revision 1.6 2002/02/26 hrk/jgk
 * I removed the automatic CVS log from this file; it can be read
 * directly from the CVS repository with "cvs log".
 */
/* ------------------------------------------------------------------------ */

import java.net.*;
import java.io.*;

public class TWUSignal
{
    private TWUProperties  twup         = null;
    private URL            bulkURL      = null;
    private BufferedReader instream     = null;
    private float          ydata[]      = null;
    private int            sampleCount  = 0;
    private int            samples2Read = 0;
    private boolean        finished     = false;
    private boolean        error        = false;

    /* -------------------------------------------------------------------- */
    // Constructors (and related functions)


    // This (constructor) function creates an array of values representing a
    // signal found on the Web-Ubrella.  This array contains at most
    // "maxSamples" samples, from the whole signal, subsampled by a simple
    // skipping algorithm.

    public 
    TWUSignal (TWUProperties SigURL)
    {    
        this(SigURL, 0, 0, 0);
    }
    

    public 
    TWUSignal (TWUProperties SigURL, int firstSample, int step, int maxSamples)
    {
        boolean success = false;
        twup            = SigURL;

        if(maxSamples==0)
          maxSamples=twup.LengthTotal();

        samples2Read    = maxSamples;

        ydata       = new float[samples2Read];
        finished    = false;
        sampleCount = 0;

        if (!SigURL.valid())
        {
            finished =true;
            return;
        }

        if (twup.Equidistant())
          success=tryToConstruct(firstSample, step, maxSamples);

        if (!success)
          prepareToRead(firstSample, step, maxSamples);
    }


    private boolean 
    tryToConstruct(int firstSample, int step, int maxSamples)
    {
        boolean success=false;
        double  last;
        double  first;

        if (step==0)
          step=1;

        if (twup.Incrementing()) 
        {
            first = twup.Minimum();
            last  = twup.Maximum();
        }
        else if (twup.Decrementing()) 
        {
            first = twup.Maximum();
            last  = twup.Minimum();
        }
        else
          return success=false;
    
        double segments = twup.LengthTotal() -1;    /* Note the minus 1 */
        double span     = last-first;
        double delta    = span / segments;

        while ( sampleCount < maxSamples )
        {
            ydata[sampleCount] = (float)((sampleCount + firstSample) *step * delta + first);

            if (ydata[sampleCount] > twup.Maximum())
              ydata[sampleCount] = (float)twup.Maximum();
            
            else if (ydata[sampleCount] < twup.Minimum())
              ydata[sampleCount] = (float)twup.Minimum();
            
            sampleCount++;
        }
        finished =true;
        return success=true ;
    }


    private void
    prepareToRead(int firstSample, int step, int maxSamples)
    {
        try
        {
            error = false ;
            StringBuffer bulk 
                = new StringBuffer(twup.FQBulkName() + "?start=" + firstSample );

            if (step>0)
              bulk.append("&step="  + step );
            if (maxSamples>0)
              bulk.append("&total=" + maxSamples);

            bulkURL = new URL(bulk.toString());

            URLConnection con = bulkURL.openConnection();

            con.setRequestProperty("User-Agent",
                                   "TWUSignal.java for jScope ($Revision: 1.21 $)");

            // It seems to be more efficient, for the type of data we have in the
            // bulk files, to close the connection after the server has send all
            // the data.  In that way HTTP/1.1 servers will not "chunk" the data.
            // This chunking doubled the amounts to transfer and the de-chunking 
            // on the client side took significant effort.

            con.setRequestProperty("Connection", "close"); 

            con.connect();

            instream = 
                new BufferedReader(new InputStreamReader(con.getInputStream()));
        }
        catch (Exception e) 
        {
            System.out.println("TWUSignal.prepareToRead :" + e);
            error = true ;
        }
    }

    /* -------------------------------------------------------------------- */
    // Methods for stepwise completion of the read-data action.

    public boolean
    complete()
    {
        return finished ;
    }

    public boolean
    error()
    { 
        return error ;
    }

    public void
    tryToRead(final int samples2Try)
    {
        int thisAttempt=0;

        try 
        {
            String s=null;

            while ( (samples2Try > thisAttempt++)
                    && 
                    (samples2Read > sampleCount) 
                    && 
                    ((s=instream.readLine()) != null) )
            {
                Float F = Float.valueOf(s);
                ydata[sampleCount++] = F.floatValue();
            }

            finished = (sampleCount>=samples2Read || s == null) ;

            if ( finished )
            {
                // boolean premature_eof = (s==null);
                // We should handle this, if it is a real problem.
                
                try { instream.close(); }
                catch (Exception e) {}

                if (sampleCount<samples2Read)
                {
                    // Fill-up required
                    if (sampleCount==0)
                      ydata[sampleCount++]=0.0F;

                    while (sampleCount<samples2Read)
                    {
                        ydata[sampleCount] = ydata[sampleCount-1] ;
                        sampleCount++;
                    }
                }
            }
        }
        catch (Exception e) 
        {
            System.out.println("TWUSignal.tryToRead :" + e);
            error = true ;
        }
    }

    /* -------------------------------------------------------------------- */
    // Accessors

    public int
    getActualSampleCount()
    {
        return sampleCount;
    }

    public float[]
    getBulkData()
    {
        return ydata;
    }

    public String 
    urlstring()
    {
        return bulkURL.toString();
    }
    
    /* -------------------------------------------------------------------- */

    public static String 
    revision()
    {
        return "$Id: TWUSignal.java,v 1.21 2003/03/14 15:53:49 jgk Exp $";
    }
}

/* ------------------------------------------------------------------------ */
// $Id: TWUSignal.java,v 1.21 2003/03/14 15:53:49 jgk Exp $
/* ------------------------------------------------------------------------ */
