/* ------------------------------------------------------------------------ */
/*
 * TWUSignal: Handling the TEC Web-Umbrella Signal (Bulk) File.
 *
 * Copyright � 2002, J.G.Krom, H.R.Koslowski, Forschungszentrum J�lich GmbH.
 * All rights reserved.
 * 
 * This class handles the bulk datafiles as used in the TEC Web-Umbrella.
 *
 * Authors: Jon Krom, H.R.Koslowski, 
 *          Forschungszentrum J�lich, Institut f�r Plasmaphysik.
 *
 * $Id: TWUSignal.java,v 1.1 2002/04/05 09:36:36 manduchi Exp $
 *
 * $Log: TWUSignal.java,v $
 * Revision 1.1  2002/04/05 09:36:36  manduchi
 * Upgraded TWUDataProvider
 *
 * Derived from (matlab-) WUSignal.java Revision 1.6 2002/02/26 hrk/jgk
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
            StringBuffer bulk 
                = new StringBuffer(twup.FQBulkName() + "?start=" + firstSample );

            if (step>0)
              bulk.append("&step="  + step );
            if (maxSamples>0)
              bulk.append("&total=" + maxSamples);

            bulkURL = new URL(bulk.toString());

            instream = 
                new BufferedReader(new InputStreamReader(bulkURL.openStream()));
        }
        catch (Exception e) 
        {
            System.out.println("TWUSignal.prepareToRead :" + e);
        }
    }

    /* -------------------------------------------------------------------- */
    // Methods for stepwise completion of the read-data action.

    public boolean
    complete()
    {
        return finished ;
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

            if (s==null)
            {
                try { instream.close(); }
                catch (Exception e) {}
                finished =true;
            }
            else
              finished = sampleCount>=samples2Read ;
        }
        catch (Exception e) 
        {
            System.out.println("TWUSignal.tryToRead :" + e);
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
        return "$Id: TWUSignal.java,v 1.1 2002/04/05 09:36:36 manduchi Exp $";
    }
}

/* ------------------------------------------------------------------------ */
// $Id: TWUSignal.java,v 1.1 2002/04/05 09:36:36 manduchi Exp $
/* ------------------------------------------------------------------------ */