/*
		A basic implementation of the DeviceSetup class.
*/

import java.awt.*;

public class E1463Setup extends DeviceSetup
{
	public E1463Setup(Frame parent)
	{
		super(parent);

		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		setDeviceTitle("E1463-Optical Multichannel Analyzer EG&G PARC Model 1463");
		getContentPane().setLayout(null);
		setSize(489,373);
		{
			String[] tempString = new String[3];
			tempString[0] = "Start";
			tempString[1] = "End";
			tempString[2] = "Group";
			ScanMask.setColumnNames(tempString);
		}
		ScanMask.setNumRows(10);
		ScanMask.setOffsetNid(11);
		ScanMask.setLabelString("Scan Mask");
		getContentPane().add(ScanMask);
		ScanMask.setBounds(24,228,228,108);
		{
			String[] tempString = new String[3];
			tempString[0] = "INIT";
			tempString[1] = "ARM";
			tempString[2] = "STORE";
			deviceButtons1.setMethods(tempString);
		}
		getContentPane().add(deviceButtons1);
		deviceButtons1.setBounds(96,336,290,40);
		getContentPane().add(Dispatch);
		Dispatch.setBounds(300,264,131,40);
		deviceField1.setNumCols(20);
		deviceField1.setTextOnly(true);
		deviceField1.setOffsetNid(2);
		deviceField1.setLabelString("Comment: ");
		getContentPane().add(deviceField1);
		deviceField1.setBounds(12,12,312,48);
		deviceField2.setNumCols(4);
		deviceField2.setOffsetNid(1);
		deviceField2.setLabelString("GPIB Addr.:");
		getContentPane().add(deviceField2);
		deviceField2.setBounds(336,12,150,40);
		deviceChoice1.setOffsetNid(3);
		{
			String[] tempString = new String[2];
			tempString[0] = "EXTERNAL";
			tempString[1] = "SOFTWARE";
			deviceChoice1.setChoiceItems(tempString);
		}
		deviceChoice1.setLabelString("Trig. Mode:");
		getContentPane().add(deviceChoice1);
		deviceChoice1.setBounds(0,60,204,36);
		deviceField3.setNumCols(15);
		deviceField3.setOffsetNid(4);
		deviceField3.setLabelString("Trig. Source:    ");
		getContentPane().add(deviceField3);
		deviceField3.setBounds(204,60,288,36);
		deviceChoice2.setOffsetNid(5);
		{
			String[] tempString = new String[3];
			tempString[0] = "NORMAL";
			tempString[1] = "LINE";
			tempString[2] = "EXTERNAL";
			deviceChoice2.setChoiceItems(tempString);
		}
		deviceChoice2.setLabelString("Synch. Mode:");
		getContentPane().add(deviceChoice2);
		deviceChoice2.setBounds(0,96,204,36);
		deviceField4.setNumCols(15);
		deviceField4.setOffsetNid(6);
		deviceField4.setLabelString("Synch. Source:");
		getContentPane().add(deviceField4);
		deviceField4.setBounds(204,96,285,36);
		deviceChoice3.setOffsetNid(7);
		{
			String[] tempString = new String[2];
			tempString[0] = "NORMAL";
			tempString[1] = "BACKGROUND";
			deviceChoice3.setChoiceItems(tempString);
		}
		deviceChoice3.setLabelString("Frame one: ");
		getContentPane().add(deviceChoice3);
		deviceChoice3.setBounds(0,132,204,40);
		deviceField5.setNumCols(15);
		deviceField5.setOffsetNid(8);
		deviceField5.setLabelString("Exp. Time ");
		getContentPane().add(deviceField5);
		deviceField5.setBounds(216,132,252,40);
		deviceField6.setOffsetNid(9);
		deviceField6.setLabelString("N. Scans: ");
		getContentPane().add(deviceField6);
		deviceField6.setBounds(12,180,204,40);
		deviceField7.setOffsetNid(10);
		deviceField7.setLabelString("Head Temp.: ");
		getContentPane().add(deviceField7);
		deviceField7.setBounds(228,180,228,40);
		//}}
	}

	public E1463Setup()
	{
		this((Frame)null);
	}

	public E1463Setup(String sTitle)
	{
		this();
		setTitle(sTitle);
	}

	public void setVisible(boolean b)
	{
		if (b)
			setLocation(50, 50);
		super.setVisible(b);
	}

	static public void main(String args[])
	{
		(new E1463Setup()).setVisible(true);
	}

	public void addNotify()
	{
		// Record the size of the window prior to calling parents addNotify.
		Dimension size = getSize();

		super.addNotify();

		if (frameSizeAdjusted)
			return;
		frameSizeAdjusted = true;

		// Adjust size of frame according to the insets
		Insets insets = getInsets();
		setSize(insets.left + insets.right + size.width, insets.top + insets.bottom + size.height);
	}

	// Used by addNotify
	boolean frameSizeAdjusted = false;

	//{{DECLARE_CONTROLS
	DeviceTable ScanMask = new DeviceTable();
	DeviceButtons deviceButtons1 = new DeviceButtons();
	DeviceDispatch Dispatch = new DeviceDispatch();
	DeviceField deviceField1 = new DeviceField();
	DeviceField deviceField2 = new DeviceField();
	DeviceChoice deviceChoice1 = new DeviceChoice();
	DeviceField deviceField3 = new DeviceField();
	DeviceChoice deviceChoice2 = new DeviceChoice();
	DeviceField deviceField4 = new DeviceField();
	DeviceChoice deviceChoice3 = new DeviceChoice();
	DeviceField deviceField5 = new DeviceField();
	DeviceField deviceField6 = new DeviceField();
	DeviceField deviceField7 = new DeviceField();
	//}}

}