/*
		A basic implementation of the DeviceSetup class.
*/

import java.awt.*;
import javax.swing.JTabbedPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

public class TRCHSetup extends DeviceSetup
{
	public TRCHSetup(Frame parent)
	{
		super(parent);
		
		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		setDeviceTitle("INCAA TRCH Transient Recorder");
		getContentPane().setLayout(null);
		setSize(534,477);
		deviceField1.setNumCols(15);
		deviceField1.setTextOnly(true);
		deviceField1.setOffsetNid(1);
		deviceField1.setLabelString("CAMAC Name: ");
		getContentPane().add(deviceField1);
		deviceField1.setBounds(0,12,300,36);
		deviceField2.setNumCols(38);
		deviceField2.setTextOnly(true);
		deviceField2.setOffsetNid(2);
		deviceField2.setLabelString("Comment:");
		getContentPane().add(deviceField2);
		deviceField2.setBounds(0,48,528,36);
		deviceField3.setNumCols(35);
		deviceField3.setOffsetNid(6);
		deviceField3.setLabelString("Trig.  Source:");
		getContentPane().add(deviceField3);
		deviceField3.setBounds(0,120,516,40);
		deviceField4.setNumCols(35);
		deviceField4.setOffsetNid(7);
		deviceField4.setLabelString("Clock source:");
		getContentPane().add(deviceField4);
		deviceField4.setBounds(0,156,516,40);
		deviceChoice1.setOffsetNid(5);
		{
			String[] tempString = new String[2];
			tempString[0] = "INTERNAL";
			tempString[1] = "EXTERNAL";
			deviceChoice1.setChoiceItems(tempString);
		}
		deviceChoice1.setLabelString("Clock mode:");
		getContentPane().add(deviceChoice1);
		deviceChoice1.setBounds(24,84,192,36);
		deviceChoice2.setChoiceFloatValues(new float[] {(float)1000000.0,(float)500000.0,(float)250000.0,(float)125000.0,(float)50000.0,(float)10000.0,(float)5000.0});
		deviceChoice2.setOffsetNid(8);
		{
			String[] tempString = new String[7];
			tempString[0] = "1E6";
			tempString[1] = "500E3";
			tempString[2] = "250E3";
			tempString[3] = "125E3";
			tempString[4] = "50E3";
			tempString[5] = "10E3";
			tempString[6] = "5E3";
			deviceChoice2.setChoiceItems(tempString);
		}
		deviceChoice2.setLabelString("Frequency (Hz): ");
		getContentPane().add(deviceChoice2);
		deviceChoice2.setBounds(228,84,192,40);
		getContentPane().add(deviceDispatch1);
		deviceDispatch1.setBounds(360,12,136,40);
		deviceChoice3.setOffsetNid(3);
		{
			String[] tempString = new String[2];
			tempString[0] = "3";
			tempString[1] = "6";
			deviceChoice3.setChoiceItems(tempString);
		}
		deviceChoice3.setLabelString("Num. Channels:");
		deviceChoice3.setChoiceIntValues(new int[] {(int)3,(int)6});
		getContentPane().add(deviceChoice3);
		deviceChoice3.setBounds(0,192,156,40);
		deviceChoice4.setOffsetNid(4);
		{
			String[] tempString = new String[2];
			tempString[0] = "0";
			tempString[1] = "1";
			deviceChoice4.setChoiceItems(tempString);
		}
		deviceChoice4.setLabelString("Chan.Offset:");
		deviceChoice4.setChoiceIntValues(new int[] {(int)0,(int)1});
		getContentPane().add(deviceChoice4);
		deviceChoice4.setBounds(156,192,122,40);
		deviceChoice5.setOffsetNid(9);
		{
			String[] tempString = new String[2];
			tempString[0] = "TRUE";
			tempString[1] = "FALSE";
			deviceChoice5.setChoiceItems(tempString);
		}
		deviceChoice5.setLabelString("Use Time:");
		getContentPane().add(deviceChoice5);
		deviceChoice5.setBounds(288,192,144,40);
		{
			String[] tempString = new String[3];
			tempString[0] = "init";
			tempString[1] = "trigger";
			tempString[2] = "store";
			deviceButtons1.setMethods(tempString);
		}
		getContentPane().add(deviceButtons1);
		deviceButtons1.setBounds(108,432,324,40);
		getContentPane().add(JTabbedPane1);
		JTabbedPane1.setBounds(12,228,516,192);
		JScrollPane1.setOpaque(true);
		JTabbedPane1.add(JScrollPane1);
		JScrollPane1.setBounds(2,24,511,165);
		JScrollPane1.setVisible(false);
		JPanel1.setLayout(new GridLayout(6,1,0,0));
		JScrollPane1.getViewport().add(JPanel1);
		JPanel1.setBounds(0,0,498,246);
		deviceChannel1.setInSameLine(true);
		deviceChannel1.setOffsetNid(11);
		deviceChannel1.setLabelString("Ch1:");
		deviceChannel1.setBorderVisible(true);
		JPanel1.add(deviceChannel1);
		deviceField5.setNumCols(15);
		deviceField5.setOffsetNid(12);
		deviceField5.setLabelString("Start :");
		deviceChannel1.add(deviceField5);
		deviceField6.setNumCols(15);
		deviceField6.setOffsetNid(13);
		deviceField6.setLabelString("End :");
		deviceChannel1.add(deviceField6);
		deviceChannel2.setInSameLine(true);
		deviceChannel2.setOffsetNid(17);
		deviceChannel2.setLabelString("Ch2:");
		deviceChannel2.setBorderVisible(true);
		JPanel1.add(deviceChannel2);
		deviceField7.setNumCols(15);
		deviceField7.setOffsetNid(18);
		deviceField7.setLabelString("Start :");
		deviceChannel2.add(deviceField7);
		deviceField8.setNumCols(15);
		deviceField8.setOffsetNid(19);
		deviceField8.setLabelString("End :");
		deviceChannel2.add(deviceField8);
		deviceChannel3.setInSameLine(true);
		deviceChannel3.setOffsetNid(23);
		deviceChannel3.setLabelString("Ch3:");
		deviceChannel3.setBorderVisible(true);
		JPanel1.add(deviceChannel3);
		deviceField9.setNumCols(15);
		deviceField9.setOffsetNid(24);
		deviceField9.setLabelString("Start :");
		deviceChannel3.add(deviceField9);
		deviceField10.setNumCols(15);
		deviceField10.setOffsetNid(25);
		deviceField10.setLabelString("End :");
		deviceChannel3.add(deviceField10);
		deviceChannel4.setInSameLine(true);
		deviceChannel4.setOffsetNid(29);
		deviceChannel4.setLabelString("Ch4:");
		deviceChannel4.setBorderVisible(true);
		JPanel1.add(deviceChannel4);
		deviceField11.setNumCols(15);
		deviceField11.setOffsetNid(30);
		deviceField11.setLabelString("Start :");
		deviceChannel4.add(deviceField11);
		deviceField12.setNumCols(15);
		deviceField12.setOffsetNid(31);
		deviceField12.setLabelString("End :");
		deviceChannel4.add(deviceField12);
		deviceChannel5.setInSameLine(true);
		deviceChannel5.setOffsetNid(35);
		deviceChannel5.setLabelString("Ch5:");
		deviceChannel5.setBorderVisible(true);
		JPanel1.add(deviceChannel5);
		deviceField13.setNumCols(15);
		deviceField13.setOffsetNid(36);
		deviceField13.setLabelString("Start :");
		deviceChannel5.add(deviceField13);
		deviceField14.setNumCols(15);
		deviceField14.setOffsetNid(37);
		deviceField14.setLabelString("End :");
		deviceChannel5.add(deviceField14);
		deviceChannel6.setInSameLine(true);
		deviceChannel6.setOffsetNid(41);
		deviceChannel6.setLabelString("Ch6:");
		deviceChannel6.setBorderVisible(true);
		JPanel1.add(deviceChannel6);
		deviceField15.setNumCols(15);
		deviceField15.setOffsetNid(42);
		deviceField15.setLabelString("Start :");
		deviceChannel6.add(deviceField15);
		deviceField16.setNumCols(15);
		deviceField16.setOffsetNid(43);
		deviceField16.setLabelString("End :");
		deviceChannel6.add(deviceField16);
		JScrollPane2.setOpaque(true);
		JTabbedPane1.add(JScrollPane2);
		JScrollPane2.setBounds(2,24,511,165);
		JScrollPane2.setVisible(false);
		JPanel2.setLayout(new GridLayout(6,1,0,0));
		JScrollPane2.getViewport().add(JPanel2);
		JPanel2.setBounds(0,0,498,246);
		deviceChannel7.setInSameLine(true);
		deviceChannel7.setOffsetNid(11);
		deviceChannel7.setLabelString("Ch1:");
		deviceChannel7.setBorderVisible(true);
		JPanel2.add(deviceChannel7);
		deviceField17.setNumCols(15);
		deviceField17.setOffsetNid(14);
		deviceField17.setLabelString("Start :");
		deviceChannel7.add(deviceField17);
		deviceField18.setNumCols(15);
		deviceField18.setOffsetNid(15);
		deviceField18.setLabelString("End :");
		deviceChannel7.add(deviceField18);
		deviceChannel8.setInSameLine(true);
		deviceChannel8.setOffsetNid(17);
		deviceChannel8.setLabelString("Ch2:");
		deviceChannel8.setBorderVisible(true);
		JPanel2.add(deviceChannel8);
		deviceField19.setNumCols(15);
		deviceField19.setOffsetNid(20);
		deviceField19.setLabelString("Start :");
		deviceChannel8.add(deviceField19);
		deviceField20.setNumCols(15);
		deviceField20.setOffsetNid(21);
		deviceField20.setLabelString("End :");
		deviceChannel8.add(deviceField20);
		deviceChannel9.setInSameLine(true);
		deviceChannel9.setOffsetNid(23);
		deviceChannel9.setLabelString("Ch3:");
		deviceChannel9.setBorderVisible(true);
		JPanel2.add(deviceChannel9);
		deviceField21.setNumCols(15);
		deviceField21.setOffsetNid(26);
		deviceField21.setLabelString("Start :");
		deviceChannel9.add(deviceField21);
		deviceField22.setNumCols(15);
		deviceField22.setOffsetNid(27);
		deviceField22.setLabelString("End :");
		deviceChannel9.add(deviceField22);
		deviceChannel10.setInSameLine(true);
		deviceChannel10.setOffsetNid(29);
		deviceChannel10.setLabelString("Ch4:");
		deviceChannel10.setBorderVisible(true);
		JPanel2.add(deviceChannel10);
		deviceField23.setNumCols(15);
		deviceField23.setOffsetNid(32);
		deviceField23.setLabelString("Start :");
		deviceChannel10.add(deviceField23);
		deviceField24.setNumCols(15);
		deviceField24.setOffsetNid(39);
		deviceField24.setLabelString("End :");
		deviceChannel10.add(deviceField24);
		deviceChannel11.setInSameLine(true);
		deviceChannel11.setOffsetNid(35);
		deviceChannel11.setLabelString("Ch5:");
		deviceChannel11.setBorderVisible(true);
		JPanel2.add(deviceChannel11);
		deviceField25.setNumCols(15);
		deviceField25.setOffsetNid(36);
		deviceField25.setLabelString("Start :");
		deviceChannel11.add(deviceField25);
		deviceField26.setNumCols(15);
		deviceField26.setOffsetNid(37);
		deviceField26.setLabelString("End :");
		deviceChannel11.add(deviceField26);
		deviceChannel12.setInSameLine(true);
		deviceChannel12.setOffsetNid(41);
		deviceChannel12.setLabelString("Ch6:");
		deviceChannel12.setBorderVisible(true);
		JPanel2.add(deviceChannel12);
		deviceField27.setNumCols(15);
		deviceField27.setOffsetNid(42);
		deviceField27.setLabelString("Start :");
		deviceChannel12.add(deviceField27);
		deviceField28.setNumCols(15);
		deviceField28.setOffsetNid(43);
		deviceField28.setLabelString("End :");
		deviceChannel12.add(deviceField28);
		JTabbedPane1.setSelectedIndex(0);
		JTabbedPane1.setSelectedComponent(JScrollPane1);
		JTabbedPane1.setTitleAt(0,"Channels(Time)");
		JTabbedPane1.setTitleAt(1,"Channels(Samples)");
		//}}
	}

