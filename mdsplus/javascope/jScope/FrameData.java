package jScope;

/* $Id: FrameData.java,v 1.1 2010/09/03 09:58:16 manduchi Exp $ */
import java.awt.Dimension;
import java.io.IOException;

/**
 * When a Frame sequence is requested to a DataProvider implementation, it is returned by means of an object
 * implemenmting the FrameData interface. FrameData defines therefore all the access methods required to handle 
 * a sequence of frames.
 * 
 * @see DataProvider
 */
interface FrameData
{
    static final int BITMAP_IMAGE_8  = 1;
    static final int BITMAP_IMAGE_16 = 2;
    static final int BITMAP_IMAGE_32 = 4;
    static final int AWT_IMAGE = 5;
    static final int JAI_IMAGE = 6;
    
    /**
     * Returns the type of the corresponding frames. Returned frames can have either of the following types:
     * <br>
     * -FrameData.BITMAP_IMAGE meaning that method GetFrameAt will return a byte matrix.
     * <br>
     * -FrameData.AWT_IMAGE meaning that method GetFrameAt will return a byte vector representing the binary
     *  content of a gif or jpeg file.
     * <br>
     * -FramDeata.JAI_IMAGE meaning that method GetFrameAt will return a byte vector representing the binary
     *  content of every image file supported by the JAI (Java Advanced Imaging) package. The JAI package needs not 
     *  to be installed unless file formats other than gif or jpeg are used.
     * 
     * @return The type of the corresponding frame.
     * @exception java.io.IOException
     */
    public int GetFrameType() throws IOException;

    /**
     * Returns thenumber of frames in the sequence.
     * 
     * @return The number of frames in the sequence.
     * @exception java.io.IOException
     */
    public int GetNumFrames() throws IOException;

    /**
     * Return the dimension of a frame. All the frames in the sequence must have the same dimension.
     * 
     * @return The frame dimension.
     * @exception java.io.IOException
     */
    public Dimension GetFrameDimension()throws IOException;

    /**
     * Return the times associated with every frame of the sequence. This information is required to correlate 
     * the frame sequence with the other signals displayed by jScope.
     * 
     * @return The time array for the frame sequence.
     * @exception java.io.IOException
     */
    public float[] GetFrameTimes() throws IOException;

    /**
     * Return the frame at the given position.
     * 
     * @param idx The index of the requested frame in the frame sequence.
     * @return The frame as a byte array. If the frame type is FrameData.BITMAP_IMAGE, the matrix uses row major
     * ordering.
     * @exception java.io.IOException
     */
    public byte[] GetFrameAt(int idx) throws IOException;
}
