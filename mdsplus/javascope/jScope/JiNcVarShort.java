package jScope;

/* $Id: JiNcVarShort.java,v 1.1 2010/09/03 09:58:17 manduchi Exp $ */
import java.io.*;
import java.util.*;

class JiNcVarShort extends JiNcVarImp 
{
	public JiNcVarShort(RandomAccessData in, JiNcVar parent, long offset)
	  {
	  super(in, parent, offset);
	  }  
	public Object read(JiDim[] dims) throws IOException
	  {
	  return readShort(dims);
	  }  
	public short[] readShort(JiDim[] dims) throws IOException
	  {
	  short[] rval = null;
	  mParent.validateDims(dims);
	  JiSlabIterator itr = new JiSlabIterator((JiNcSource)mParent.getSource(), mParent, dims);
	  int size = itr.size();
	  rval = new short[size];
	  JiSlab slab;
	  int counter = 0;
	  while((slab = itr.next()) != null){
	      mRFile.seek(mOffset + slab.mOffset);
	      for (int i=0; i < slab.mSize; ++i){
		  rval[counter++] = mRFile.readShort();
	      }
	  }
	  return rval;
	  }  
	public int sizeof()
	  {
	  return 2;
	  }  
}
