package jScope;

/* $Id: JiNcVarFloat.java,v 1.1 2010/09/03 09:58:17 manduchi Exp $ */
import java.io.*;
import java.util.*;

class JiNcVarFloat extends JiNcVarImp 
{
	public JiNcVarFloat(RandomAccessData in, JiNcVar parent, long offset)
	  {
	  super(in, parent, offset);
	  }  
	public Object read(JiDim[] dims) throws IOException
	  {
	  return readFloat(dims);
	  }  
	public float[] readFloat(JiDim[] dims) throws IOException
	  {
	  float[] rval = null;
	  mParent.validateDims(dims);
	  JiSlabIterator itr =
	    new JiSlabIterator((JiNcSource)mParent.getSource(), mParent, dims);
	  int size = itr.size();
	  rval = new float[size];
	  JiSlab slab;
	  int counter = 0;
	  while((slab = itr.next()) != null){
	      byte[] bytes = new byte[slab.mSize * sizeof()];
	      float[] floats = new float[slab.mSize];
	      mRFile.seek(mOffset + slab.mOffset);
	      mRFile.readFully(bytes);
	      convertFloats(bytes,floats);
	      for (int i=0; i < slab.mSize; ++i){
		  rval[counter++] = floats[i];
	      }
	  }
	  return rval;
	  }  
	public int sizeof()
	  {
	  return 4;
	  }  
}
