/*
		A basic implementation of the DeviceSetup class.
*/

import java.awt.*;
import javax.swing.JTabbedPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.JRootPane;
import javax.swing.JLabel;

public class T2ControlSetup extends DeviceSetup
{
	public T2ControlSetup(Frame parent)
	{
		super(parent);

		// This code is automatically generated by Visual Cafe when you add
		// components to the visual environment. It instantiates and initializes
		// the components. To modify the code, only use code syntax that matches
		// what Visual Cafe can generate, or Visual Cafe may be unable to back
		// parse your Java file into its visual environment.
		//{{INIT_CONTROLS
		setDeviceProvider("localhost");
		setDeviceTitle("Control Supervision for T2");
		setDeviceType("T2Control");
		getContentPane().setLayout(new BorderLayout(0,0));
		setSize(716,524);
		JPanel13.setLayout(new BorderLayout(0,0));
		getContentPane().add(BorderLayout.CENTER,JPanel13);
		JTabbedPane1.setAlignmentX(1.0F);
		JPanel13.add(BorderLayout.CENTER,JTabbedPane1);
		JPanel1.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JTabbedPane1.add(JPanel1);
		JPanel1.setBounds(2,24,711,277);
		JPanel1.setVisible(false);
		deviceField5.setNumCols(5);
		deviceField5.setOffsetNid(14);
		deviceField5.setLabelString("Kp:");
		JPanel1.add(deviceField5);
		deviceField6.setNumCols(5);
		deviceField6.setOffsetNid(16);
		deviceField6.setLabelString("Ki:");
		JPanel1.add(deviceField6);
		deviceField48.setNumCols(5);
		deviceField48.setOffsetNid(18);
		deviceField48.setLabelString("Kd:");
		JPanel1.add(deviceField48);
		deviceChoice4.setOffsetNid(106);
		{
			String[] tempString = new String[33];
			tempString[0] = "-16";
			tempString[1] = "-15";
			tempString[2] = "-14";
			tempString[3] = "-13";
			tempString[4] = "-12";
			tempString[5] = "-11";
			tempString[6] = "-10";
			tempString[7] = "-9";
			tempString[8] = "-8";
			tempString[9] = "-7";
			tempString[10] = "-6";
			tempString[11] = "-5";
			tempString[12] = "-4";
			tempString[13] = "-3";
			tempString[14] = "-2";
			tempString[15] = "-1";
			tempString[16] = "0";
			tempString[17] = "1";
			tempString[18] = "2";
			tempString[19] = "3";
			tempString[20] = "4";
			tempString[21] = "5";
			tempString[22] = "6";
			tempString[23] = "7";
			tempString[24] = "8";
			tempString[25] = "9";
			tempString[26] = "10";
			tempString[27] = "11";
			tempString[28] = "12";
			tempString[29] = "13";
			tempString[30] = "14";
			tempString[31] = "15";
			tempString[32] = "None";
			deviceChoice4.setChoiceItems(tempString);
		}
		deviceChoice4.setLabelString("Exclude mode:");
		deviceChoice4.setConvert(true);
		deviceChoice4.setChoiceIntValues(new int[] {(int)-16,(int)-15,(int)-14,(int)-13,(int)-12,(int)-11,(int)-10,(int)-9,(int)-8,(int)-7,(int)-6,(int)-5,(int)-4,(int)-3,(int)-2,(int)-1,(int)0,(int)1,(int)2,(int)3,(int)4,(int)5,(int)6,(int)7,(int)8,(int)9,(int)10,(int)11,(int)12,(int)13,(int)14,(int)15,(int)1000});
		JPanel1.add(deviceChoice4);
		deviceChoice5.setOffsetNid(108);
		{
			String[] tempString = new String[33];
			tempString[0] = "-16";
			tempString[1] = "-15";
			tempString[2] = "-14";
			tempString[3] = "-13";
			tempString[4] = "-12";
			tempString[5] = "-11";
			tempString[6] = "-10";
			tempString[7] = "-9";
			tempString[8] = "-8";
			tempString[9] = "-7";
			tempString[10] = "-6";
			tempString[11] = "-5";
			tempString[12] = "-4";
			tempString[13] = "-3";
			tempString[14] = "-2";
			tempString[15] = "-1";
			tempString[16] = "0";
			tempString[17] = "1";
			tempString[18] = "2";
			tempString[19] = "3";
			tempString[20] = "4";
			tempString[21] = "5";
			tempString[22] = "6";
			tempString[23] = "7";
			tempString[24] = "8";
			tempString[25] = "9";
			tempString[26] = "10";
			tempString[27] = "11";
			tempString[28] = "12";
			tempString[29] = "13";
			tempString[30] = "14";
			tempString[31] = "15";
			tempString[32] = "None";
			deviceChoice5.setChoiceItems(tempString);
		}
		deviceChoice5.setLabelString("Exclude mode:");
		deviceChoice5.setConvert(true);
		deviceChoice5.setChoiceIntValues(new int[] {(int)-16,(int)-15,(int)-14,(int)-13,(int)-12,(int)-11,(int)-10,(int)-9,(int)-8,(int)-7,(int)-6,(int)-5,(int)-4,(int)-3,(int)-2,(int)-1,(int)0,(int)1,(int)2,(int)3,(int)4,(int)5,(int)6,(int)7,(int)8,(int)9,(int)10,(int)11,(int)12,(int)13,(int)14,(int)15,(int)1000});
		JPanel1.add(deviceChoice5);
		JPanel4.setLayout(new GridLayout(4,4,0,0));
		JTabbedPane1.add(JPanel4);
		JPanel4.setBounds(2,24,711,277);
		JPanel4.setVisible(false);
		deviceField17.setNumCols(2);
		deviceField17.setIdentifier("id1");
		deviceField17.setOffsetNid(68);
		deviceField17.setLabelString("Out id:");
		JPanel4.add(deviceField17);
		deviceField18.setNumCols(4);
		deviceField18.setOffsetNid(70);
		deviceField18.setLabelString("Amp.:");
		JPanel4.add(deviceField18);
		deviceField19.setNumCols(4);
		deviceField19.setOffsetNid(72);
		deviceField19.setLabelString("Omega:");
		JPanel4.add(deviceField19);
		deviceField20.setNumCols(4);
		deviceField20.setOffsetNid(74);
		deviceField20.setLabelString("Phi:");
		JPanel4.add(deviceField20);
		deviceField21.setNumCols(2);
		deviceField21.setIdentifier("id2");
		deviceField21.setOffsetNid(76);
		deviceField21.setLabelString("Out id:");
		JPanel4.add(deviceField21);
		deviceField22.setNumCols(4);
		deviceField22.setOffsetNid(78);
		deviceField22.setLabelString("Amp.:");
		JPanel4.add(deviceField22);
		deviceField23.setNumCols(4);
		deviceField23.setOffsetNid(80);
		deviceField23.setLabelString("Omega:");
		JPanel4.add(deviceField23);
		deviceField24.setNumCols(4);
		deviceField24.setOffsetNid(82);
		deviceField24.setLabelString("Phi:");
		JPanel4.add(deviceField24);
		deviceField25.setNumCols(2);
		deviceField25.setIdentifier("id3");
		deviceField25.setOffsetNid(84);
		deviceField25.setLabelString("Out id:");
		JPanel4.add(deviceField25);
		deviceField26.setNumCols(4);
		deviceField26.setOffsetNid(86);
		deviceField26.setLabelString("Amp.:");
		JPanel4.add(deviceField26);
		deviceField27.setNumCols(4);
		deviceField27.setOffsetNid(88);
		deviceField27.setLabelString("Omega:");
		JPanel4.add(deviceField27);
		deviceField28.setNumCols(4);
		deviceField28.setOffsetNid(90);
		deviceField28.setLabelString("Phi:");
		JPanel4.add(deviceField28);
		deviceField29.setNumCols(2);
		deviceField29.setIdentifier("id4");
		deviceField29.setOffsetNid(92);
		deviceField29.setLabelString("Out id:");
		JPanel4.add(deviceField29);
		deviceField30.setNumCols(4);
		deviceField30.setOffsetNid(94);
		deviceField30.setLabelString("Amp.:");
		JPanel4.add(deviceField30);
		deviceField31.setNumCols(4);
		deviceField31.setOffsetNid(96);
		deviceField31.setLabelString("Omega:");
		JPanel4.add(deviceField31);
		deviceField32.setNumCols(4);
		deviceField32.setOffsetNid(98);
		deviceField32.setLabelString("Phi:");
		JPanel4.add(deviceField32);
		JScrollPane1.setAutoscrolls(true);
		JScrollPane1.setDoubleBuffered(true);
		JTabbedPane1.add(JScrollPane1);
		JScrollPane1.setBounds(2,24,711,277);
		JScrollPane1.setVisible(false);
		JViewport1.setExtentSize(new java.awt.Dimension(797,224));
		JViewport1.setView(JPanel2);
		JViewport1.setViewSize(new java.awt.Dimension(797,156));
		JScrollPane1.getViewport().add(JViewport1);
		JViewport1.setBounds(0,0,797,259);
		JPanel2.setLayout(new GridLayout(4,1,0,0));
		JViewport1.add(JPanel2);
		JPanel2.setBounds(0,0,797,259);
		JPanel5.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel2.add(JPanel5);
		deviceField8.setNumCols(2);
		deviceField8.setIdentifier("n1");
		deviceField8.setOffsetNid(20);
		deviceField8.setLabelString("N 1:");
		JPanel5.add(deviceField8);
		deviceField9.setNumCols(4);
		deviceField9.setOffsetNid(22);
		deviceField9.setLabelString("Amp. : ");
		JPanel5.add(deviceField9);
		deviceField10.setNumCols(4);
		deviceField10.setOffsetNid(24);
		deviceField10.setLabelString("Freq. (Hz)");
		JPanel5.add(deviceField10);
		deviceField11.setNumCols(4);
		deviceField11.setOffsetNid(26);
		deviceField11.setLabelString("Phase (Rad):");
		JPanel5.add(deviceField11);
		deviceField7.setNumCols(6);
		deviceField7.setOffsetNid(28);
		deviceField7.setLabelString("Rot. Freq.(Hz):");
		JPanel5.add(deviceField7);
		deviceField49.setNumCols(6);
		deviceField49.setOffsetNid(30);
		deviceField49.setLabelString("Rot. Phase(Rad):");
		JPanel5.add(deviceField49);
		JPanel6.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel2.add(JPanel6);
		deviceField36.setNumCols(2);
		deviceField36.setIdentifier("n2");
		deviceField36.setOffsetNid(32);
		deviceField36.setLabelString("N 2:");
		JPanel6.add(deviceField36);
		deviceField37.setNumCols(4);
		deviceField37.setOffsetNid(34);
		deviceField37.setLabelString("Amp. : ");
		JPanel6.add(deviceField37);
		deviceField38.setNumCols(4);
		deviceField38.setOffsetNid(36);
		deviceField38.setLabelString("Freq. (Hz)");
		JPanel6.add(deviceField38);
		deviceField39.setNumCols(4);
		deviceField39.setOffsetNid(38);
		deviceField39.setLabelString("Phase (Rad):");
		JPanel6.add(deviceField39);
		deviceField50.setNumCols(6);
		deviceField50.setOffsetNid(40);
		deviceField50.setLabelString("Rot. Freq.(Hz):");
		JPanel6.add(deviceField50);
		deviceField51.setNumCols(6);
		deviceField51.setOffsetNid(42);
		deviceField51.setLabelString("Rot. Phase(Rad):");
		JPanel6.add(deviceField51);
		JPanel7.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel2.add(JPanel7);
		deviceField40.setNumCols(2);
		deviceField40.setIdentifier("n3");
		deviceField40.setOffsetNid(44);
		deviceField40.setLabelString("N 3:");
		JPanel7.add(deviceField40);
		deviceField41.setNumCols(4);
		deviceField41.setOffsetNid(46);
		deviceField41.setLabelString("Amp. : ");
		JPanel7.add(deviceField41);
		deviceField42.setNumCols(4);
		deviceField42.setOffsetNid(48);
		deviceField42.setLabelString("Freq. (Hz)");
		JPanel7.add(deviceField42);
		deviceField43.setNumCols(4);
		deviceField43.setOffsetNid(50);
		deviceField43.setLabelString("Phase (Rad):");
		JPanel7.add(deviceField43);
		deviceField52.setNumCols(6);
		deviceField52.setOffsetNid(52);
		deviceField52.setLabelString("Rot. Freq.(Hz):");
		JPanel7.add(deviceField52);
		deviceField53.setNumCols(6);
		deviceField53.setOffsetNid(54);
		deviceField53.setLabelString("Rot. Phase(Rad):");
		JPanel7.add(deviceField53);
		JPanel8.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel2.add(JPanel8);
		deviceField44.setNumCols(2);
		deviceField44.setIdentifier("n4");
		deviceField44.setOffsetNid(56);
		deviceField44.setLabelString("N 4:");
		JPanel8.add(deviceField44);
		deviceField45.setNumCols(4);
		deviceField45.setOffsetNid(58);
		deviceField45.setLabelString("Amp. : ");
		JPanel8.add(deviceField45);
		deviceField46.setNumCols(4);
		deviceField46.setOffsetNid(60);
		deviceField46.setLabelString("Freq. (Hz)");
		JPanel8.add(deviceField46);
		deviceField47.setNumCols(4);
		deviceField47.setOffsetNid(62);
		deviceField47.setLabelString("Phase (Rad):");
		JPanel8.add(deviceField47);
		deviceField54.setNumCols(6);
		deviceField54.setOffsetNid(64);
		deviceField54.setLabelString("Rot. Freq.(Hz):");
		JPanel8.add(deviceField54);
		deviceField55.setNumCols(6);
		deviceField55.setOffsetNid(66);
		deviceField55.setLabelString("Rot. Phase(Rad):");
		JPanel8.add(deviceField55);
		JScrollPane2.setAutoscrolls(true);
		JScrollPane2.setDoubleBuffered(true);
		JTabbedPane1.add(JScrollPane2);
		JScrollPane2.setBounds(2,24,711,277);
		JScrollPane2.setVisible(false);
		JViewport2.setExtentSize(new java.awt.Dimension(1613,279));
		JViewport2.setView(JPanel3);
		JViewport2.setViewSize(new java.awt.Dimension(1613,279));
		JScrollPane2.getViewport().add(JViewport2);
		JViewport2.setBounds(0,0,1613,279);
		JPanel3.setLayout(new BorderLayout(0,0));
		JViewport2.add(JPanel3);
		JPanel3.setBounds(0,0,1613,279);
		JPanel9.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel3.add(BorderLayout.NORTH, JPanel9);
                deviceTable3.setDisplayRowNumber(true);
		deviceTable3.setNumCols(32);
		{
			String[] tempString = new String[32];
			tempString[0] = "0";
			tempString[1] = "1";
			tempString[2] = "2";
			tempString[3] = "3";
			tempString[4] = "4";
			tempString[5] = "5";
			tempString[6] = "6";
			tempString[7] = "7";
			tempString[8] = "8";
			tempString[9] = "9";
			tempString[10] = "10";
			tempString[11] = "11";
			tempString[12] = "12";
			tempString[13] = "13";
			tempString[14] = "14";
			tempString[15] = "15";
			tempString[16] = "-16";
			tempString[17] = "-15";
			tempString[18] = "-14";
			tempString[19] = "-13";
			tempString[20] = "-12";
			tempString[21] = "-11";
			tempString[22] = "-10";
			tempString[23] = "-9";
			tempString[24] = "-8";
			tempString[25] = "-7";
			tempString[26] = "-6";
			tempString[27] = "-5";
			tempString[28] = "-4";
			tempString[29] = "-3";
			tempString[30] = "-2";
			tempString[31] = "-1";
			deviceTable3.setColumnNames(tempString);
		}
		deviceTable3.setNumRows(32);
		deviceTable3.setOffsetNid(100);
		deviceTable3.setLabelString("Kp");
		{
			String[] tempString = new String[32];
			tempString[0] = "0";
			tempString[1] = "1";
			tempString[2] = "2";
			tempString[3] = "3";
			tempString[4] = "4";
			tempString[5] = "5";
			tempString[6] = "6";
			tempString[7] = "7";
			tempString[8] = "8";
			tempString[9] = "9";
			tempString[10] = "10";
			tempString[11] = "11";
			tempString[12] = "12";
			tempString[13] = "13";
			tempString[14] = "14";
			tempString[15] = "15";
			tempString[16] = "-16";
			tempString[17] = "-15";
			tempString[18] = "-14";
			tempString[19] = "-13";
			tempString[20] = "-12";
			tempString[21] = "-11";
			tempString[22] = "-10";
			tempString[23] = "-9";
			tempString[24] = "-8";
			tempString[25] = "-7";
			tempString[26] = "-6";
			tempString[27] = "-5";
			tempString[28] = "-4";
			tempString[29] = "-3";
			tempString[30] = "-2";
			tempString[31] = "-1";
			deviceTable3.setRowNames(tempString);
		}
		JPanel9.add(deviceTable3);
		JPanel10.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel3.add(BorderLayout.CENTER, JPanel10);
                deviceTable4.setDisplayRowNumber(true);
		deviceTable4.setNumCols(32);
		{
			String[] tempString = new String[32];
			tempString[0] = "0";
			tempString[1] = "1";
			tempString[2] = "2";
			tempString[3] = "3";
			tempString[4] = "4";
			tempString[5] = "5";
			tempString[6] = "6";
			tempString[7] = "7";
			tempString[8] = "8";
			tempString[9] = "9";
			tempString[10] = "10";
			tempString[11] = "11";
			tempString[12] = "12";
			tempString[13] = "13";
			tempString[14] = "14";
			tempString[15] = "15";
			tempString[16] = "-16";
			tempString[17] = "-15";
			tempString[18] = "-14";
			tempString[19] = "-13";
			tempString[20] = "-12";
			tempString[21] = "-11";
			tempString[22] = "-10";
			tempString[23] = "-9";
			tempString[24] = "-8";
			tempString[25] = "-7";
			tempString[26] = "-6";
			tempString[27] = "-5";
			tempString[28] = "-4";
			tempString[29] = "-3";
			tempString[30] = "-2";
			tempString[31] = "-1";
			deviceTable4.setColumnNames(tempString);
		}
		deviceTable4.setNumRows(32);
		deviceTable4.setOffsetNid(102);
		deviceTable4.setLabelString("Ki");
		{
			String[] tempString = new String[32];
			tempString[0] = "0";
			tempString[1] = "1";
			tempString[2] = "2";
			tempString[3] = "3";
			tempString[4] = "4";
			tempString[5] = "5";
			tempString[6] = "6";
			tempString[7] = "7";
			tempString[8] = "8";
			tempString[9] = "9";
			tempString[10] = "10";
			tempString[11] = "11";
			tempString[12] = "12";
			tempString[13] = "13";
			tempString[14] = "14";
			tempString[15] = "15";
			tempString[16] = "-16";
			tempString[17] = "-15";
			tempString[18] = "-14";
			tempString[19] = "-13";
			tempString[20] = "-12";
			tempString[21] = "-11";
			tempString[22] = "-10";
			tempString[23] = "-9";
			tempString[24] = "-8";
			tempString[25] = "-7";
			tempString[26] = "-6";
			tempString[27] = "-5";
			tempString[28] = "-4";
			tempString[29] = "-3";
			tempString[30] = "-2";
			tempString[31] = "-1";
			deviceTable4.setRowNames(tempString);
		}
		JPanel10.add(deviceTable4);
		JPanel11.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel3.add(BorderLayout.SOUTH, JPanel11);
		deviceTable5.setNumCols(32);
		{
			String[] tempString = new String[32];
			tempString[0] = "0";
			tempString[1] = "1";
			tempString[2] = "2";
			tempString[3] = "3";
			tempString[4] = "4";
			tempString[5] = "5";
			tempString[6] = "6";
			tempString[7] = "7";
			tempString[8] = "8";
			tempString[9] = "9";
			tempString[10] = "10";
			tempString[11] = "11";
			tempString[12] = "12";
			tempString[13] = "13";
			tempString[14] = "14";
			tempString[15] = "15";
			tempString[16] = "-16";
			tempString[17] = "-15";
			tempString[18] = "-14";
			tempString[19] = "-13";
			tempString[20] = "-12";
			tempString[21] = "-11";
			tempString[22] = "-10";
			tempString[23] = "-9";
			tempString[24] = "-8";
			tempString[25] = "-7";
			tempString[26] = "-6";
			tempString[27] = "-5";
			tempString[28] = "-4";
			tempString[29] = "-3";
			tempString[30] = "-2";
			tempString[31] = "-1";
			deviceTable5.setColumnNames(tempString);
		}
		deviceTable5.setNumRows(32);
		deviceTable5.setOffsetNid(104);
		deviceTable5.setLabelString("Kd");
		{
			String[] tempString = new String[32];
			tempString[0] = "0";
			tempString[1] = "1";
			tempString[2] = "2";
			tempString[3] = "3";
			tempString[4] = "4";
			tempString[5] = "5";
			tempString[6] = "6";
			tempString[7] = "7";
			tempString[8] = "8";
			tempString[9] = "9";
			tempString[10] = "10";
			tempString[11] = "11";
			tempString[12] = "12";
			tempString[13] = "13";
			tempString[14] = "14";
			tempString[15] = "15";
			tempString[16] = "-16";
			tempString[17] = "-15";
			tempString[18] = "-14";
			tempString[19] = "-13";
			tempString[20] = "-12";
			tempString[21] = "-11";
			tempString[22] = "-10";
			tempString[23] = "-9";
			tempString[24] = "-8";
			tempString[25] = "-7";
			tempString[26] = "-6";
			tempString[27] = "-5";
			tempString[28] = "-4";
			tempString[29] = "-3";
			tempString[30] = "-2";
			tempString[31] = "-1";
			deviceTable5.setRowNames(tempString);
		}
		JPanel11.add(deviceTable5);
		JTabbedPane1.setSelectedIndex(3);
		JTabbedPane1.setSelectedComponent(JScrollPane2);
		JTabbedPane1.setTitleAt(0,"Intelligent Shell");
		JTabbedPane1.setTitleAt(1,"Ind.Perturbation");
		JTabbedPane1.setTitleAt(2,"Rot. Perturbation");
		JTabbedPane1.setTitleAt(3,"Mode Control");
		{
			String[] tempString = new String[8];
			tempString[0] = "N1 must be between -16 and 15";
			tempString[1] = "Id 1 must be between 1 and 32";
			tempString[2] = "Id 2 must be between 1 and 32";
			tempString[3] = "Id 3 must be between 1 and 32";
			tempString[4] = "Id 4 must be between 1 and 32";
			tempString[5] = "N2 must be between -16 and 15";
			tempString[6] = "N3 must be between -16 and 15";
			tempString[7] = "N4 must be between -16 and 15";
			deviceButtons1.setCheckMessages(tempString);
		}
		{
			String[] tempString = new String[8];
			tempString[0] = "_n1 >= -15 && _n1 < 16";
			tempString[1] = "_id1 > 0 && _id1 < 33";
			tempString[2] = "_id2 > 0 && _id2 < 33";
			tempString[3] = "_id3 > 0 && _id3 < 33";
			tempString[4] = "_id4 > 0 && _id4 < 33";
			tempString[5] = "_n2 >= -16 && _n2< 16";
			tempString[6] = "_n3 >= -16 && _n3< 16";
			tempString[7] = "_n4 >= -16 && _n4< 16";
			deviceButtons1.setCheckExpressions(tempString);
		}
		{
			String[] tempString = new String[3];
			tempString[0] = "INIT";
			tempString[1] = "ABORT";
			tempString[2] = "STORE";
			deviceButtons1.setMethods(tempString);
		}
		getContentPane().add(BorderLayout.SOUTH,deviceButtons1);
		JPanel20.setLayout(new BorderLayout(0,0));
		getContentPane().add(BorderLayout.NORTH,JPanel20);
		JPanel12.setLayout(new GridLayout(5,1,0,0));
		JPanel20.add(BorderLayout.CENTER,JPanel12);
		JPanel14.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel12.add(JPanel14);
		deviceField1.setNumCols(25);
		deviceField1.setTextOnly(true);
		deviceField1.setOffsetNid(1);
		deviceField1.setLabelString("Comment: ");
		JPanel14.add(deviceField1);
		deviceField2.setNumCols(12);
		deviceField2.setTextOnly(true);
		deviceField2.setOffsetNid(2);
		deviceField2.setLabelString("VME IP:");
		JPanel14.add(deviceField2);
		JPanel15.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel12.add(JPanel15);
		deviceField16.setNumCols(5);
		deviceField16.setOffsetNid(8);
		deviceField16.setLabelString("Trigger time (s):");
		JPanel15.add(deviceField16);
		deviceField33.setNumCols(5);
		deviceField33.setIdentifier("pre_time");
		deviceField33.setOffsetNid(11);
		deviceField33.setLabelString("Start sampling time (s):");
		JPanel15.add(deviceField33);
		deviceField34.setNumCols(5);
		deviceField34.setIdentifier("post_time");
		deviceField34.setOffsetNid(12);
		deviceField34.setLabelString("End sampling time (s):");
		JPanel15.add(deviceField34);
		JPanel16.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel12.add(JPanel16);
		deviceField3.setNumCols(8);
		deviceField3.setIdentifier("frequency");
		deviceField3.setOffsetNid(3);
		deviceField3.setLabelString("Frequency (Hz): ");
		JPanel16.add(deviceField3);
		deviceField4.setNumCols(4);
		deviceField4.setIdentifier("contr_durat");
		deviceField4.setOffsetNid(9);
		deviceField4.setLabelString("Control duration (s):");
		JPanel16.add(deviceField4);
		deviceField35.setNumCols(5);
		deviceField35.setIdentifier("sys_durat");
		deviceField35.setOffsetNid(10);
		deviceField35.setLabelString("System duration (s):");
		JPanel16.add(deviceField35);
		JPanel17.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel12.add(JPanel17);
		deviceChoice1.setOffsetNid(6);
		{
			String[] tempString = new String[5];
			tempString[0] = "Offset Correction";
			tempString[1] = "Intelligent Shell";
			tempString[2] = "Rotating Perturbation";
			tempString[3] = "Mode Control";
			tempString[4] = "Individual Perturbation";
			deviceChoice1.setChoiceItems(tempString);
		}
		deviceChoice1.setLabelString("Initial Control");
		deviceChoice1.setConvert(true);
		deviceChoice1.setChoiceIntValues(new int[] {(int)1,(int)2,(int)3,(int)4,(int)5});
		JPanel17.add(deviceChoice1);
		deviceChoice2.setOffsetNid(7);
		{
			String[] tempString = new String[5];
			tempString[0] = "Offset Correction";
			tempString[1] = "Intelligent Shell";
			tempString[2] = "Rotating Perturbation";
			tempString[3] = "Mode Control";
			tempString[4] = "Individual Perturbation";
			deviceChoice2.setChoiceItems(tempString);
		}
		deviceChoice2.setLabelString("Triggered Control");
		deviceChoice2.setConvert(true);
		deviceChoice2.setChoiceIntValues(new int[] {(int)1,(int)2,(int)3,(int)4,(int)5});
		JPanel17.add(deviceChoice2);
		JPanel18.setLayout(new FlowLayout(FlowLayout.CENTER,5,5));
		JPanel12.add(JPanel18);
		deviceField56.setNumCols(4);
		deviceField56.setIdentifier("autozero_start");
		deviceField56.setOffsetNid(109);
		deviceField56.setLabelString("Autozero start (sec. before trigger):");
		JPanel18.add(deviceField56);
		deviceField57.setNumCols(4);
		deviceField57.setIdentifier("autozero_end");
		deviceField57.setOffsetNid(110);
		deviceField57.setLabelString("Autozero end:");
		JPanel18.add(deviceField57);
		deviceChoice3.setOffsetNid(112);
		{
			String[] tempString = new String[2];
			tempString[0] = "16 Coils";
			tempString[1] = "8 Coils";
			deviceChoice3.setChoiceItems(tempString);
		}
		deviceChoice3.setLabelString("Map:");
		deviceChoice3.setConvert(true);
		deviceChoice3.setChoiceIntValues(new int[] {(int)1,(int)2});
		JPanel18.add(deviceChoice3);
		JPanel19.setLayout(new GridLayout(2,1,0,0));
		JPanel20.add(BorderLayout.WEST,JPanel19);
		deviceTable1.setNumCols(2);
		deviceTable1.setDisplayRowNumber(true);
		{
			String[] tempString = new String[2];
			tempString[0] = "Gain";
			tempString[1] = "Offset";
			deviceTable1.setColumnNames(tempString);
		}
		deviceTable1.setNumRows(96);
		deviceTable1.setOffsetNid(4);
		deviceTable1.setLabelString("In Calibration");
		JPanel19.add(deviceTable1);
		deviceTable2.setNumCols(2);
		deviceTable2.setDisplayRowNumber(true);
		{
			String[] tempString = new String[2];
			tempString[0] = "Gain";
			tempString[1] = "Offset";
			deviceTable2.setColumnNames(tempString);
		}
		deviceTable2.setNumRows(32);
		deviceTable2.setOffsetNid(5);
		deviceTable2.setLabelString("Out Calibration");
		JPanel19.add(deviceTable2);
		//}}
	}

