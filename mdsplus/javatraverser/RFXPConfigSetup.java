/*
		A basic implementation of the DeviceSetup class.
*/

import java.awt.*;
import javax.swing.JPanel;
import javax.swing.*;
import javax.swing.border.*;
//import com.symantec.itools.javax.swing.borders.EtchedBorder;
import javax.swing.border.*;

public class RFXPConfigSetup extends DeviceSetup
{
	public RFXPConfigSetup(Frame parent)
	{
		super(parent);
		
		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		setDeviceProvider("150.178.3.112");
		setDeviceTitle("RFX Poloidal Configuration");
		setDeviceType("RFXPConfig");
		getContentPane().setLayout(null);
		setSize(556,457);
		deviceField1.setNumCols(35);
		deviceField1.setTextOnly(true);
		deviceField1.setOffsetNid(1);
		deviceField1.setLabelString("Comment: ");
		getContentPane().add(deviceField1);
		deviceField1.setBounds(12,12,468,40);
		deviceField2.setNumCols(8);
		deviceField2.setOffsetNid(6);
		deviceField2.setLabelString("   Plasma Ind. (mH): ");
		getContentPane().add(deviceField2);
		deviceField2.setBounds(24,84,216,40);
		deviceField3.setNumCols(8);
		deviceField3.setOffsetNid(4);
		deviceField3.setLabelString("Load Induct. (mH):");
		getContentPane().add(deviceField3);
		deviceField3.setBounds(12,48,252,40);
		deviceField4.setNumCols(8);
		deviceField4.setOffsetNid(5);
		deviceField4.setLabelString("  Load Res.(Ohm):");
		getContentPane().add(deviceField4);
		deviceField4.setBounds(276,48,228,40);
		deviceField15.setOffsetNid(20);
		deviceField15.setLabelString("Max I Magn (A):");
		getContentPane().add(deviceField15);
		deviceField15.setBounds(300,360,240,40);
		getContentPane().add(deviceButtons1);
		deviceButtons1.setBounds(144,408,281,40);
		JPanel1.setBorder(etchedBorder1);
		JPanel1.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		getContentPane().add(JPanel1);
		JPanel1.setBounds(12,132,264,132);
		deviceField5.setNumCols(15);
		deviceField5.setTextOnly(true);
		deviceField5.setOffsetNid(7);
		deviceField5.setLabelString("PM Units: ");
		deviceField5.setEditable(false);
		JPanel1.add(deviceField5);
		deviceField7.setTextOnly(true);
		deviceField7.setOffsetNid(8);
		deviceField7.setLabelString("PM Connection: ");
		deviceField7.setEditable(false);
		JPanel1.add(deviceField7);
		deviceField8.setNumCols(4);
		deviceField8.setOffsetNid(10);
		deviceField8.setLabelString("PM Window (s): ");
		deviceField8.setEditable(false);
		JPanel1.add(deviceField8);
		JPanel2.setBorder(etchedBorder1);
		JPanel2.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		getContentPane().add(JPanel2);
		JPanel2.setBounds(276,132,264,132);
		deviceField6.setNumCols(15);
		deviceField6.setTextOnly(true);
		deviceField6.setOffsetNid(11);
		deviceField6.setLabelString("Bias Units: ");
		deviceField6.setEditable(false);
		JPanel2.add(deviceField6);
		deviceField9.setTextOnly(true);
		deviceField9.setOffsetNid(12);
		deviceField9.setLabelString("Bias Connection: ");
		deviceField9.setEditable(false);
		JPanel2.add(deviceField9);
		deviceField10.setNumCols(4);
		deviceField10.setOffsetNid(1);
		deviceField10.setLabelString("Bias Window (s):");
		deviceField10.setEditable(false);
		JPanel2.add(deviceField10);
		JPanel3.setBorder(etchedBorder1);
		JPanel3.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		getContentPane().add(JPanel3);
		JPanel3.setBounds(12,264,528,84);
		deviceField11.setNumCols(30);
		deviceField11.setTextOnly(true);
		deviceField11.setOffsetNid(15);
		deviceField11.setLabelString("PV Units: ");
		deviceField11.setEditable(false);
		JPanel3.add(deviceField11);
		deviceField12.setTextOnly(true);
		deviceField12.setOffsetNid(16);
		deviceField12.setLabelString("PV Config: ");
		deviceField12.setEditable(false);
		JPanel3.add(deviceField12);
		deviceField13.setNumCols(4);
		deviceField13.setOffsetNid(18);
		deviceField13.setLabelString("PV Window (s):");
		deviceField13.setEditable(false);
		JPanel3.add(deviceField13);
		//$$ etchedBorder1.move(24,417);
		deviceField16.setOffsetNid(21);
		deviceField16.setLabelString("PTCB Voltage: ");
		getContentPane().add(deviceField16);
		deviceField16.setBounds(12,360,276,40);
		deviceField14.setNumCols(8);
		deviceField14.setOffsetNid(19);
		deviceField14.setLabelString("Transfer R (Ohm):");
		getContentPane().add(deviceField14);
		deviceField14.setBounds(276,84,228,40);
		//}}
	}

	public RFXPConfigSetup()
	{
		this((Frame)null);
	}

	public RFXPConfigSetup(String sTitle)
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
		(new RFXPConfigSetup()).setVisible(true);
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
	DeviceField deviceField15 = new DeviceField();
	DeviceButtons deviceButtons1 = new DeviceButtons();
	javax.swing.JPanel JPanel1 = new javax.swing.JPanel();
	DeviceField deviceField5 = new DeviceField();
	DeviceField deviceField7 = new DeviceField();
	DeviceField deviceField8 = new DeviceField();
	javax.swing.JPanel JPanel2 = new javax.swing.JPanel();
	DeviceField deviceField6 = new DeviceField();
	DeviceField deviceField9 = new DeviceField();
	DeviceField deviceField10 = new DeviceField();
	javax.swing.JPanel JPanel3 = new javax.swing.JPanel();
	DeviceField deviceField11 = new DeviceField();
	DeviceField deviceField12 = new DeviceField();
	DeviceField deviceField13 = new DeviceField();
	//com.symantec.itools.javax.swing.borders.EtchedBorder etchedBorder1 = new com.symantec.itools.javax.swing.borders.EtchedBorder();
	EtchedBorder etchedBorder1 = new EtchedBorder();
	DeviceField deviceField16 = new DeviceField();
	DeviceField deviceField14 = new DeviceField();
	//}}

}