	public TRCHSetup()
	{
		this((Frame)null);
	}

	public TRCHSetup(String sTitle)
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
		(new TRCHSetup()).setVisible(true);
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
	DeviceField deviceField1 = new DeviceField();
	DeviceField deviceField2 = new DeviceField();
	DeviceField deviceField3 = new DeviceField();
	DeviceField deviceField4 = new DeviceField();
	DeviceChoice deviceChoice1 = new DeviceChoice();
	DeviceChoice deviceChoice2 = new DeviceChoice();
	DeviceDispatch deviceDispatch1 = new DeviceDispatch();
	DeviceChoice deviceChoice3 = new DeviceChoice();
	DeviceChoice deviceChoice4 = new DeviceChoice();
	DeviceChoice deviceChoice5 = new DeviceChoice();
	DeviceButtons deviceButtons1 = new DeviceButtons();
	javax.swing.JTabbedPane JTabbedPane1 = new javax.swing.JTabbedPane();
	javax.swing.JScrollPane JScrollPane1 = new javax.swing.JScrollPane();
	javax.swing.JPanel JPanel1 = new javax.swing.JPanel();
	DeviceChannel deviceChannel1 = new DeviceChannel();
	DeviceField deviceField5 = new DeviceField();
	DeviceField deviceField6 = new DeviceField();
	DeviceChannel deviceChannel2 = new DeviceChannel();
	DeviceField deviceField7 = new DeviceField();
	DeviceField deviceField8 = new DeviceField();
	DeviceChannel deviceChannel3 = new DeviceChannel();
	DeviceField deviceField9 = new DeviceField();
	DeviceField deviceField10 = new DeviceField();
	DeviceChannel deviceChannel4 = new DeviceChannel();
	DeviceField deviceField11 = new DeviceField();
	DeviceField deviceField12 = new DeviceField();
	DeviceChannel deviceChannel5 = new DeviceChannel();
	DeviceField deviceField13 = new DeviceField();
	DeviceField deviceField14 = new DeviceField();
	DeviceChannel deviceChannel6 = new DeviceChannel();
	DeviceField deviceField15 = new DeviceField();
	DeviceField deviceField16 = new DeviceField();
	javax.swing.JScrollPane JScrollPane2 = new javax.swing.JScrollPane();
	javax.swing.JPanel JPanel2 = new javax.swing.JPanel();
	DeviceChannel deviceChannel7 = new DeviceChannel();
	DeviceField deviceField17 = new DeviceField();
	DeviceField deviceField18 = new DeviceField();
	DeviceChannel deviceChannel8 = new DeviceChannel();
	DeviceField deviceField19 = new DeviceField();
	DeviceField deviceField20 = new DeviceField();
	DeviceChannel deviceChannel9 = new DeviceChannel();
	DeviceField deviceField21 = new DeviceField();
	DeviceField deviceField22 = new DeviceField();
	DeviceChannel deviceChannel10 = new DeviceChannel();
	DeviceField deviceField23 = new DeviceField();
	DeviceField deviceField24 = new DeviceField();
	DeviceChannel deviceChannel11 = new DeviceChannel();
	DeviceField deviceField25 = new DeviceField();
	DeviceField deviceField26 = new DeviceField();
	DeviceChannel deviceChannel12 = new DeviceChannel();
	DeviceField deviceField27 = new DeviceField();
	DeviceField deviceField28 = new DeviceField();
	//}}

}