	public T2ControlSetup()
	{
		this((Frame)null);
	}

	public T2ControlSetup(String sTitle)
	{
		this();
		setTitle(sTitle);
	}

	static public void main(String args[])
	{
		(new T2ControlSetup()).setVisible(true);
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
	javax.swing.JPanel JPanel13 = new javax.swing.JPanel();
	javax.swing.JTabbedPane JTabbedPane1 = new javax.swing.JTabbedPane();
	javax.swing.JPanel JPanel1 = new javax.swing.JPanel();
	DeviceField deviceField5 = new DeviceField();
	DeviceField deviceField6 = new DeviceField();
	DeviceField deviceField48 = new DeviceField();
	DeviceChoice deviceChoice4 = new DeviceChoice();
	DeviceChoice deviceChoice5 = new DeviceChoice();
	javax.swing.JPanel JPanel4 = new javax.swing.JPanel();
	DeviceField deviceField17 = new DeviceField();
	DeviceField deviceField18 = new DeviceField();
	DeviceField deviceField19 = new DeviceField();
	DeviceField deviceField20 = new DeviceField();
	DeviceField deviceField21 = new DeviceField();
	DeviceField deviceField22 = new DeviceField();
	DeviceField deviceField23 = new DeviceField();
	DeviceField deviceField24 = new DeviceField();
	DeviceField deviceField25 = new DeviceField();
	DeviceField deviceField26 = new DeviceField();
	DeviceField deviceField27 = new DeviceField();
	DeviceField deviceField28 = new DeviceField();
	DeviceField deviceField29 = new DeviceField();
	DeviceField deviceField30 = new DeviceField();
	DeviceField deviceField31 = new DeviceField();
	DeviceField deviceField32 = new DeviceField();
	javax.swing.JScrollPane JScrollPane1 = new javax.swing.JScrollPane();
	javax.swing.JViewport JViewport1 = new javax.swing.JViewport();
	javax.swing.JPanel JPanel2 = new javax.swing.JPanel();
	javax.swing.JPanel JPanel5 = new javax.swing.JPanel();
	DeviceField deviceField8 = new DeviceField();
	DeviceField deviceField9 = new DeviceField();
	DeviceField deviceField10 = new DeviceField();
	DeviceField deviceField11 = new DeviceField();
	DeviceField deviceField7 = new DeviceField();
	DeviceField deviceField49 = new DeviceField();
	javax.swing.JPanel JPanel6 = new javax.swing.JPanel();
	DeviceField deviceField36 = new DeviceField();
	DeviceField deviceField37 = new DeviceField();
	DeviceField deviceField38 = new DeviceField();
	DeviceField deviceField39 = new DeviceField();
	DeviceField deviceField50 = new DeviceField();
	DeviceField deviceField51 = new DeviceField();
	javax.swing.JPanel JPanel7 = new javax.swing.JPanel();
	DeviceField deviceField40 = new DeviceField();
	DeviceField deviceField41 = new DeviceField();
	DeviceField deviceField42 = new DeviceField();
	DeviceField deviceField43 = new DeviceField();
	DeviceField deviceField52 = new DeviceField();
	DeviceField deviceField53 = new DeviceField();
	javax.swing.JPanel JPanel8 = new javax.swing.JPanel();
	DeviceField deviceField44 = new DeviceField();
	DeviceField deviceField45 = new DeviceField();
	DeviceField deviceField46 = new DeviceField();
	DeviceField deviceField47 = new DeviceField();
	DeviceField deviceField54 = new DeviceField();
	DeviceField deviceField55 = new DeviceField();
	javax.swing.JScrollPane JScrollPane2 = new javax.swing.JScrollPane();
	javax.swing.JViewport JViewport2 = new javax.swing.JViewport();
	javax.swing.JPanel JPanel3 = new javax.swing.JPanel();
	javax.swing.JPanel JPanel9 = new javax.swing.JPanel();
	DeviceTable deviceTable3 = new DeviceTable();
	javax.swing.JPanel JPanel10 = new javax.swing.JPanel();
	DeviceTable deviceTable4 = new DeviceTable();
	javax.swing.JPanel JPanel11 = new javax.swing.JPanel();
	DeviceTable deviceTable5 = new DeviceTable();
	DeviceButtons deviceButtons1 = new DeviceButtons();
	javax.swing.JPanel JPanel20 = new javax.swing.JPanel();
	javax.swing.JPanel JPanel12 = new javax.swing.JPanel();
	javax.swing.JPanel JPanel14 = new javax.swing.JPanel();
	DeviceField deviceField1 = new DeviceField();
	DeviceField deviceField2 = new DeviceField();
	javax.swing.JPanel JPanel15 = new javax.swing.JPanel();
	DeviceField deviceField16 = new DeviceField();
	DeviceField deviceField33 = new DeviceField();
	DeviceField deviceField34 = new DeviceField();
	javax.swing.JPanel JPanel16 = new javax.swing.JPanel();
	DeviceField deviceField3 = new DeviceField();
	DeviceField deviceField4 = new DeviceField();
	DeviceField deviceField35 = new DeviceField();
	javax.swing.JPanel JPanel17 = new javax.swing.JPanel();
	DeviceChoice deviceChoice1 = new DeviceChoice();
	DeviceChoice deviceChoice2 = new DeviceChoice();
	javax.swing.JPanel JPanel18 = new javax.swing.JPanel();
	DeviceField deviceField56 = new DeviceField();
	DeviceField deviceField57 = new DeviceField();
	DeviceChoice deviceChoice3 = new DeviceChoice();
	javax.swing.JPanel JPanel19 = new javax.swing.JPanel();
	DeviceTable deviceTable1 = new DeviceTable();
	DeviceTable deviceTable2 = new DeviceTable();
	//}}

}